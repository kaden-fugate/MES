#pragma once

#include <variant>
#include <string>
#include <unordered_map>
#include <optional>
#include <vector>

using sql_var = std::variant<std::monostate, int64_t, double, std::string>;

namespace mes {

struct row {
    std::unordered_map<std::string, sql_var> cols;
    template <typename T>
    T get(const std::string &name) const {
        return std::get<T>(cols.at(name));
    }

}; // row

}

using row  = mes::row;
using rows = std::vector<row>;
using opt  = std::optional<mes::row>;