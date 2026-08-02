#pragma once

#include "db_conn.h"
#include <iostream>

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
class transaction {
    private:
        conn_type &conn_;
        virtual response exec(const request&) = 0;

    public:
        explicit transaction(conn_type &conn) : conn_(conn) {}
        virtual ~transaction() = default;

        response run(request req){
            conn_.begin();
            try {
                response res = exec(req);
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

// template for read transactions
template <typename request, typename response>
using read_tx = transaction<mes::r_conn, request, response>;

// template for write transactions
template <typename request, typename response>
using write_tx = transaction<mes::db_conn, request, response>;

// macro for inheriting the constructor in child classes
#define inherit_tx(alias, req, res) \
    using base = alias<req, res>;   \
    using base::base;