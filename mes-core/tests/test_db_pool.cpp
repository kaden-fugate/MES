#include "../include/db_pool.h"
#include <iostream>

using row        = mes::row;
using read_pool  = mes::db_pool<mes::r_conn>;
using reader     = mes::db_pool<mes::r_conn>::pool_conn;
using write_pool = mes::db_pool<mes::db_conn>;
using writer     = mes::db_pool<mes::db_conn>::pool_conn;

bool read_test() {

    int i;
    try {
        read_pool r_pool(
            "/Users/kadenator/Desktop/Code/MES/Database/MESDB.db", 7
        );

        reader r_conn = r_pool.acquire();

        // select test lot - this works just fine in read connections
        vector<row> rows = r_conn->query_all(
            "SELECT * FROM LOT"
        );
        
        // print output of query (For now, I don't expect this to have >1 lot)
        if (rows.size() > 0) {
            for (i = 0; i < rows.size(); i++) {
                printf(
                    "[QUERY ALL]:\t\tFound lot (%d): %s, %s, %s, %s\n", i,
                    rows[i].get<string>("LOT_ID").c_str(), rows[i].get<string>("ROUTE_ID").c_str(),
                    rows[i].get<string>("OPER_NO").c_str(),rows[i].get<string>("STATUS").c_str()
                );
            }
        }
        else {
            printf("[QUERY ALL]:\t\tTest lot(s) not found.\n");
        }
        
        // ILLEGAL:
        //      no begin, execute, or rollback allowed in read connection
        //
        // test rollback
        // r_conn->begin();
        // r_conn->execute(
        //      "INSERT INTO LOT ("
        //      "   LOT_ID, ROUTE_ID, OPER_NO, STATUS, PRIORITY, WAFER_CNT,"
        //      "   WAFER_SIZE, CREATE_DATE, LAST_OPER_DATE, COMPLETE_DATE"
        //      ") "
        //      "SELECT"
        //      "   ?, ROUTE_ID, OPER_NO, STATUS, PRIORITY, WAFER_CNT, WAFER_SIZE,"
        //      "   CREATE_DATE, LAST_OPER_DATE, COMPLETE_DATE "
        //      "FROM LOT "
        //      "WHERE LOT_ID = ?",
        //      {string("TEST_00"), string("TEST")}
        // );
        // r_conn->rollback();
        
        // all query functions are allowed in read connection
        optional<row> o_row = r_conn->query_one(
            "SELECT * FROM LOT WHERE LOT_ID = ?", {string("TEST")}
        );

        if (o_row) {
            row row = o_row.value();
            printf(
                "[QUERY ONE]:\t\tFound lot (%d): %s, %s, %s, %s\n", i,
                row.get<string>("LOT_ID").c_str(), row.get<string>("ROUTE_ID").c_str(),
                row.get<string>("OPER_NO").c_str(),row.get<string>("STATUS").c_str()
            );
        }
        else {
            printf("[QUERY ONE]:\t\tTest lot not found.\n");
        }  
        
        // we only have 7 read threads allocated to us and no way to unlock
        // them here so this will halt forever (no reader ever gets freed)
        // vector<reader> readers;
        // readers.reserve(7);
        // for (i = 0; i < 7; ++i) {
        //     readers.push_back(r_pool.acquire());
        // }

        return true;
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return false;
    }

}

// same as test_db_conn but with a pool instead
bool write_test() {

    int i;
    try {
        write_pool w_pool(
            "/Users/kadenator/Desktop/Code/MES/Database/MESDB.db", 1
        );

        writer w_conn = w_pool.acquire();

        // select test lot
        vector<row> rows = w_conn->query_all(
            "SELECT * FROM LOT"
        );
        
        // print output of query (For now, I don't expect this to have >1 lot)
        if (rows.size() > 0) {
            for (i = 0; i < rows.size(); i++) {
                printf(
                    "[BEFORE INSERT]:\tFound lot (%d): %s, %s, %s, %s\n", i,
                    rows[i].get<string>("LOT_ID").c_str(), rows[i].get<string>("ROUTE_ID").c_str(),
                    rows[i].get<string>("OPER_NO").c_str(),rows[i].get<string>("STATUS").c_str()
                );
            }
        }
        else {
            printf("[BEFORE INSERT]:\tTest lot(s) not found.\n");
            return false;
        }
        
        // test rollback
        w_conn->begin();
        w_conn->execute(
            "INSERT INTO LOT ("
            "   LOT_ID, ROUTE_ID, OPER_NO, STATUS, PRIORITY, WAFER_CNT,"
            "   WAFER_SIZE, CREATE_DATE, LAST_OPER_DATE, COMPLETE_DATE"
            ") "
            "SELECT"
            "   ?, ROUTE_ID, OPER_NO, STATUS, PRIORITY, WAFER_CNT, WAFER_SIZE,"
            "   CREATE_DATE, LAST_OPER_DATE, COMPLETE_DATE "
            "FROM LOT "
            "WHERE LOT_ID = ?",
            {string("TEST_00"), string("TEST")}
        );
        
        // check that lot is truly inserted with write connection
        rows = w_conn->query_all(
            "SELECT * FROM LOT WHERE LOT_ID = ?", {string("TEST_00")}
        );

        if (rows.size() > 0) {
            for (i = 0; i < rows.size(); i++) {
                printf(
                    "[BEFORE ROLLBACK]:\tFound lot (%d): %s, %s, %s, %s\n", i,
                    rows[i].get<string>("LOT_ID").c_str(), rows[i].get<string>("ROUTE_ID").c_str(),
                    rows[i].get<string>("OPER_NO").c_str(),rows[i].get<string>("STATUS").c_str()
                );
            }
        }
        else {
            printf("[BEFORE ROLLBACK]:\tTest lot(s) not found.\n");
            return false;
        }

        w_conn->rollback();
        
        // check that rollback was successful
        rows = w_conn->query_all(
            "SELECT * FROM LOT WHERE LOT_ID = ?", {string("TEST_00")}
        );
        
        if (rows.size() > 0) {
            printf("[AFTER ROLLBACK]:\tRollback didn't work?\n");
            return false;
        }
        else
            printf("[AFTER ROLLBACK]:\tRollback successful.\n");

        return true;
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return false;
    }

}

int main() {

    bool res = read_test();

    if (!res) { printf("[READ_TEST]:\t\tFail.\n"); }
    else { printf("[READ_TEST]:\t\tSuccess.\n"); }

    res = write_test();

    if (!res) { printf("[WRITE_TEST]:\t\tFail.\n"); }
    else { printf("[WRITE_TEST]:\t\tSuccess.\n"); }


    return 0;
}