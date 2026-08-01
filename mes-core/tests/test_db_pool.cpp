#include "../include/db_pool.h"
#include <iostream>

using mes::row;

int main() {

    int i;

    try {
        mes::db_pool<mes::db_conn> w_pool(
            "/Users/kadenator/Desktop/Code/MES/Database/MESDB.db", 1
        );
        mes::db_pool<mes::r_conn> r_pool(
            "/Users/kadenator/Desktop/Code/MES/Database/MESDB.db", 7
        );

        mes::db_pool<mes::db_conn>::pool_conn w_conn = w_pool.acquire();

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
        }

        w_conn->rollback();
        
        // check that rollback was successful
        rows = w_conn->query_all(
            "SELECT * FROM LOT WHERE LOT_ID = ?", {string("TEST_00")}
        );
        
        if (rows.size() > 0)
            printf("[AFTER ROLLBACK]:\tRollback didn't work?\n");
        else
            printf("[AFTER ROLLBACK]:\tRollback successful.\n");

    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}