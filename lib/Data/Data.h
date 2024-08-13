#ifndef DATA_H
#define DATA_H

#include <config.h>
#include <Esp.h>
#include <Arduino.h>
#include <cstdint>

#define PACKET_SIZE 255

#define DRS_NAVAILABLE 0
#define DRS_AVAILABLE 1
#define DRS_ENABLED 2

#define PIT_NONE 0
#define PIT_PITTING 1
#define PIT_IN_PIT_AREA 2

#define ERS_NONE 0
#define ERS_MEDIUM 1
#define ERS_HOTLAP 2
#define ERS_OVERTAKE 3

#define INVALID_PACKET 0
#define INVALID_PACKET_TYPE 1

#define SAFETY_CAR_NONE 0
#define SAFETY_CAR_FULL 1
#define SAFETY_CAR_VIRTUAL 2

#define MAIN_MFD 0
#define SETUP_MFD 1
#define DAMAGE_MFD 2
#define TEMP_MFD 3
#define WEAR_MFD 4

// Struct containing the incoming packet data
struct IncomingPacket
{
  uint64_t timestamp;
  char type;
  String contents;
};

// Struct containing the error data
struct Error
{
  /**
   * List of error codes:
   * 0: Recieved invalid packet
   * 1: Invalid packet type
   */ 
  uint64_t timestamp;
  uint8_t code = 255;
  String errMessage;
};

// Struct containing the player data
struct PlayerData
{
  uint16_t speed = 0;              // Variable containing the speed of the vehicle in km/h.
  uint16_t rpm = 0;                // Variable containing the RPM of the vehicle.
  uint8_t gear[2] = {1, 0};        // Variable containing the current gear and the suggested gear. First byte is the current gear, second byte is the suggested gear. 0 = R, 1 = N, 2 = 1, 3 = 2, 4 = 3, 5 = 4, 6 = 5, 7 = 6, 8 = 7, 9 = 8.
  uint8_t ers[2] = {0, 0};         // Variable containing the ERS level and the ERS deployment mode. First byte is the ERS level (0-100), second byte is the ERS deployment mode. 0 = none, 1 = medium, 2 = hotlap, 3 = overtake.
  uint8_t revLightsPercent = 0;    // Variable containing the rev lights percentage (0-100) of the vehicle.
  uint8_t position = 20;           // Variable containing the current position of the vehicle.
  uint8_t lapData[2] = {0, 0};     // Variable containing the current lap number and the total laps, respectively. First byte is the current lap, second byte is the total laps.
  uint16_t lapTime[3] = {0, 0, 0}; // Variable containing the current lap time in milliseconds. First byte is the minutes, second byte is the seconds, third byte is the milliseconds.
  uint8_t drsState = 0;            // Variable containing the DRS state of the vehicle. 0 = DRS not available, 1 = DRS available, 2 = DRS enabled.
  uint8_t pitStatus = 0;           // Variable containing the pit status of the vehicle. 0 = none, 1 = pitting, 2 = in pit area.
  uint8_t fuelLevel = 0;           // Variable containing the amount of laps of fuel remaining.
  uint8_t paddles[2] = {0, 0};     // Variable containing the state of the paddles. First byte is the amount of gas (0-100), second byte is the amount of brake (0-100).
};

// Not implemented yet
struct VehicleData
{
  float tyresWear[4];           // Tyre wear (percentage)
  uint8_t tyresDamage[4];       // Tyre damage (percentage)
  uint8_t brakesDamage[4];      // Brakes damage (percentage)
  uint8_t frontLeftWingDamage;  // Front left wing damage (percentage)
  uint8_t frontRightWingDamage; // Front right wing damage (percentage)
  uint8_t rearWingDamage;       // Rear wing damage (percentage)
  uint8_t floorDamage;          // Floor damage (percentage)
  uint8_t diffuserDamage;       // Diffuser damage (percentage)
  uint8_t sidepodDamage;        // Sidepod damage (percentage)
  bool drsFault;                // Indicator for DRS fault, 0 = OK, 1 = fault
  bool ersFault;                // Indicator for ERS fault, 0 = OK, 1 = fault
  uint8_t gearBoxDamage;        // Gear box damage (percentage)
  uint8_t engineDamage;         // Engine damage (percentage)
  uint8_t engineMGUHWear;       // Engine wear MGU-H (percentage)
  uint8_t engineESWear;         // Engine wear ES (percentage)
  uint8_t engineCEWear;         // Engine wear CE (percentage)
  uint8_t engineICEWear;        // Engine wear ICE (percentage)
  uint8_t engineMGUKWear;       // Engine wear MGU-K (percentage)
  uint8_t engineTCWear;         // Engine wear TC (percentage)
};

struct CarSetup
{
  uint8_t differential = 50; // Differential setup (percentage) (0 = open, 100 = locked)
  uint8_t frontBrakeBias = 50; // Front brake bias (percentage) (0 = rear, 100 = front)
  uint8_t fuelMix = 1; // Fuel mix (0 = lean, 1 = standard, 2 = rich)
};

struct SessionData
{
  uint8_t safetyCar = 0;     // Variable containing the safety car status. 0 = no safety car, 1 = full safety car, 2 = virtual safety car.
  uint8_t pitSpeedLimit = 0; // Variable containing the pit speed limit in km/h.
  uint8_t pitIdealLap = 0;   // Variable containing the ideal lap for pitting.
};

extern IncomingPacket incomingPacket;
extern Error errorData;
extern PlayerData playerData;
extern VehicleData vehicleData;
extern CarSetup carSetup;
extern SessionData sessionData;

extern uint8_t currentMenu;

#endif