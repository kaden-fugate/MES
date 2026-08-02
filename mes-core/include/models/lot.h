#pragma once

#include <string>
#include "row.h"
#include "transaction.h"

using std::string;

namespace mes {
class lot {
    public:
        lot() = default;
        lot(const row& r) {
            lot_id         = r.get<string>("LOT_ID");
            route_id       = r.get<string>("ROUTE_ID");
            oper_no        = r.get<string>("OPER_NO");
            status         = r.get<string>("STATUS");
            priority       = r.get<int64_t>("PRIORITY");
            wafer_cnt      = r.get<int64_t>("WAFER_CNT");
            wafer_size     = r.get<string>("WAFER_SIZE");
            create_date    = r.get<string>("CREATE_DATE");
            last_oper_date = r.get<string>("LAST_OPER_DATE");
            complete_date  = r.get<string>("COMPLETE_DATE");
        } // lot(const row&)

        string to_string() const {
            return format(
                "lot{{\n\t\tlot_id:\t{}\n\troute_id:\t{}\n\toper_no:\t{}\n\t"
                "status:\t\t{}\n\tpriority:\t{}\n\twafer_cnt:\t{}\n\t"
                "wafer_size:\t{}\n\tcreate_date:\t{}\n\t"
                "last_oper_date:\t{}\n\tcomplete_date:\t{}\n}}",
                lot_id, route_id, oper_no, status, priority, wafer_cnt,
                wafer_size, create_date, last_oper_date, complete_date
            );
        }

        string  lot_id;
        string  route_id;
        string  oper_no;
        string  status;
        int64_t priority;
        int64_t wafer_cnt;
        string  wafer_size;
        string  create_date;
        string  last_oper_date;
        string  complete_date;
}; // lot

// translates to: rows --> lots
inline vector<lot> rtol(rows r) {
    vector<lot> l;
    l.reserve(r.size());
    for(int i = 0; i < r.size(); ++i) l.push_back( lot(r.at(i)) );

    return l;
} // rtol

} // namespace mes

using lot  = mes::lot;
using lots = std::vector<lot>;
using mes::rtol;