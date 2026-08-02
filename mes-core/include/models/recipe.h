#pragma once

#include <string>

using std::string;

struct recipe {
    string recipe_id;
    string recipe_nm;
    string process_type;
    int    target_layer;
    int    wafer_size;
    string timestamp;
};