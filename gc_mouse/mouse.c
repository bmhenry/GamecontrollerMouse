#include "mouse.h"



void Mouse_Init(Mouse* this) {
    this->leftButton = false;
    this->rightButton = false;
    this->sleepTime = TIME_INTERVAL;
}


void Mouse_ParseGamepad(Mouse* this, XINPUT_GAMEPAD gamepad) {
    // get left thumbstick position
    int x, y;
    x = gamepad.sThumbLX;
    y = gamepad.sThumbLY;
    this->sleepTime = TIME_INTERVAL;

    // create INPUT struct to send mouse input
    INPUT input;
    ZeroMemory(&input, sizeof(input));

    input.type = 0; // input is a mouse

    // Left thumbstick is the mouse control
    // check if it's in the deadzone
    if (x < GAMEPAD_ANALOG_DEAD_LOW) {
        // use 'fast' speed
        if (x < GAMEPAD_ANALOG_SLOW_LOW) {
            input.mi.dx = -MAX_PIXEL_TRAVEL * x / SHRT_MIN;
        }
        // use 'slow' speed
        else {
            input.mi.dx = -MAX_PIXEL_TRAVEL / SLOW_DIVIDER * x / SHRT_MIN;
        }
        input.mi.dwFlags |= MOUSEEVENTF_MOVE;
    }
    else if (x > GAMEPAD_ANALOG_DEAD_HIGH) {
        if (x > GAMEPAD_ANALOG_SLOW_HIGH) {
            input.mi.dx = MAX_PIXEL_TRAVEL * x / SHRT_MAX;
        }
        else {
            input.mi.dx = MAX_PIXEL_TRAVEL / SLOW_DIVIDER * x / SHRT_MAX;
        }
        input.mi.dwFlags |= MOUSEEVENTF_MOVE;
    }

    if (y < GAMEPAD_ANALOG_DEAD_LOW) {
        if (y < GAMEPAD_ANALOG_SLOW_LOW) {
            input.mi.dy = MAX_PIXEL_TRAVEL * y / SHRT_MIN;
        }
        else {
            input.mi.dy = MAX_PIXEL_TRAVEL / SLOW_DIVIDER * y / SHRT_MIN;
        }
        input.mi.dwFlags |= MOUSEEVENTF_MOVE;
    }
    else if (y > GAMEPAD_ANALOG_DEAD_HIGH) {
        if (y > GAMEPAD_ANALOG_SLOW_HIGH) {
            input.mi.dy = -MAX_PIXEL_TRAVEL * y / SHRT_MAX;
        }
        else {
            input.mi.dy = -MAX_PIXEL_TRAVEL / SLOW_DIVIDER * y / SHRT_MAX;
        }
        input.mi.dwFlags |= MOUSEEVENTF_MOVE;
    }

    // Right thumbstick acts as the wheel (scroll, typically)
    // only scrolls one way at a time
    x = gamepad.sThumbRX;
    y = gamepad.sThumbRY;

    // vertical
    if (y < GAMEPAD_ANALOG_DEAD_LOW) {
        input.mi.mouseData = -SCROLL_SPEED * y/SHRT_MIN;
        input.mi.dwFlags |= MOUSEEVENTF_WHEEL;
    }
    else if (y > GAMEPAD_ANALOG_DEAD_HIGH) {
        input.mi.mouseData = SCROLL_SPEED * y / SHRT_MAX;
        input.mi.dwFlags |= MOUSEEVENTF_WHEEL;
    }
    // horizontal
    else if (x < GAMEPAD_ANALOG_DEAD_LOW) {
        input.mi.mouseData = -SCROLL_SPEED * x / SHRT_MIN;
        input.mi.dwFlags |= MOUSEEVENTF_HWHEEL;
    }
    else if (x > GAMEPAD_ANALOG_DEAD_HIGH) {
        input.mi.mouseData = SCROLL_SPEED * x/SHRT_MAX;
        input.mi.dwFlags |= MOUSEEVENTF_HWHEEL;
    }

    // check DPad buttons
    bool leftDPadPressed =  (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) > 0;
    bool rightDPadPressed = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) > 0;
    bool upDPadPressed =    (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) > 0;
    bool downDPadPressed =  (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) > 0;

    // dpad scroll (only 1 unit at a time, mostly for Reddit image scrolling)
    if (upDPadPressed) {
        input.mi.mouseData = 1;
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        this->sleepTime = LONG_SLEEP_INTERVAL;
    }
    else if (downDPadPressed) {
        input.mi.mouseData = -1;
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        this->sleepTime = LONG_SLEEP_INTERVAL;
    }

    if (leftDPadPressed) {
        input.mi.mouseData = -1;
        input.mi.dwFlags = MOUSEEVENTF_HWHEEL;
        this->sleepTime = LONG_SLEEP_INTERVAL;
    }
    else if (rightDPadPressed) {
        input.mi.mouseData = 1;
        input.mi.dwFlags = MOUSEEVENTF_HWHEEL;
        this->sleepTime = LONG_SLEEP_INTERVAL;
    }

    // check if left or right bumpers are pressed (left & right click)
    bool leftPressed = (gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) > 0;
    bool rightPressed = (gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) > 0;

    if (leftPressed && this->leftButton == false) {
        input.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
        this->leftButton = true;
    }
    else if (!leftPressed && this->leftButton == true) {
        input.mi.dwFlags |= MOUSEEVENTF_LEFTUP;
        this->leftButton = false;
    }

    if (rightPressed && this->rightButton == false) {
        input.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN;
        this->rightButton = true;
    }
    else if (!leftPressed && this->rightButton == true) {
        input.mi.dwFlags |= MOUSEEVENTF_RIGHTUP;
        this->rightButton = false;
    }

    // check if the 'x' button is pressed; if so, quit
    if ((gamepad.wButtons & XINPUT_GAMEPAD_X) > 0) {
        exit(0);
    }

    this->input = input;
}


void Mouse_SendInput(Mouse* this) {
    // create array of inputs (required for send function)
    INPUT inputArray[1];
    inputArray[0] = this->input;

    // check if data needs to be sent
    if (this->input.mi.dwFlags != 0) {
        // send mouse input to OS
        UINT success = SendInput(1, inputArray, sizeof(INPUT));
    }
}