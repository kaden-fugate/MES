package dev.mes.screens;

import dev.mes.ScreenManager;
import dev.mes.api.MESClient;
import dev.mes.ui.FormField;
import dev.mes.data.UpsertEndpointInfo;

import com.google.gson.JsonObject;

import javafx.geometry.Insets;
import javafx.scene.Parent;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.application.Platform;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class UpsertScreen {
    private final ScreenManager   manager;
    private final String          title;
    private final List<FormField> fields;
    private final String          createEndpoint;
    private final String          updateEndpoint;
    private final JsonObject      existingData;                                // null if creating new

    // create new
    public UpsertScreen(ScreenManager manager, UpsertEndpointInfo info) {
        this.manager        = manager;
        this.title          = info.title;
        this.fields         = info.fields;
        this.createEndpoint = info.createEndpoint;
        this.updateEndpoint = info.updateEndpoint;
        this.existingData   = null;
    }

    // edit existing — pass the raw JsonObject of the row
    public UpsertScreen(ScreenManager manager, UpsertEndpointInfo info,
                        JsonObject existingData) {
        this.manager        = manager;
        this.title          = info.title;
        this.fields         = info.fields;
        this.createEndpoint = info.createEndpoint;
        this.updateEndpoint = info.updateEndpoint;
        this.existingData   = existingData;
    }

    public Parent getView() {
        Button backBtn = new Button("← Back");
        backBtn.setOnAction(e -> manager.showScreen(new DataOverviewScreen(manager).getView()));

        Label titleLabel = new Label(title);

        GridPane form = new GridPane();
        form.setHgap(10);
        form.setVgap(10);

        // map of key -> control so we can read values on submit
        Map<String, Control> controls = new LinkedHashMap<>();

        int row = 0;
        for (FormField field : fields) {
            form.add(new Label(field.getLabel()), 0, row);

            Control control;
            if (field.getType() == FormField.Type.DROPDOWN) {
                ComboBox<String> cb = new ComboBox<>();
                cb.getItems().addAll(field.getOptions());

                // pre-populate if editing
                if (existingData != null && existingData.has(field.getKey())) {
                    cb.setValue(existingData.get(field.getKey()).getAsString());
                } else {
                    cb.setValue(field.getOptions()[0]);
                }
                control = cb;
            } else {
                TextField tf = new TextField();

                // pre-populate if editing
                if (existingData != null && existingData.has(field.getKey())) {
                    tf.setText(existingData.get(field.getKey()).getAsString());
                }
                control = tf;
            }

            controls.put(field.getKey(), control);
            form.add(control, 1, row);
            row++;
        }

        Button submitBtn = new Button(existingData == null ? "Create" : "Update");
        submitBtn.setOnAction(e -> submit(controls));

        VBox layout = new VBox(15, backBtn, titleLabel, form, submitBtn);
        layout.setPadding(new Insets(20));
        return layout;
    }

    private void submit(Map<String, Control> controls) {
        // build JSON body from form values
        JsonObject body = new JsonObject();
        for (Map.Entry<String, Control> entry : controls.entrySet()) {
            String key = entry.getKey();
            Control control = entry.getValue();

            if (control instanceof TextField tf) {
                body.addProperty(key, tf.getText().trim());
            } else if (control instanceof ComboBox<?> cb) {
                body.addProperty(key, cb.getValue().toString());
            }
        }

        String endpoint = existingData == null ? createEndpoint : updateEndpoint;
        String json = body.toString();

        new Thread(() -> {
            String response = MESClient.post(endpoint, json);
            System.out.println("Submit response: " + response);
            Platform.runLater(() ->
                manager.showScreen(new HomeScreen(manager).getView()));
        }).start();
    }
}