/******************************************************************************
*
* transaction ID:   000
* transaction name: Query lot info
* description:      000 will be used for querying lot information. 
*                   we have in this system. each transaction will have its own 
*                   special pre-defined input/output types defined in their 
*                   specific derivation of this class.
*
******************************************************************************/

#include "db_conn.h"
#include "tx000.h"

using tx000i_t = mes::tx000i_t;
using tx000o_t = mes::tx000o_t;
using tx000    = mes::tx000;

tx000o_t tx000::exec(const tx000i_t &input) {}
