/*---------------------------------------------------------------------------------------*\
*                                                                                         *
| transaction ID:   000                                                                   |
| transaction name: Query lot info                                                        |
| description:      000 will be used for querying a SINGLE lots information. we want to   |
|                   return the following to the user:                                     |                                                |
|                       - a list of all ancestors belonging to the lot                    |
|                       - a list of all descendants of the lot                            |
|                       - a list of the lots operation history                            |
| functions:        tx000::exec                                                           |
*                                                                                         *
\*---------------------------------------------------------------------------------------*/

#include "db_conn.h"
#include "tx000.h"

using tx000i_t = mes::tx000i_t;
using tx000o_t = mes::tx000o_t;
using tx000    = mes::tx000;

int    get_gen(const string&);
string like_family(const string&);

tx000o_t tx000::exec(const tx000i_t &input) {

    const string lot_id = input.lot_id;
    int i;

    // read lot info + verify lot existence
    string sql = "SELECT *    "
                 "FROM LOT    "
                 "WHERE       "
                 "  LOT_ID = ?";
    
    opt o_lot = conn_.query_one(sql, {lot_id});

    if (!o_lot) 
        throw no_data_err(format("[TX000]: No lot ({}) found.", lot_id));
    const lot l(o_lot.value());

    // find ancestors for lot:
    //    funny thing about lots, they will all be formatted like:
    //    1st char:   A-Z     (lot type)
    //    2nd char:   0-9     (lot sub-type)
    //    3/4 char:   A-Z     (lot location)
    //    5-9 char:   0-9     (lot ID)
    //    10th char:  .       (seperator between ID [aka family] and generation)
    //    11/12 char: 0-9/A-Z (number to denote generation in family)
    //
    // so all living ancestors will be in the LOT database AND will have a
    // lower generation number. the same thought follows for descendants
    //
    // start by finding generation number, follow by getting the lot formatted
    // like: _________.%, then query all lots older and younger
    const string sql_lt = "SELECT *                                                      "
                          "FROM LOT                                                      "
                          "WHERE                                                         "
                          "  LOT_ID LIKE ? AND                                           "
                          "  CAST(SUBSTR(LOT_ID, INSTR(LOT_ID, '.') + 1) AS INTEGER) < ?";
    const string sql_gt = "SELECT *                                                      "
                          "FROM LOT                                                      "
                          "WHERE                                                         "
                          "  LOT_ID LIKE ? AND                                           "
                          "  CAST(SUBSTR(LOT_ID, INSTR(LOT_ID, '.') + 1) AS INTEGER) > ?";
    
    string family     = like_family(lot_id);
    int    generation = get_gen(lot_id);

    printf(
        "[TX000]: family:\t\t%s\n\t generation:\t%d\n", 
        family.c_str(), generation
    );

    rows ancst_r = conn_.query_all(
        sql_lt, { family, std::to_string(generation)}
    );
    lots ancst_l = rtol(ancst_r);

    printf("[TX000]: Ancestors:\n");
    for(i = 0; i < ancst_l.size(); ++i)
        printf("\t%s\n\n", ancst_l.at(i).to_string().c_str());

    rows descendants = conn_.query_all(
        sql_gt,
        { family, std::to_string(generation) }
    );
    lots desc_l = rtol(descendants);

    printf("[TX000]: Descendants:\n");
    for(i = 0; i < desc_l.size(); ++i)
        printf("\t%s\n\n", desc_l.at(i).to_string().c_str());

    // now we just need to get the operation history
    sql = "SELECT *            "
          "FROM OPERATION_HIST "
          "WHERE               "
          "  LOT_ID = ?        ";
    rows    operhs_r = conn_.query_all(sql, {l.lot_id});
    oper_hs operhs_o = rtooh(operhs_r);

    printf("[TX000]: Operation History:\n");
    for(i = 0; i < operhs_o.size(); ++i)
        printf("\t%s\n\n", operhs_o.at(i).to_string().c_str());

    return {
        .l            = l,
        .ancestors    = ancst_l,
        .descendants  = desc_l,
        .oper_history = operhs_o
    };
} // exec

int get_gen(const string &lot_id) {
    size_t pos = lot_id.find('.');
    if (pos == std::string::npos) return -1;
    return std::stoi(lot_id.substr(pos + 1));
}

string like_family(const string &lot_id) {
    size_t pos    = lot_id.find('.');
    if (pos == std::string::npos) pos = lot_id.length();
    string family = lot_id.substr(0, pos + 1);
    return family + '%';
}