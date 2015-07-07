#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <ctype.h>
#include <cmath>
#include "renderSystem.h"
#include "input.h"

#define PI 3.14159265

void SetupSystem();
void Initialize();
void Render();
void Update();
void Shutdown();

////////////////////////////////////
//VARIABLES
bool isGameActive = true;
clock_t clockLastFrame = 0;

int framesCounter = 0;
float framesTimeCounter = 0;
int fps = 0;

struct Point
{
    int x;
    int y;
} sinusoid[SCREEN_WIDTH];
int startX = 0;

void main()
{
    SetupSystem();
    Initialize();

    do
    {
        Render();
        Update();
    } while (isGameActive);

    Shutdown();
}

void SetupSystem()
{
    srand(time(0));
    RenderSystemInitialize();
}

void Initialize()
{
    clockLastFrame = clock();
}

void Render()
{
    RenderSystemClear();

    /*RenderSystemDrawText(8, 30, "Test Console Render System!", ConsoleColor_Red, ConsoleColor_DarkBlue);
    if (isKeyDown(VK_UP))
    {
        RenderSystemDrawChar(12, 33, 24, ConsoleColor_Red, ConsoleColor_Grey);
    }
    if (isKeyDown(VK_LEFT))
    {
        RenderSystemDrawChar(11, 34, 27, ConsoleColor_Blue, ConsoleColor_White);
    }
    if (isKeyDown(VK_DOWN))
    {
        RenderSystemDrawChar(12, 35, 25, ConsoleColor_Yellow, ConsoleColor_DarkGrey);
    }
    if (isKeyDown(VK_RIGHT))
    {
        RenderSystemDrawChar(13, 34, 26, ConsoleColor_Green, ConsoleColor_DarkCyan);
    }*/

    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        RenderSystemDrawChar(sinusoid[i].x, sinusoid[i].y, 219, ConsoleColor_Red, ConsoleColor_Black);
    }

    char textBuffer[32];
    sprintf_s(textBuffer, "FPS: %d", fps);
    RenderSystemDrawText(0, 0, textBuffer, ConsoleColor_Grey, ConsoleColor_Black);

    RenderSystemFlush();
}

void Update()
{
    clock_t clockNow = clock();
    clock_t deltaClock = clockNow - clockLastFrame;
    float deltaTime = float(deltaClock) / CLOCKS_PER_SEC;
    clockLastFrame = clockNow;

    framesCounter++;
    framesTimeCounter += deltaTime;
    if (framesTimeCounter >= 1.0)
    {
        framesTimeCounter -= 1.0;
        fps = framesCounter;
        framesCounter = 0;
    }

    if (isKeyDown(VK_RIGHT))
    {
        startX++;
    }
    else if (startX > 0)
    {
        startX--;
    }

    int sy = SCREEN_HEIGHT / 2;
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        sinusoid[i].x = i;
        sinusoid[i].y = sy - ((sy / 3) * (1 - sin((startX+i) * 7 * PI / SCREEN_WIDTH)));
    }
}

void Shutdown()
{

}
