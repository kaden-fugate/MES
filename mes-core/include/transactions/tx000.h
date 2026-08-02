#pragma once

#include "transaction.h" 
#include "lot.h"
#include "operation.h"

using std::string;

namespace mes {

    struct tx000i_t {
        string tx_id;
        string lot_id;
    }; // tx000i_t

    struct tx000o_t {
        int               rc;
        vector<lot>       ancestors;
        vector<lot>       descendants;
        vector<operation> oper_history; 
    }; // tx000o_t

    class tx000 : public read_tx<tx000i_t, tx000o_t> {
        private:
            inherit_tx(read_tx, tx000i_t, tx000o_t)
            tx000o_t exec(const tx000i_t&) override;
    }; // tx000

}; // namespace mes