#pragma once

#include <string>
#include "row.h"
#include "transaction.h"

using std::string;

namespace mes {

class operation {
    public:
        operation() = default;
        operation(const row& r) {
            route_id  = r.get<string>("ROUTE_ID");
            oper_no   = r.get<string>("OPER_NO");
            recipe_id = r.get<string>("RECIPE_ID");
            oper_nm   = r.get<string>("OPER_NM");
            mandatory = r.get<int64_t>("MANDATORY");
            give_hold = r.get<int64_t>("GIVE_HOLD");    
        }

        string to_string() const {
            return format(
                "oper{{\n\troute_id:\t{}\n\toper_no:\t{}\n\trecipe_id:\t{}\n\t"
                "oper_nm:\t{}\n\tmandatory:\t{}\n\tgive_hold:\t{}\n}}",
                route_id, oper_no, recipe_id, oper_nm, mandatory, give_hold
            );
        }

        string  route_id;
        string  oper_no;
        string  recipe_id;
        string  oper_nm;
        int64_t mandatory;
        int64_t give_hold;

}; // operation

inline vector<operation> rtoo(const rows &r) {
    vector<operation> o;
    o.reserve(r.size());
    for (int i = 0; i < r.size(); ++i) o.push_back(operation(r.at(i)));

    return o;
} // rtoo

} // namespace mes

using oper  = mes::operation;
using opers = std::vector<oper>;
using mes::rtoo;