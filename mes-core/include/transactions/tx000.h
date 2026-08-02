#pragma once

#include <string>

#include "transaction.h" 
#include "lot.h"
#include "operation_hist.h"

using std::string;

namespace mes {

    struct tx000i_t {
        string lot_id;
    }; // tx000i_t

    struct tx000o_t {
        lot     l;
        lots    ancestors;
        lots    descendants;
        oper_hs oper_history; 
    }; // tx000o_t

    class tx000 : public read_tx<tx000i_t, tx000o_t> {
        private:
            inherit_tx(read_tx, tx000i_t, tx000o_t)
            tx000o_t exec(const tx000i_t&) override;
    }; // tx000

}; // namespace mes

using tx000i_t = mes::tx000i_t;
using tx000o_t = mes::tx000o_t;
using tx000    = mes::tx000;