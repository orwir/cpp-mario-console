#include "renderSystem.h"

#include <stdio.h>
#include <Windows.h>

HANDLE consoleHandle = 0;
ConsoleSymbolData backBuffer[SCREEN_WIDTH][SCREEN_HEIGHT];
ConsoleSymbolData screenBuffer[SCREEN_WIDTH][SCREEN_HEIGHT];

void RenderSystemInitialize()
{
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = 0;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            backBuffer[x][y].symbol = 0;
            backBuffer[x][y].symbolColor = ConsoleColor_Grey;
            backBuffer[x][y].backgroundColor = ConsoleColor_Black;
            screenBuffer[x][y] = backBuffer[x][y];
        }
    }
}

void RenderSystemClear()
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            backBuffer[x][y].symbol = 0;
            backBuffer[x][y].backgroundColor = ConsoleColor_Black;
        }
    }
}

void RenderSystemDrawChar(int x, int y, char symbol, ConsoleColor symbolColor, ConsoleColor backgroundColor)
{
    backBuffer[x][y].symbol = symbol;
    backBuffer[x][y].symbolColor = symbolColor;
    backBuffer[x][y].backgroundColor = backgroundColor;
}

void RenderSystemDrawText(int x, int y, const char* text, ConsoleColor symbolColor, ConsoleColor backgroundColor)
{
    int tx = x;
    char symbol = *text;
    while (symbol != 0)
    {
        RenderSystemDrawChar(tx, y, symbol, symbolColor, backgroundColor);
        tx++;
        symbol = *(++text);
    }
}

void SetConsoleCursor(int x, int y)
{
    COORD cursorCoord;
    cursorCoord.X = x;
    cursorCoord.Y = y;
    SetConsoleCursorPosition(consoleHandle, cursorCoord);
}

void SetConsoleColor(ConsoleColor symbolColor, ConsoleColor backgroundColor)
{
    unsigned char consoleColor = symbolColor | (backgroundColor << 4);
    SetConsoleTextAttribute(consoleHandle, consoleColor);
}

void RenderSystemFlush()
{
    bool screenBufferModified = false;
    
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            if(backBuffer[x][y] != screenBuffer[x][y]) 
            {
                screenBuffer[x][y] = backBuffer[x][y];

                SetConsoleCursor(x, y);
                SetConsoleColor(screenBuffer[x][y].symbolColor, screenBuffer[x][y].backgroundColor);
                printf("%c", screenBuffer[x][y].symbol);

                screenBufferModified = true;
            }
        }
    }
    if (screenBufferModified)
    {
        SetConsoleCursor(0, 0);
    }
}
