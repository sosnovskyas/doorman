#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define HANDSET_PIN 16  // D0
#define DOOR_PIN 5      // D1
#define RING_PIN 4     // D2

const char *ssid = "dd-wrt";
const char *password = "z8X26q19";

ESP8266WebServer server(80);

void handleRoot() {
    char temp[1400];

    snprintf(temp, 1400, "<!DOCTYPE html>\n"
                         "<html lang=\"en\">\n"
                         "<head>\n"
                         "    <meta charset=\"UTF-8\">\n"
                         "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
                         "    <title>Doorman</title>\n"
                         "    <style>\n"
                         "        body {\n"
                         "            display: flex;\n"
                         "\n"
                         "            flex-direction: column;\n"
                         "            align-items: center;\n"
                         "        }\n"
                         "\n"
                         "        .buttons {\n"
                         "            display: flex;\n"
                         "\n"
                         "            background-color: darkgray;\n"
                         "\n"
                         "            justify-content: space-between;\n"
                         "            align-items: center;\n"
                         "\n"
                         "            width: 150px;\n"
                         "            padding: 20px;\n"
                         "        }\n"
                         "\n"
                         "        button {\n"
                         "            background-color: cadetblue;\n"
                         "            height: 30px;\n"
                         "            width: 60px;\n"
                         "        }\n"
                         "    </style>\n"
                         "</head>\n"
                         "<body>\n"
                         "<button class=\"open-door-button\" style=\"margin-bottom: 80px\">open door</button>\n"
                         "\n"
                         "\n"
                         "<div class=\"buttons\">\n"
                         "    <button onclick=\"fetch('/up')\">up</button>\n"
                         "    <button onclick=\"fetch('/down')\">down</button>\n"
                         "</div>\n"
                         "\n"
                         "<div class=\"buttons\">\n"
                         "    <button onclick=\"fetch('/open')\">open</button>\n"
                         "    <button onclick=\"fetch('/close')\">close</button>\n"
                         "</div>\n"
                         "\n"
                         "<script>\n"
                         "    document\n"
                         "        .querySelector('.open-door-button')\n"
                         "        .addEventListener('click', (event) => {\n"
                         "            const target = event.target;\n"
                         "\n"
                         "            target.setAttribute('disabled', true);\n"
                         "            setTimeout(() => {target.removeAttribute('disabled')}, 6000);\n"
                         "\n"
                         "            fetch('/open-door');\n"
                         "        })\n"
                         "</script>\n"
                         "\n"
                         "</body>\n"
                         "</html>");
    server.send(200, "text/html", temp);
}

void handleNotFound() {
    server.send(404);
}

String getStatus() {
    return "{\
           HANDSET:" + String(digitalRead(HANDSET_PIN)) + ",\
           DOOR:" + String(digitalRead(DOOR_PIN)) + "\
                                                        }";
}

void up() {
    digitalWrite(HANDSET_PIN, HIGH);
    Serial.println("UP");
    server.send(200, "application/json", getStatus());
}

void down() {
    digitalWrite(HANDSET_PIN, LOW);
    Serial.println("DOWN");
    server.send(200, "application/json", getStatus());
}

void open() {
    digitalWrite(DOOR_PIN, HIGH);
    Serial.println("UP");
    server.send(200, "application/json", getStatus());
}

void close() {
    digitalWrite(DOOR_PIN, LOW);
    Serial.println("DOWN");
    server.send(200, "application/json", getStatus());
}

void status() {
    server.send(200, "application/json", getStatus());
}

void openDoor() {
    digitalWrite(HANDSET_PIN, HIGH);
    delay(1000);
    digitalWrite(DOOR_PIN, HIGH);
    delay(1000);
    digitalWrite(DOOR_PIN, LOW);
    delay(1000);
    digitalWrite(HANDSET_PIN, LOW);
    delay(1000);

    Serial.println("OPEN DOOR");
    server.send(200, "application/json", getStatus());
}


void setup(void) {
    pinMode(HANDSET_PIN, OUTPUT);
    pinMode(DOOR_PIN, OUTPUT);
    pinMode(RING_PIN, INPUT_PULLUP);

    digitalWrite(HANDSET_PIN, LOW);
    digitalWrite(DOOR_PIN, LOW);

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/up", up);
    server.on("/down", down);
    server.on("/open", open);
    server.on("/close", close);
    server.on("/status", status);
    server.on("/open-door", openDoor);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}

void loop(void) {
    server.handleClient();
}
