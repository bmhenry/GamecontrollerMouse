#pragma once

#include <Windows.h>
#include <Xinput.h>

// define borders to dead zone
#define GAMEPAD_BUFFER 0.15
#define GAMEPAD_ANALOG_DEAD_LOW    SHRT_MIN*GAMEPAD_BUFFER
#define GAMEPAD_ANALOG_DEAD_HIGH   SHRT_MAX*GAMEPAD_BUFFER
// define borders to slow zone
#define GAMEPAD_ANALOG_SLOW_LOW    SHRT_MIN*5/8
#define GAMEPAD_ANALOG_SLOW_HIGH   SHRT_MAX*5/8


// attempts to connect to first available controller
// returns controller number
DWORD Controller_Connect();

// gets the current state of specified controller
int Controller_GetState(DWORD gamepad_id, XINPUT_GAMEPAD* gamepad);