#pragma once

#include "consoleColor.h"

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;

struct ConsoleSymbolData
{
    char symbol;
    ConsoleColor symbolColor;
    ConsoleColor backgroundColor;

    bool operator==(ConsoleSymbolData other)
    {
        return symbol == other.symbol && symbolColor == other.symbolColor && backgroundColor == other.backgroundColor;
    }

    bool operator!=(ConsoleSymbolData other)
    {
        return !(*this == other);
    }
};

void RenderSystemInitialize();
void RenderSystemClear();
void RenderSystemDrawChar(int x, int y, char symbol, ConsoleColor symbolColor, ConsoleColor backgroundColor);
void RenderSystemDrawText(int x, int y, const char* text, ConsoleColor symbolColor, ConsoleColor backgroundColor);
void RenderSystemFlush();
