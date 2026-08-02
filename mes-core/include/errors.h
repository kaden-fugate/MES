#pragma once

#include <stdexcept>
#include <string>
#include <format>

namespace mes {

using std::string;
using std::runtime_error;

class tx_err : public runtime_error {
    public:
        explicit tx_err(const string &msg) : runtime_error(msg) {}
};

class invalid_err : public tx_err {
    public:
        using tx_err::tx_err;
};

class no_data_err : public tx_err {
    public:
        using tx_err::tx_err;
};

class db_err : public tx_err {
    public:
        using tx_err::tx_err;
};

}