#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

using std::map;
using std::string;
using std::vector;

using row = map<string, string>;
using res = vector<row>;

class Database {
    private:
        sqlite3* DB;

    public:
        Database(const std::string path) {
            if (sqlite3_open(path.c_str(), &DB) != SQLITE_OK) {
                throw std::runtime_error("Failed to open database: " 
                    + std::string(sqlite3_errmsg(DB)));
            }
            // enable WAL mode
            sqlite3_exec(DB, "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);
        }

        ~Database() {
            sqlite3_close(DB);
        }

    res query (string sql, vector<string> params = {}) {
        res result;
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Query failed: " 
                + std::string(sqlite3_errmsg(DB)));
        }

        // collect rows
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            row row;
            int cols = sqlite3_column_count(stmt);
            for (int i = 0; i < cols; i++) {
                std::string col = sqlite3_column_name(stmt, i);
                const char* val = (const char*)sqlite3_column_text(stmt, i);
                row[col] = val ? val : "";
            }
            result.push_back(row);
        }

        sqlite3_finalize(stmt);
        return result;
    }

};