package dev.mes.api;

import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.net.URI;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

import javafx.application.Platform;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;


public class MESClient {
    private static final String BASE_URL = "http://localhost:8888";
    private static final HttpClient client = HttpClient.newHttpClient();

    // GET request
    public static String get(String endpoint) {
        try {
            HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(BASE_URL + endpoint))
                .GET()
                .build();

            HttpResponse<String> response = client.send(
                request,
                HttpResponse.BodyHandlers.ofString()
            );

            return response.body();  // returns JSON string
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    // POST request
    public static String post(String endpoint, String jsonBody) {
        try {
            HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(BASE_URL + endpoint))
                .POST(HttpRequest.BodyPublishers.ofString(jsonBody))
                .header("Content-Type", "application/json")
                .build();

            HttpResponse<String> response = client.send(
                request,
                HttpResponse.BodyHandlers.ofString()
            );

            return response.body();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public static <T> void fetchArray (
        String endpoint, String key, Class<T> type, Consumer<List<T>> onResult
    ) {
        // run HTTP call in background
        new Thread(() -> {
            String response = get(endpoint);

            // parse JSON into a list of Lot objects
            Gson gson = new Gson();
            JsonObject json = gson.fromJson(response, JsonObject.class);
            JsonArray array = json.getAsJsonArray(key);

            if (array == null) return;

            List<T> items = new ArrayList<>();
            for (JsonElement el : array) {
                items.add(gson.fromJson(el, type));
            }

            Platform.runLater(() -> {
                onResult.accept(items);
            });
        }).start();
    }
}