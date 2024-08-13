const { F122UDP } = require("f1-22-udp");
const { SerialPort } = require('serialport');

const port = new SerialPort({ path: 'COM6', baudRate: 115200 });

const f122 = new F122UDP();
f122.start();

let playerCarIndex = undefined;
let TelemetryData = undefined;
let openMFD = undefined;
let suggestedGear = undefined;
let carStatusData = undefined;
let lapData = undefined;

function map(x, A, B, C, D) {
    return Math.round(C + ((x - A) * (D - C)) / (B - A));
}

// Assuming 'participants' event provides player's car index
f122.on('participants', async function (data) {
    playerCarIndex = data.m_header.m_playerCarIndex;
});

f122.on('carTelemetry', async function (data) {
    TelemetryData = data.m_carTelemetryData[playerCarIndex];
    if (data.m_mfdPanelIndex === 255) {
        openMFD = 0; // Ensure value is 1 digit, saving space
    } else {
    openMFD = data.m_mfdPanelIndex + 1;
    }
    suggestedGear = data.m_suggestedGear;

    if (playerCarIndex !== undefined && TelemetryData !== undefined && openMFD !== undefined && suggestedGear !== undefined && carStatusData !== undefined && lapData !== undefined)
    {
    sendTelemetry();
    }
});

f122.on('lapData', async function (data) {
    lapData = data.m_lapData[playerCarIndex];
});

f122.on('session', async function (data) {
    sessionData = data;

    if (playerCarIndex !== undefined && TelemetryData !== undefined && openMFD !== undefined && suggestedGear !== undefined && carStatusData !== undefined && lapData !== undefined)
    {
    sendSessiondata();
    }
});

f122.on('carStatus', async function (data) {
    carStatusData = data.m_carStatusData[playerCarIndex];

    if (playerCarIndex !== undefined && TelemetryData !== undefined && openMFD !== undefined && suggestedGear !== undefined && carStatusData !== undefined && lapData !== undefined)
    {
    sendCarStatusData();
    }
});

async function sendTelemetry() {
    const lapTime = lapData.m_currentLapTimeInMS;
    const minutes = Math.floor(lapTime / 60000);
    const seconds = Math.floor((lapTime % 60000) / 1000);
    const milliseconds = lapTime % 1000;

    const ersPercentage = map(carStatusData.m_ersStoreEnergy, 0, 4000000, 0, 100);

    let drsStatus;
    if (TelemetryData.m_drs === 1) {
        drsStatus = 2;
    } else if (carStatusData.m_drsAllowed === 1 && TelemetryData.m_drs === 0) {
        drsStatus = 1;
    } else {
        drsStatus = 0;
    }
    // Now drsStatus can be used outside the if-else blocks

    const data = {
        "0": openMFD, // Represents the open MFD panel (0 = closed, 1 = car setup, 2 = pits, 3 = damage, 4 = engine, 5 = temperature)
        "S": TelemetryData.m_speed, // Represents speed in km/h
        "R": TelemetryData.m_engineRPM, // Represents engine RPM
        "G": TelemetryData.m_gear + 1, // Represents gear (0 = R, 1 = N, 2 = first...)
        "U": suggestedGear, // Represents suggested gear (0 = None, 1 = first...)
        "E": ersPercentage, // Represents ERS percentage available
        "F": carStatusData.m_ersDeployMode, // Represents ERS deployment mode (0 = None, 1 = Medium, 2 = Hotlap, 3 = Overtake)
        "T": Math.round(TelemetryData.m_throttle * 100), // Represents throttle level (0-100)
        "B": Math.round(TelemetryData.m_brake * 100), // Represents brake level (0-100)
        "L": TelemetryData.m_revLightsPercent, // Represents rev lights indicator
        "M": minutes, // Represents minutes of current lap time
        "C": seconds, // Represents seconds of current lap time
        "X": milliseconds, // Represents milliseconds of current lap time
        "W": drsStatus, // Represents DRS status (0 = Not allowed, 1 = Allowed, 2 = Activated)
        "Y": Math.floor(carStatusData.m_fuelRemainingLaps) // Represents fuel level percentage (0-100)
    }

    const message = 'T' + JSON.stringify(data); // Ensure newline is part of the message
    port.write(message + '\n', 'utf8', function (err) { // Specify UTF-8 encoding
        if (err) {
            return console.log('Error on write: ', err.message);
        }});
}

async function sendSessiondata() {
    const data = {
        "I": sessionData.m_pitStopWindowIdealLap, // Represents ideal lap for pit stop window
        "A": sessionData.m_safetyCarStatus, // Represents safety car status (0 = None, 1 = Full, 2 = Virtual)
        "V": lapData.m_carPosition, // Represents car position
        "D": lapData.m_currentLapNum, // Represents current lap number
        "T": lapData.m_totalLaps, // Represents total laps
        "P": lapData.m_pitStatus, // Represents pit status (0 = None, 1 = Pitting, 2 = In pit area)
    }

    const message = 'S' + JSON.stringify(data); // Ensure newline is part of the message
    port.write(message + '\n', 'utf8', function (err) { // Specify UTF-8 encoding
        if (err) {
            return console.log('Error on write: ', err.message);
        }});
}

async function sendCarStatusData() {
    const data = {
        "F": carStatusData.m_fuelMix, // Represents fuel mix (0 = Lean, 1 = Standard, 2 = Rich)
        "B": carStatusData.m_brakeBias, // Represents brake bias (0-100)
        "D": 50
    }

    const message = 'C' + JSON.stringify(data); // Ensure newline is part of the message
    port.write(message + '\n', 'utf8', function (err) { // Specify UTF-8 encoding
        if (err) {
            return console.log('Error on write: ', err.message);
        }});
}


async function init() {
    console.log('Waiting for player car index...');
    await waitForPlayerCarIndex(); // This function should resolve once playerCarIndex is updated
    setInterval(x, 16.666666666666667);
    console.log('Data transmission started');
}

// Example implementation of waitForPlayerCarIndex using a Promise
function waitForPlayerCarIndex() {
    return new Promise((resolve) => {
        // Replace this with the actual logic to wait for the 'participants' event
        // and then set playerCarIndex. This is just a placeholder example.
        const checkInterval = setInterval(() => {
            if (playerCarIndex !== undefined) {
                clearInterval(checkInterval);
                resolve();
            }
        }, 100); // Check every 100 ms
    });
}
