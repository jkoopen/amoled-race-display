#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "rm67162.h"
#include "ani.h"
#include "Latin_Hiragana_24.h"
#include "NotoSansMonoSCB20.h"
#include "NotoSansBold36.h"

// Create HTTPClient instance
HTTPClient http;
DynamicJsonDocument doc(1048);
DynamicJsonDocument doc2(1048);

// Create TFT instances
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

// WiFi credentials
const char* ssid = "BLADE15";
const char* password = "arduino1234";

// HTTP response codes
int httpCode = 0;
int httpCode2 = 0;

// Pin definitions
#define up 21
#define down 0
#define led 38

// Variables for LED and brightness control
bool deb = 0;
bool deb2 = 0;
byte bright = 5;
byte brightness[7] = {100, 120, 140, 180, 200, 230, 254};
bool ledON = false;

// Timing variables
unsigned long currTime = 0;
long period = 100000;
unsigned long currTime2 = 0;
int period2 = 20000;

// Variables to store API responses
String local_ip = "";
String ip = "";
String ver = "";
String country = "";
String country_name = "";
String town = "";
String region = "";
String timezone = "";
String continent_code = "";
String provider = "";
String timeMy = "";
String dateMy = "";
String utcOff = "xxx";
String timeLink = "https://timeapi.io/api/Time/current/ip?ipAddress=";
String payload = "";
String payload2 = "";

int n = 0;
bool first = 0;
int fromTop = 38;

void getData() {
    if (WiFi.status() == WL_CONNECTED) { // Check if connected to WiFi
        http.begin("https://ipapi.co/json/"); // URL for IP geolocation
        httpCode = http.GET(); // Make the request
        if (httpCode > 0) {
            payload = http.getString();
            deserializeJson(doc, payload);
            const char* ii = doc["ip"];
            const char* ve = doc["version"];
            const char* cc = doc["country_code"];
            const char* cco = doc["continent_code"];
            const char* cn = doc["country_name"];
            const char* tz = doc["timezone"];
            const char* utc = doc["utc_offset"];
            const char* twn = doc["city"];
            const char* rgn = doc["region"];
            const char* pro = doc["org"];

            ip = String(ii);
            ver = String(ve);
            country = String(cc);
            country_name = String(cn);
            continent_code = String(cco);
            timezone = String(tz);
            town = String(twn);
            region = String(rgn);
            provider = String(pro);
        }
        http.end(); // Free the resources
    }
}

void getTime() {
    if (WiFi.status() == WL_CONNECTED) { // Check if connected to WiFi
        http.begin("https://timeapi.io/api/Time/current/ip?ipAddress=" + ip); // URL for time API
        httpCode2 = http.GET(); // Make the request
        if (httpCode2 > 0) {
            payload2 = http.getString();
            deserializeJson(doc2, payload2);
            const char* ti = doc2["time"];
            const char* dat = doc2["date"];

            timeMy = String(ti);
            dateMy = String(dat);
        }
        http.end(); // Free the resources
    }
}

void setup() {
    pinMode(up, INPUT_PULLUP);
    pinMode(down, INPUT_PULLUP);
    pinMode(led, OUTPUT);

    sprite.createSprite(240, 536); // Create sprite with dimensions
    sprite.setSwapBytes(1);

    rm67162_init(); // Initialize AMOLED LCD

    WiFi.begin(ssid, password); // Connect to WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    delay(1000);

    lcd_brightness(brightness[3]); // Set initial brightness
    local_ip = WiFi.localIP().toString(); // Store local IP address
}

void draw() {
    sprite.fillSprite(TFT_BLACK); // Clear the sprite with black color
    sprite.pushImage(150, 200 + fromTop, 102, 160, ani[n]); // Draw image on sprite

    sprite.setTextDatum(0);
    sprite.loadFont(NotoSansBold36);
    sprite.setTextColor(TFT_WHITE, TFT_BLACK);
    sprite.drawString("GeoLocation", 0, 4); // Draw header text
    sprite.unloadFont();

    // Draw lines for section dividers
    sprite.drawLine(0, 30 + fromTop, 240, 30 + fromTop, 0x8410);
    sprite.drawLine(0, 90 + fromTop, 240, 90 + fromTop, 0x8410);
    sprite.drawLine(0, 150 + fromTop, 240, 150 + fromTop, 0x8410);
    sprite.drawLine(0, 210 + fromTop, 180, 210 + fromTop, 0x8410);
    sprite.drawLine(0, 390 + fromTop, 240, 390 + fromTop, 0x8410);
    sprite.drawLine(0, 450 + fromTop, 240, 450 + fromTop, 0x8410);

    sprite.loadFont(NotoSansMonoSCB20);
    sprite.setTextColor(0x5E9E, TFT_BLACK);
    sprite.drawString("PUBLIC IP", 0, 10 + fromTop);
    sprite.drawString("COUNTRY", 0, 70 + fromTop);
    sprite.drawString("TIMEZONE", 0, 130 + fromTop);
    sprite.drawString("T0WN", 0, 190 + fromTop);
    sprite.drawString("REGION", 0, 370 + fromTop);
    sprite.drawString("PROVIDER", 0, 430 + fromTop);
    sprite.unloadFont();

    sprite.setTextDatum(2);
    sprite.drawString(ver, 240, 38 + fromTop);
    sprite.drawString(country, 240, 98 + fromTop);
    sprite.drawString(continent_code, 240, 158 + fromTop);

    sprite.unloadFont();

    sprite.setTextDatum(0);
    sprite.setTextColor(0xC618, TFT_BLACK);
    sprite.loadFont(Latin_Hiragana_24);

    sprite.drawString(ip, 0, 35 + fromTop);
    sprite.drawString(country_name, 0, 95 + fromTop);
    sprite.drawString(timezone, 0, 155 + fromTop);
    sprite.drawString(town, 0, 215 + fromTop);
    sprite.drawString(region, 0, 395 + fromTop);
    sprite.drawString(provider, 0, 455 + fromTop);
    sprite.unloadFont();

    sprite.setTextColor(0xEF5D, TFT_BLACK);
    sprite.drawString(dateMy, 0, 270 + fromTop, 4);
    sprite.drawString(timeMy, 0, 296 + fromTop, 7);
    sprite.drawString(String(httpCode2), 0, 520, 2);
    sprite.setTextDatum(2);
    sprite.drawString(local_ip, 240, 520, 2);

    lcd_PushColors(0, 0, 240, 536, (uint16_t*)sprite.getPointer()); // Push the sprite buffer to LCD
}

void readButtons() {
    if (digitalRead(up) == 0) {
        if (deb == 0) {
            deb = 1;
            bright++;
            if (bright == 7) bright = 0;
            lcd_brightness(brightness[bright]); // Adjust LCD brightness
        }
    } else {
        deb = 0;
    }

    if (digitalRead(down) == 0) {
        if (deb2 == 0) {
            deb2 = 1;
            ledON = !ledON;
            digitalWrite(led, ledON); // Toggle LED state
        }
    } else {
        deb2 = 0;
    }
}

void loop() {
    if (first == 0) {
        getData();
        getTime();
        first = 1;
    }
    readButtons();
    n++;
    if (n == 45) n = 0;

    if (millis() > currTime + period) {
        currTime = millis();
        getData(); // Update geolocation data
    }

    if (millis() > currTime2 + period2) {
        currTime2 = millis();
        getTime(); // Update time data
    }

    draw();
    delay(40); // Small delay to limit loop speed
}
