#pragma once

#include <Windows.h>

bool isKeyDown(int virtualKeyCode)
{
    short keyState = GetAsyncKeyState(virtualKeyCode);
    return (keyState & 0x8000) > 0;
}