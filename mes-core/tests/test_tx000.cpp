#include "tx000.h"
#include "db_pool.h"

using read_pool  = mes::db_pool<mes::r_conn>;
using reader     = mes::db_pool<mes::r_conn>::pool_conn;

int main() {

    read_pool pool("/Users/kadenator/Desktop/Code/MES/Database/MESDB.db", 1);
    reader    conn  = pool.acquire();
    tx000i_t  input = { .lot_id = "TEST" };

    tx000 tx(*conn);
    tx000o_t output = tx.run(input);

    printf("[TX000O_T] LOT:\n\n");
    printf("%s\n", output.l.to_string().c_str());

    printf("[TX000O_T] ANCESTORS:\n\n");
    for (int i = 0; i < output.ancestors.size(); ++i) {
        printf("%s\n", output.ancestors.at(i).to_string().c_str());
        printf("\n");
    }

    printf("[TX000O_T] DESCENDANTS:\n\n");
    for (int i = 0; i < output.descendants.size(); ++i) {
        printf("%s\n", output.descendants.at(i).to_string().c_str());
        printf("\n");
    }

    printf("[TX000O_T] OPERATION HISTORY:\n\n");
    for (int i = 0; i < output.oper_history.size(); ++i) {
        printf("%s\n", output.oper_history.at(i).to_string().c_str());
        printf("\n");
    }

    return 0;
}