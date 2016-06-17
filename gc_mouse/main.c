#include "main.h"


#define TIME_INTERVAL 15    // milliseconds between controller check



int main() {
    
    DWORD gamepad_id = 4;
    printf("Searching for controller...\n");

    while (gamepad_id > 3) {
        gamepad_id = Controller_Connect();
    }

    printf("Got controller id %d\n", gamepad_id);

    // hide the console window
    ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
    
    if (gamepad_id == -1) {
        printf("Failed to find controller. (You should never see this)");
    }

    XINPUT_GAMEPAD gamepad;
    Mouse mouse;
    Mouse_Init(&mouse);
    while (true) {
        // get current state of controller
        int result = Controller_GetState(gamepad_id, &gamepad);

        if (result == 0) {
            // parse gamepad
            Mouse_ParseGamepad(&mouse, gamepad);

            // send input to mouse
            Mouse_SendInput(&mouse);

            // check controller for input every TIME_INTERVAL milliseconds
            Sleep(TIME_INTERVAL);
        }
    }

    return 0;
}