package dev.mes.screens;

import dev.mes.ScreenManager;
import dev.mes.screens.HomeScreen;
import dev.mes.models.Tool;

import javafx.scene.Parent;
import javafx.scene.layout.VBox;
import javafx.scene.control.Button;
import javafx.scene.control.TableView;
import javafx.scene.control.TableColumn;
import javafx.scene.control.Button;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.geometry.Insets;

public class ToolsScreen {
    private final ScreenManager manager;

    public ToolsScreen(ScreenManager manager) {
        this.manager = manager;
    }

    public Parent getView() {
        Button backBtn = new Button("← Back");
        backBtn.setOnAction(e -> 
            manager.showScreen(new HomeScreen(manager).getView()));

        TableView<Tool> table = new TableView<>();
        // add columns...

        Button completeBtn = new Button("Complete Operation");
        Button holdBtn     = new Button("Insert Hold");

        HBox buttons = new HBox(10, completeBtn, holdBtn);
        VBox layout  = new VBox(10, backBtn, table, buttons);
        layout.setPadding(new Insets(15));
        return layout;
    }
}