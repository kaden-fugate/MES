#include "../include/db_conn.h"
#include <iostream>

using row        = mes::row;
using writer     = mes::db_conn;

int main() {

    int i;

    try {
        writer conn("/Users/kadenator/Desktop/Code/MES/Database/MESDB.db");

        // select test lot
        vector<row> rows = conn.query_all(
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
        conn.begin();
        conn.execute(
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
        rows = conn.query_all(
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

        conn.rollback();
        
        // check that rollback was successful
        rows = conn.query_all(
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