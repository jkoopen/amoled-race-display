#ifndef DECODER_H
#define DECODER_H
#include <Data.h>
#include <ArduinoJson.h>
#include <cstdlib>
#include <cstring>

namespace Decoder {
  /**
   * The loop function will be used to decode the incoming packets.
   * @note This function should be called in the main loop.
   * @param params None
   */
  void loop();

  /**
   * Decode the raw data coming from the serial port.
   * @param rawData A pointer to the raw data
   */
  void decodeRawData(char *rawData);

  /**
   * Data is sent per type. This function will decode the data for the telemetry of the car (speed RPM etc.).
   * @param packetContents The packet containing the player data
   */
  void decodeTelemetry(String packetContents);

  /**
   * Data is sent per type. This function will decode the data for the session data (position etc.).
   * @param packetContents The packet containing the session data
   */
  void decodeSessiondata(String packetContents);

  /**
   * Data is sent per type. This function will decode the data for the car status (damage etc.).
   * @param packetContents The packet containing the event data
   */
  void decodeCarstatus(String packetContents);
}

#endif