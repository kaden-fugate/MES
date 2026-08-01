package dev.mes.data;

import dev.mes.ui.FormField;

import java.util.HashMap;
import java.util.Map;
import java.util.List;

public class UpsertEndpointInfo {
    public List<FormField> fields;
    public String          title;
    public String          updateEndpoint;
    public String          createEndpoint;

    public UpsertEndpointInfo () {}
    public UpsertEndpointInfo (
        List<FormField> Fields,
        String          ScreenTitle,
        String          UpdateEndpoint,
        String          CreateEndpoint
    ) {
        this.fields         = Fields;
        this.title          = ScreenTitle;
        this.updateEndpoint = UpdateEndpoint;
        this.createEndpoint = CreateEndpoint;
    }

    public UpsertEndpointInfo get(String data) {
        UpsertEndpointList list = new UpsertEndpointList();
        return list.get(data);
    }

    private class UpsertEndpointList {
        
        private final Map<String, UpsertEndpointInfo> EndpointMap = Map.of(
            "Lot", new UpsertEndpointInfo(
                List.of(
                    new FormField("Lot ID",  "LOT_ID"),
                    new FormField("Route",   "ROUTE_ID"),
                    new FormField("Status",  "STATUS", "WAIT", "RUN", "HOLD", "COMPLETE")
                ),
                "Edit Lot", "/lots/create", "/lots/update"
            ),

            "Tool", new UpsertEndpointInfo(
                List.of(
                    new FormField("Tool ID",            "TOOL_ID"),
                    new FormField("Tool Name",          "TOOL_NM"),
                    new FormField("Work Area",          "WORK_AREA"),
                    new FormField("Status",             "UP", "DOWN", "LOST", "WAIT", "PM", "MFGH", "TEST"),
                    new FormField("Number of Chambers", "CHAMBER_CNT")
                ),
                "Edit Tool", "/tools/create", "/tools/update"
            )
        );
        
        public UpsertEndpointList () {}
        
        public UpsertEndpointInfo get(String data) {
            return EndpointMap.get(data);
        }
    }
}

