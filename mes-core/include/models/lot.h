#pragma once

#include <string>

using std::string;

struct lot {
    string lot_id;
    string route_id;
    int    oper_no;
    string status;
    int    priority;
    int    wafer_cnt;
    int    wafer_size;
    string create_date;
    string last_oper_date;
    string complete_date;
};