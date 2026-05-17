package dev.mes;

import dev.mes.screens.HomeScreen;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

/**
 * JavaFX App
 */
public class App extends Application {

    @Override
    public void start(Stage stage) {
        ScreenManager manager = new ScreenManager(stage);
        manager.showScreen(new HomeScreen(manager).getView());

        stage.setTitle("MES Control");
        stage.show();
    }

    public static void main(String[] args) {
        launch();
    }

}