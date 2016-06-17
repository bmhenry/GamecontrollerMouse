#include "controller.h"
#include <stdio.h>


DWORD Controller_Connect() {
    DWORD dwResult;

    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        // get the state of the controller from XInput
        dwResult = XInputGetState(i, &state);

        if (dwResult == ERROR_SUCCESS) {
            // controller is connected
            return i;
        }
    }

    return -1;
}

int Controller_GetState(DWORD gamepad_id, XINPUT_GAMEPAD* gamepad) {
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    
    // get controller state
    DWORD result = XInputGetState(gamepad_id, &state);

    if (result == ERROR_SUCCESS) {
        *gamepad = state.Gamepad;

        return 0;
    }

    return 1;
}