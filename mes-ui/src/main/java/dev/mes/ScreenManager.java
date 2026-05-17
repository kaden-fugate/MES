package dev.mes;

import javafx.stage.Stage;
import javafx.scene.Parent;
import javafx.scene.Scene;

public class ScreenManager {
    private final Stage stage;

    public ScreenManager(Stage stage) {
        this.stage = stage;
    }

    public void showScreen(Parent root) {
        Scene scene = new Scene(root, 1024, 768);
        stage.setScene(scene);
    }
}