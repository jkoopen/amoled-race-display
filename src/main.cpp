#include "main.hpp"

F1WiFiUDP wifiUDP;
F1Parser parser(&wifiUDP);

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello World");
}

void loop()
{
    delay(1000);
    F1WiFiUDP::credentialsChecker(&wifiUDP); // Loop to check for credentials on the serial buffer
}