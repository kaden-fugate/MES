package dev.mes.screens;

import dev.mes.ScreenManager;
import dev.mes.screens.HomeScreen;

import dev.mes.models.Lot;

import javafx.scene.Parent;
import javafx.scene.layout.VBox;
import javafx.scene.control.Button;
import javafx.scene.control.TableView;
import javafx.scene.control.TableColumn;
import javafx.scene.control.Button;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.geometry.Insets;

public class DataOverviewScreen {
    private final ScreenManager manager;

    public DataOverviewScreen(ScreenManager manager) {
        this.manager = manager;
    }

    public Parent getView() {
        Button backBtn = new Button("← Back");
        backBtn.setOnAction(e -> 
            manager.showScreen(new HomeScreen(manager).getView()));

        TableView<Lot> table = new TableView<>();

        VBox layout  = new VBox(10, backBtn, table);
        layout.setPadding(new Insets(15));
        return layout;
    }
}