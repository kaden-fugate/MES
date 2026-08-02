#pragma once

#include <string>

using std::string;

struct operation {
    string route_id;
    string oper_no;
    string recipe_id;
    string oper_nm;
    bool   mandatory;
    bool   give_hold;
};