#include "../include/db_conn.h"

namespace mes {

/******************************************************************************
**                          READ-ONLY DB CONNECTION                          **
******************************************************************************/

void r_conn::begin() {
 
    if (sqlite3_exec(handle_, "BEGIN;", nullptr, nullptr, nullptr) != SQLITE_OK)
        throw db_err("BEGIN failed:\n" + string(sqlite3_errmsg(handle_)));

} // begin

void r_conn::commit() {

    if (sqlite3_exec(handle_, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK)
        throw db_err("COMMIT failed:\n" + string(sqlite3_errmsg(handle_)));

} // commit

void r_conn::rollback() {

    if (sqlite3_exec(handle_, "ROLLBACK;", nullptr, nullptr, nullptr) != SQLITE_OK)
        throw db_err("ROLLBACK failed:\n" + string(sqlite3_errmsg(handle_)));

} // rollback

sqlite3_stmt* r_conn::prepare(
    const string &sql, const vector<sql_var> &parms
)
{
    sqlite3_stmt *stmt = nullptr;
    if (
        sqlite3_prepare_v2(
            handle_, sql.c_str(), -1, &stmt, nullptr
        ) != SQLITE_OK 
    )
        throw db_err("PREPARE failed:\n" + string(sqlite3_errmsg(handle_)));
    
    // bind the actual values depending on type
    for (size_t i = 0; i < parms.size(); i++) {
        int idx = i + 1;
        visit([&](auto &&val) {
            using T = decay_t<decltype(val)>;
            if constexpr (is_same_v<T, monostate>)
                sqlite3_bind_null(stmt, idx);
            else if constexpr (is_same_v<T, int64_t>)
                sqlite3_bind_int64(stmt, idx, val);
            else if constexpr (is_same_v<T, double>)
                sqlite3_bind_double(stmt, idx, val);
            else if constexpr (is_same_v<T, string>)
                sqlite3_bind_text(stmt, idx, val.c_str(), -1, SQLITE_TRANSIENT);
        }, parms[i]);
    }

    return stmt;
} // prepare

optional<row> r_conn::query_one(
    const string &sql, const vector<sql_var> parms
) 
{
    vector<row> rows = query_all(sql, parms);
    if (rows.empty()) return std::nullopt;
    return rows.front();
} // query_one

vector<row> r_conn::query_all(
    const string &sql, const vector<sql_var> parms
)
{
    sqlite3_stmt *stmt = prepare(sql, parms);
    vector<row> rows;

    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        row cur;
        int col_cnt = sqlite3_column_count(stmt);
        for (int i = 0; i < col_cnt; i++) {
            string col_nm = sqlite3_column_name(stmt, i);
            switch (sqlite3_column_type(stmt, i)) {
                case SQLITE_INTEGER:
                    cur.cols[col_nm] = (int64_t) sqlite3_column_int64(stmt, i);
                    break;    
                case SQLITE_FLOAT:
                    cur.cols[col_nm] = (double) sqlite3_column_double(stmt, i);
                    break;
                case SQLITE_TEXT:
                    cur.cols[col_nm] = string (
                        reinterpret_cast<const char*>(sqlite3_column_text(stmt, i))
                    );
                    break;
                default:
                    cur.cols[col_nm] = monostate{};
            }
        }
        rows.push_back(cur);
    }
    return rows;
} // query_all

/******************************************************************************
**                         READ-WRITE DB CONNECTION                          **
******************************************************************************/

db_conn::db_conn(const string &path) {

    if (sqlite3_open(path.c_str(), &handle_) != SQLITE_OK)
        throw db_err("Failed to open db:\n" + string(sqlite3_errmsg(handle_)));
    
    sqlite3_extended_result_codes(handle_, 1);
    sqlite3_busy_timeout(handle_, 1000);
    sqlite3_exec(handle_, "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);
    sqlite3_exec(handle_, "PRAGMA foreign_keys=ON;", nullptr, nullptr, nullptr);

} // db_conn(const string&)

db_conn::~db_conn() { if (handle_) sqlite3_close(handle_); } // ~db_conn

void db_conn::execute(const string &sql, const vector<sql_var> &parms) {

    sqlite3_stmt *stmt = prepare(sql, parms);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        throw db_err("EXECUTE failed:\n" + string(sqlite3_errmsg(handle_)));
    }

} // execute

} // namespace mes