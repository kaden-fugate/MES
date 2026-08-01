#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <unordered_map>
#include <stdexcept>

using std::string;   using std::runtime_error; using std::vector;
using std::variant;  using std::optional;      using std::unordered_map;
                     using std::visit;         using std::decay_t;
using std::is_same_v;using std::monostate;     using std::optional;

namespace mes {

    using sql_var = std::variant<std::monostate, int64_t, double, string>;

    struct row {
        unordered_map<string, sql_var> cols;

        template <typename T>
        T get(const string &name) const {
            return std::get<T>(cols.at(name));
        }
    }; // row

    class tx_err : public runtime_error {
        public:
            explicit tx_err(const string &msg) : runtime_error(msg) {}
    }; // tx_err

    class r_conn {
        protected:
            r_conn() = default;                                             // only derived classes can access
            sqlite3 *handle_ = nullptr;
            sqlite3_stmt *prepare(
                const string &sql, const vector<sql_var> &parms
            );

        public:
            virtual ~r_conn() = default;

            r_conn(const r_conn&)            = delete;                   // no CC or AOO to avoid duplicating db conns
            r_conn& operator=(const r_conn&) = delete;

            optional<row> query_one(
                const string &sql, const vector<sql_var> parms = {}
            );
            vector<row> query_all(
                const string &sql, const vector<sql_var> parms = {}
            );

    }; // r_conn

    class db_conn : public r_conn {
        public:
            explicit db_conn(const string &path);
            ~db_conn() override;

            void begin();
            void commit();
            void rollback();

            void execute(const string &sql, const vector<sql_var> &parms = {});
    }; // db_conn

} // namespace mes