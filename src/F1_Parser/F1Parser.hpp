#pragma once
#include "F1WiFiUDP.hpp"
#include "F1Structs.hpp"

class F1Parser; // Forward declaration

struct PacketData
{
    PacketMotionData motionData;
    PacketSessionData sessionData;
    PacketLapData lapData;
    PacketEventData eventData;
    PacketParticipantsData participantsData;
    PacketCarSetupData carSetupData;
    PacketCarTelemetryData telemetryData;
    PacketCarStatusData statusData;
};

struct parseTaskArgs
{
    F1WiFiUDP *udp;
    F1Parser *parser;
};

class F1Parser
{
public:
    F1Parser(F1WiFiUDP *udpPtr);
    ~F1Parser();

    void start();

    static void parseTask(parseTaskArgs *args);
    TaskHandle_t parseTaskHandle;
    volatile bool parseTaskRunning;

    PacketData packetData; // Contains the most recent packet data
    SemaphoreHandle_t packetMutex; // Mutex for packetData

private:
    F1WiFiUDP *udp;
    void parseMotionPacket(F1Parser *parser);
    void parseSessionPacket(F1Parser *parser);
    void parseLapPacket(F1Parser *parser);
    void parseEventPacket(F1Parser *parser);
    void parseParticipantsPacket(F1Parser *parser);
    void parseCarSetupPacket(F1Parser *parser);
    void parseTelemetryPacket(F1Parser *parser);
    void parseStatusPacket(F1Parser *parser);
};