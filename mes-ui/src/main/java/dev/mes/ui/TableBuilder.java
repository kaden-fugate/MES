package dev.mes.ui;

import javafx.beans.property.SimpleStringProperty;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import java.util.Map;
import java.util.function.Function;

public class TableBuilder {

    public static <T> TableView<T> build(Map<String, Function<T, String>> columns) {
        TableView<T> table = new TableView<>();
        table.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);

        for (Map.Entry<String, Function<T, String>> entry : columns.entrySet()) {
            TableColumn<T, String> col = new TableColumn<>(entry.getKey());
            col.setCellValueFactory(data ->
                new SimpleStringProperty(entry.getValue().apply(data.getValue())));
            table.getColumns().add(col);
        }

        return table;
    }
}