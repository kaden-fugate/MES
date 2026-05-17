package dev.mes.screens;

import dev.mes.ScreenManager;
import dev.mes.screens.LotsScreen;
import dev.mes.screens.ToolsScreen;
import dev.mes.screens.DataOverviewScreen;

import javafx.scene.Parent;
import javafx.scene.layout.VBox;
import javafx.scene.control.Button;
import javafx.geometry.Insets;

public class HomeScreen {
    private final ScreenManager manager;

    public HomeScreen(ScreenManager manager) {
        this.manager = manager;
    }

    public Parent getView() {
        Button lotsBtn    = new Button("Lots");
        Button toolsBtn   = new Button("Tools");
        Button recipesBtn = new Button("Recipes");
        Button dataViewBtn = new Button("Data Overview");

        lotsBtn.setOnAction(e -> 
            manager.showScreen(new LotsScreen(manager).getView()));
        toolsBtn.setOnAction(e -> 
            manager.showScreen(new ToolsScreen(manager).getView()));
        dataViewBtn.setOnAction(e -> 
            manager.showScreen(new DataOverviewScreen(manager).getView()));
        // recipesBtn.setOnAction(e -> 
        //     manager.showScreen(new RecipesScreen(manager).getView()));

        VBox layout = new VBox(15, lotsBtn, toolsBtn, recipesBtn, dataViewBtn);
        layout.setPadding(new Insets(30));
        return layout;
    }
}