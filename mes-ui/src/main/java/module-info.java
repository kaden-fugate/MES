module dev.mes {
    requires javafx.controls;
    requires javafx.fxml;
    requires javafx.graphics;
    requires java.net.http;
    requires com.google.gson;

    exports dev.mes to javafx.graphics;
    opens dev.mes to javafx.graphics;
    opens dev.mes.models to com.google.gson;
}
