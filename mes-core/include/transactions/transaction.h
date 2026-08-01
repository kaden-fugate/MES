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
template <typename req, typename res>
class transcation {
    private:
        db_conn &conn_;

    public:
        res run(req){
            
        }
        virtual res exec(req) = 0;
};