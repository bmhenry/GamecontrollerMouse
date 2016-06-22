#pragma once

#include "controller.h"
#include "main.h"

#define MAX_PIXEL_TRAVEL 20         // max # of pixels travelled per time interval
#define SLOW_DIVIDER 3              // how much to slow travel speed in 'slow zone'
#define SCROLL_SPEED WHEEL_DELTA/4  // how fast to scroll with right stick
#define TIME_INTERVAL 15            // milliseconds between controller check
#define LONG_SLEEP_INTERVAL 150     // debounce time for dpad buttons


typedef struct Mouse {
    INPUT input;
    bool leftButton;
    bool rightButton;
    bool leftDpad;
    bool rightDpad;
    int sleepTime;
} Mouse;

// initialize Mouse struct
void Mouse_Init(Mouse*);

// parse buttons, analogs, etc.
void Mouse_ParseGamepad(Mouse*, XINPUT_GAMEPAD);

// send input
void Mouse_SendInput(Mouse*);