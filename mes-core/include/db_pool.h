#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "db_conn.h"

using std::queue;               using std::unique_lock; using std::mutex;
using std::condition_variable;  using std::unique_ptr; 
using std::make_unique;         using std::lock_guard;


namespace mes {

    template <typename T>
    class db_pool {
        private:
            string                     path_;
            queue<unique_ptr<db_conn>> avail_;
            mutex                      mtx_;
            condition_variable         cv_;
        
            void release(unique_ptr<db_conn> conn) {
                lock_guard<mutex> lock(mtx_);
                avail_.push(std::move(conn));
                cv_.notify_one();
            }

        public:
            db_pool(const string &path, size_t pool_size) : path_(path) {
                for (size_t i = 0; i < pool_size; i++) {
                    avail_.push(make_unique<db_conn>(path_));
                }
            }

            class pool_conn {
                private:
                    db_pool &pool_;
                    unique_ptr<db_conn> conn_;
                public:
                    pool_conn(db_pool &pool, unique_ptr<db_conn> conn) 
                        : pool_(pool), conn_(std::move(conn)) {}
                    ~pool_conn() { if (conn_) pool_.release(std::move(conn_)); }

                    pool_conn(pool_conn&&)      = default;
                    pool_conn(const pool_conn&) = delete;

                    T& operator*()  { return *conn_; }
                    T* operator->() { return conn_.get(); }
                    
            }; // pool_conn
            
            pool_conn acquire() {
                unique_lock<mutex> lock(mtx_);
                cv_.wait(lock, [this] { return !avail_.empty(); } );
                unique_ptr<db_conn> conn = std::move(avail_.front());
                avail_.pop();
                return pool_conn(*this, std::move(conn));
            }

    }; // db_pool

} // namespace mes