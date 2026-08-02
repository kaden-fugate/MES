#pragma once

#include "db_conn.h"

/******************************************************************************
*
* name:         transaction
* description:  this will be an abstract base class for each transaction that 
*               we have in this system. each transaction will have its own 
*               special pre-defined input/output types defined in their 
*               specific derivation of this class.
*
******************************************************************************/
template <typename conn_type, typename request, typename response>
class transcation {
    private:
        conn_type &conn_;
        virtual response exec(request) = 0;

    public:
        response run(request){
            conn_.begin();
            try {
                response res = exec(request);
                conn_.commit();
                return res;
            }
            catch (mes::db_err &e) {
                std::cout << e.what() << "\n";
                conn_.rollback();
                throw;
            }
        }
};

template <typename request, typename response>
using read_tx = transaction<r_conn, request, response>;

template <typename request, typename response>
using write_tx = transaction<r_conn, request, response>;