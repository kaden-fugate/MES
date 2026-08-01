package dev.mes.screens;

import dev.mes.ScreenManager;
import dev.mes.screens.HomeScreen;
import dev.mes.screens.UpsertScreen;

import dev.mes.models.Lot;
import dev.mes.models.Tool;

import dev.mes.api.MESClient;

import dev.mes.data.UpsertEndpointInfo;

import dev.mes.ui.TableBuilder;
import dev.mes.ui.FormField;

import javafx.application.Platform;
import javafx.beans.property.SimpleStringProperty;
import javafx.geometry.Insets;
import javafx.scene.Parent;
import javafx.scene.control.Button;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.ComboBox;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;

import java.util.HashMap;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

public class DataOverviewScreen {
    private final ScreenManager manager;
    private final Map<String, JsonObject> rawLotData = new HashMap<>();
    private UpsertEndpointInfo EPI = new UpsertEndpointInfo();


    // to be commented out
    private Gson gson = new Gson();

    private enum View {
        LOT, ROUTE, TOOL
    }
    private View view = View.LOT;

    public DataOverviewScreen(ScreenManager manager) {
        this.manager = manager;
    }

    public Parent getView() {
        Button backBtn = new Button("← Back");
        backBtn.setOnAction(e -> 
            manager.showScreen(new HomeScreen(manager).getView()));

        TableView table = new TableView();

        // Create the dropdown
        ComboBox<String> actionDropdown = new ComboBox<>();
        actionDropdown.getItems().addAll("View Lots", "View Routes", "View Tools");
        actionDropdown.setValue("View Lots"); // default selection

        // React to selection changes
        actionDropdown.setOnAction(e -> {
            String selected = actionDropdown.getValue();
            switch (selected) {
                case "View Lots":
                    view = View.LOT;
                    loadLots(table);
                    break;
                case "View Routes":
                    view = View.ROUTE;
                    break;
                case "View Tools":
                    view = View.TOOL;
                    loadTools(table);
                    break;
            }
        });

        loadLots(table);

        // make class EndpointInfo
        //   contains:
        //      list of form fields
        //      title
        //      create endpoint
        //      update endpoint       
        table.setOnMouseClicked(e -> {
            if (e.getClickCount() == 2) {
                Object selected = table.getSelectionModel().getSelectedItem();
                JsonObject json = gson.toJsonTree(selected).getAsJsonObject();

                switch (view) {
                    case LOT:
                        EPI = EPI.get("Lot");
                        break;
                    case ROUTE:
                        break;
                    case TOOL:
                        EPI = EPI.get("Tool");
                        break;
                }

                // edit existing — pass the JsonObject from the row
                manager.showScreen(new UpsertScreen(
                    manager, EPI, json
                ).getView());
            }
        });

        VBox layout  = new VBox(10, backBtn, table, actionDropdown);
        layout.setPadding(new Insets(15));
        return layout;
    }

    private <T> void setTable(
        String endpoint, String key, Class<T> type, TableView <T> table
    ) {
        MESClient.fetchArray(endpoint, key, type, items -> {
            table.getItems().clear();
            table.getItems().addAll(items);
        });
    }

    private void loadLots (TableView table) {
        TableView<Lot> typed = TableBuilder.build(Map.of(
            "Lot ID",              Lot::getLotID,
            "Route ID",            Lot::getRouteID,
            "Operation No",        Lot::getOperNo,
            "Status",              Lot::getStatus,
            "Priority",            Lot::getPriority,
            "Wafer Count",         Lot::getWaferCnt,
            "Wafer Size",          Lot::getWaferSize,
            "Create Date",         Lot::getCreateDate,
            "Last Operation Date", Lot::getLastOperDate,
            "Completion Date",     Lot::getCompleteDate
        ));

        table.getColumns().setAll(typed.getColumns());
        table.getItems().clear();

        setTable("/lots", "LOTS", Lot.class, table);
    }

    private void loadTools (TableView table) {
        TableView<Tool> typed = TableBuilder.build(Map.of(
            "Tool ID",             Tool::getToolID,
            "Tool Name",           Tool::getToolNM,
            "Work Area",           Tool::getWorkArea,
            "Status",              Tool::getStatus,
            "Last Operation Time", Tool::getOperTime,
            "Employee ID",         Tool::getEmplID,
            "Chamber Count",       Tool::getChamberCnt
        ));

        table.getColumns().setAll(typed.getColumns());
        table.getItems().clear();

        setTable("/tools", "TOOLS", Tool.class, table);
    }
}