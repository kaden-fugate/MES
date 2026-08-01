#include "crow_all.h"
#include "Database.h"

int main() {
    crow::SimpleApp app;
    Database db("/Users/kadenator/Desktop/Code/MES/Database/MESDB.db");

    // test endpoint
    CROW_ROUTE(app, "/ping")
    ([]() {
        return crow::response(200, "pong");
    });

    // GET all lots
    CROW_ROUTE(app, "/lots")
    ([&db]() {
        auto rows = db.query("SELECT * FROM LOT", {});

        crow::json::wvalue result;
        std::vector<crow::json::wvalue> lots;

        for (auto& row : rows) {
            crow::json::wvalue lot;
            lot["LOT_ID"]         = row["LOT_ID"];
            lot["ROUTE_ID"]       = row["ROUTE_ID"];
            lot["OPER_NO"]        = row["OPER_NO"];
            lot["STATUS"]         = row["STATUS"];
            lot["PRIORITY"]       = row["PRIORITY"];
            lot["WAFER_CNT"]      = row["WAFER_CNT"];
            lot["WAFER_SIZE"]     = row["WAFER_SIZE"];
            lot["CREATE_DATE"]    = row["CREATE_DATE"];
            lot["LAST_OPER_DATE"] = row["LAST_OPER_DATE"];
            lot["COMPLETE_DATE"]  = row["COMPLETE_DATE"];
            lots.push_back(std::move(lot));
        }

        result["LOTS"] = std::move(lots);
        return crow::response(result);
    });

    CROW_ROUTE(app, "/tools")
    ([&db]() {

        auto rows = db.query("SELECT * FROM TOOL", {});

        crow::json::wvalue result;
        std::vector<crow::json::wvalue> tools;

        for (auto& row : rows) {
            crow::json::wvalue tool;
            tool["TOOL_ID"]      = row["TOOL_ID"];
            tool["TOOL_NM"]      = row["TOOL_NM"];
            tool["WORK_AREA"]    = row["WORK_AREA"];
            tool["STATUS"]       = row["STATUS"];
            tool["OPER_TIME"]    = row["OPER_TIME"];
            tool["EMPL_ID"]      = row["EMPL_ID"];
            tool["CHAMBER_CNT"]  = row["CHAMBER_CNT"];
            tools.push_back(std::move(tool));
        }

        result["TOOLS"] = std::move(tools);
        return crow::response(result);
    });

    // POST complete operation
    CROW_ROUTE(app, "/lot/complete-operation").methods("POST"_method)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "invalid json");
        }

        std::string lot_id = body["lot_id"].s();

        crow::json::wvalue result;
        result["status"] = "ok";
        result["lot_id"] = lot_id;
        return crow::response(result);
    });

    app.port(8888).multithreaded().run();
}