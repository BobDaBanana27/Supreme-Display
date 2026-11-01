#include "sync.h"
#include "power.h"
#include "storage.h"
#include <WiFi.h>
#include <WebServer.h>

static WebServer server(80);

static void handle_root() {
  String html = "<!DOCTYPE html><html><head><title>Supreme Display</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:sans-serif;margin:30px;background:#fafafa;color:#222}";
  html += ".card{background:white;padding:20px;border-radius:8px;box-shadow:0 2px 8px rgba(0,0,0,0.1);max-width:500px}";
  html += "h1{margin-top:0}button{background:#0066cc;color:white;border:none;padding:10px 20px;border-radius:4px;cursor:pointer}</style></head>";
  html += "<body><div class='card'><h1>Supreme Display</h1><p>Status: <strong>Connected</strong></p>";
  html += "<p>Battery: " + String(power_battery_percent()) + "% (" + String(power_battery_voltage(), 2) + "V)</p>";
  html += "<p>Free SD: " + String(storage_get_free_kb() / 1024) + " MB</p>";
  html += "<a href='/api/status'><button>View System Telemetry</button></a></div></body></html>";
  server.send(200, "text/html", html);
}

static void handle_status() {
  String json = "{";
  json += "\"device\":\"Supreme-Display\",";
  json += "\"firmware\":\"1.0.0\",";
  json += "\"battery_percent\":" + String(power_battery_percent()) + ",";
  json += "\"battery_voltage\":" + String(power_battery_voltage(), 2) + ",";
  json += "\"sd_free_kb\":" + String(storage_get_free_kb()) + ",";
  json += "\"resolution\":\"1872x1404\",";
  json += "\"dpi\":226";
  json += "}";
  server.send(200, "application/json", json);
}

void sync_init() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("Supreme-Display-Sync", "hackclub2026");
  Serial.printf("[SYNC] SoftAP started: SSID: Supreme-Display-Sync, IP: %s\n", WiFi.softAPIP().toString().c_str());

  server.on("/", HTTP_GET, handle_root);
  server.on("/api/status", HTTP_GET, handle_status);
  server.begin();
}

void sync_task(void*) {
  for (;;) {
    server.handleClient();
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}
