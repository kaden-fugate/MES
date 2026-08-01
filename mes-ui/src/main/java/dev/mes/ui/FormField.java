package dev.mes.ui;

public class FormField {
    public enum Type { TEXT, DROPDOWN }

    private final String label;
    private final String key;        // matches JSON key sent to server
    private final Type type;
    private final String[] options;  // only used for DROPDOWN

    // text field
    public FormField(String label, String key) {
        this.label = label;
        this.key = key;
        this.type = Type.TEXT;
        this.options = null;
    }

    // dropdown field
    public FormField(String label, String key, String... options) {
        this.label = label;
        this.key = key;
        this.type = Type.DROPDOWN;
        this.options = options;
    }

    public String getLabel()   { return label; }
    public String getKey()     { return key; }
    public Type getType()      { return type; }
    public String[] getOptions() { return options; }
}