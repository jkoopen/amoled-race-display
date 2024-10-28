#include <Decoder.hpp>
using namespace Decoder;

void Decoder::loop()
{
    static char message[PACKET_SIZE];
#if PACKET_SIZE <= 255
    static uint8_t messagePosition = 0; // Create a variable to store the message position, 8-bit for smaller packet sizes
#else
    static uint16_t messagePosition = 0; // Create a variable to store the message position, 16-bit for larger packet sizes
#endif

    while (Serial.available() > 0)
    {
        char incomingByte = Serial.read(); // Grab the incoming byte

        if (incomingByte != '\n' /* LINE FEED*/ && (messagePosition < PACKET_SIZE - 1))
        {
            message[messagePosition] = incomingByte; // Add the incoming byte to the message
            messagePosition++;                       // Increment the message position
        }
        else
        {
            message[messagePosition] = '\0'; // Null-terminate the string (empty charset)
            Decoder::decodeRawData(message); // Decode the message
            messagePosition = 0; // Reset the message position
            Serial.flush();      // Clear the serial buffer
        }
    }
}

void Decoder::decodeRawData(char *rawData)
{
    String packetData;
    char type = rawData[0]; // The first character of the message is the packet type
    // Strip the packet type from the message
    uint16_t messageLength = strlen(rawData);
    for (int i = 0; i < messageLength; i++)
    {
        rawData[i] = rawData[i + 1];
    }
    packetData = String(rawData); // Convert the message to a string
    // Switch statement to determine the packet type
    switch (type)
    {
    case 'T': // Telemetry packet
        Decoder::decodeTelemetry(packetData);
        break;
    case 'S': // Session data packet
        Decoder::decodeSessiondata(packetData);
        break;
    case 'C': // Car status packet
        Decoder::decodeCarstatus(packetData);
        break;
    default: // Unknown packet, do nothing
        break;
    }
}

// Telemetry data is a type of data which is sent 60 times per second. This data contains the speed, RPM, gear, ERS level, ERS deployment mode, rev lights percentage, lap time, DRS state, and fuel level.
void Decoder::decodeTelemetry(String contents)
{

    ArduinoJson::JsonDocument doc;
    ArduinoJson::DeserializationError err = ArduinoJson::deserializeJson(doc, contents);
    if (err)
    {
        return;
    }
    // All types of incoming data contain the current menu
    if (!doc["0"].isNull()) currentMenu = doc["0"].as<uint8_t>(); // {0} Current menu

    if (!doc["S"].isNull()) playerData.speed = doc["S"].as<uint16_t>(); // {S} Speed
    if (!doc["R"].isNull()) playerData.rpm = doc["R"].as<uint16_t>(); // {R} RPM
    if (!doc["G"].isNull()) playerData.gear[0] = doc["G"].as<uint8_t>(); // {G} Gear
    if (!doc["U"].isNull()) playerData.gear[1] = doc["U"].as<uint8_t>(); // {U} Suggested gear
    if (!doc["E"].isNull()) playerData.ers[0] = doc["E"].as<uint8_t>(); // {E} ERS level (0-100)
    if (!doc["F"].isNull()) playerData.ers[1] = doc["F"].as<uint8_t>(); // {F} ERS deployment mode
    if (!doc["T"].isNull()) playerData.paddles[0] = doc["T"].as<uint8_t>(); // {T} Throttle level (0-100)
    if (!doc["B"].isNull()) playerData.paddles[1] = doc["B"].as<uint8_t>(); // {B} Brake level (0-100)
    if (!doc["L"].isNull()) playerData.revLightsPercent = doc["L"].as<uint8_t>(); // {L} Rev lights percentage
    if (!doc["M"].isNull()) playerData.lapTime[0] = doc["M"].as<uint16_t>(); // {M} Lap time, minutes
    if (!doc["C"].isNull()) playerData.lapTime[1] = doc["C"].as<uint16_t>(); // {C} Lap time, seconds
    if (!doc["X"].isNull()) playerData.lapTime[2] = doc["X"].as<uint16_t>(); // {X} Lap time, milliseconds
    if (!doc["W"].isNull()) playerData.drsState = doc["W"].as<uint8_t>(); // {W} DRS state
}

// Session data is a type of data which is sent 2 times per second. This data contains the pit ideal lap number, safety car status, position, lap data, and pit status.
void Decoder::decodeSessiondata(String contents)
{
    ArduinoJson::JsonDocument doc;
    ArduinoJson::DeserializationError err = ArduinoJson::deserializeJson(doc, contents);
    if (err)
    {
        return;
    }
    
    if (!doc["I"].isNull()) sessionData.pitIdealLap = doc["I"].as<uint8_t>(); // {I} Pit ideal lap no
    if (!doc["S"].isNull()) sessionData.pitSpeedLimit = doc["S"].as<uint8_t>(); // {S} Pit speed limit
    if (!doc["A"].isNull()) sessionData.safetyCar = doc["A"].as<uint8_t>(); // {A} Safety car status
    if (!doc["V"].isNull()) playerData.position = doc["V"].as<uint8_t>(); // {V} Position
    if (!doc["D"].isNull()) playerData.lapData[0] = doc["D"].as<uint8_t>(); // {D} Lap data, current lap
    if (!doc["T"].isNull()) playerData.lapData[1] = doc["T"].as<uint8_t>(); // {T} Lap data, total laps
    if (!doc["P"].isNull()) playerData.pitStatus = doc["P"].as<uint8_t>(); // {P} Pit status
}

// Car status data is a type of data which is sent 2 times per second. This data contains the tyre wear, tyre damage, car damage, and all temperatures.
void Decoder::decodeCarstatus(String contents)
{
    ArduinoJson::JsonDocument doc;
    ArduinoJson::DeserializationError err = ArduinoJson::deserializeJson(doc, contents);
    if (err)
    {
        return;
    }

    if (!doc["F"].isNull()) carSetup.fuelMix = doc["F"].as<uint8_t>(); // {F} Fuel mix level (0 = lean 1 = standard 2 = rich)
    if (!doc["L"].isNull()) playerData.fuelLevel = doc["L"].as<uint8_t>(); // {T} Fuel level (laps remaining) (*10 (160 = 16.0 laps))
    if (!doc["B"].isNull()) carSetup.frontBrakeBias = doc["B"].as<uint8_t>(); // {B} Front brake bias (percentage) (0-100)
    if (!doc["D"].isNull()) carSetup.differential = doc["D"].as<uint8_t>(); // {D} Differential setup (percentage) (0-100)
}