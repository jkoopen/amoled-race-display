#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include <config.h>
#include <Esp.h>
#include <Renderer.hpp>
#include <Decoder.hpp>
#include <Data.hpp>

namespace Hardware {
    /**************************************************************************
     * @brief Initialize and setup all other hardware related functions.
     * @param baud Baud rate for the serial connection
     *************************************************************************/
    void init(uint32_t baud = 115200);

    /**************************************************************************
     * @brief Cycle the brightness of display when the button is pressed.
     * @param None
     *************************************************************************/
    void cycleBrightness();
}

#endif // HARDWARE_H