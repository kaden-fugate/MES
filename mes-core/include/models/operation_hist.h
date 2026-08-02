#pragma once

#include <string>
#include "row.h"
#include "transaction.h"

using std::string;

namespace mes {

class operation_hist {
    public:
        operation_hist() = default;
        operation_hist(const row& r) {
            lot_id    = r.get<string>("LOT_ID");
            route_id  = r.get<string>("ROUTE_ID");
            oper_no   = r.get<string>("OPER_NO");
            timestamp = r.get<string>("TIMESTAMP");
            tx_id     = r.get<string>("TX_ID");
            comment   = r.get<string>("COMMENT");    
        }

        string to_string() const {
            return format(
                "operation_hist{{\n\tlot_id:\t{}\n\troute_id:\t{}\n\t"
                "oper_no:\t{}\n\ttimestamp:\t{}\n\ttx_id:\t{}\n\t"
                "comment:\t{}\n}}",
                lot_id, route_id, oper_no, timestamp, tx_id, comment
            );
        }

        string lot_id;
        string route_id;
        string oper_no;
        string timestamp;
        string tx_id;
        string comment;

}; // operation

inline vector<operation_hist> rtooh(const rows &r) {
    vector<operation_hist> oh;
    oh.reserve(r.size());
    for (int i = 0; i < r.size(); ++i) oh.push_back(operation_hist(r.at(i)));

    return oh;
} // rtoo

} // namespace mes

using oper_h  = mes::operation_hist;
using oper_hs = std::vector<oper_h>;
using mes::rtooh;