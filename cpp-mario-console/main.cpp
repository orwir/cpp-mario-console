#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <ctype.h>
#include <cmath>
#include "renderSystem.h"
#include "input.h"
#include "level.h"
#include "unitData.h"

#define PI 3.14159265

void SetupSystem();
void Initialize();
void Render();
void Update();
void Shutdown();

////////////////////////////////////
//CONSTANTS
const int maxLevels = 3;
const int maxUnitsCount = 35;
const float cellBeginValue = 0.001f;
const float cellEndValue = 0.999f;

//VARIABLES
bool isGameActive = true;

clock_t clockLastFrame = 0;
int framesCounter = 0;
float framesTimeCounter = 0;
int fps = 0;

int currentLevel = 0;
unsigned char levelData[LEVEL_ROWS][LEVEL_COLS];

UnitData unitsData[maxUnitsCount];
int unitsCount = 0;
int heroIndex = 0;

//Hero data
int prevLevelHeroHealth = 1;
int score = 0;

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
    currentLevel = 0;
}

void Initialize()
{
    clockLastFrame = clock();

    unitsCount = 0;
    heroIndex = 0;

    for (int r = 0; r < LEVEL_ROWS; r++)
    {
        for (int c = 0; c < LEVEL_COLS; c++)
        {
            unsigned char symbol = 0;
            switch (currentLevel)
            {
            case 0:
                symbol = levelData0[r][c];
                break;
            case 1:
                symbol = levelData1[r][c];
                break;
            case 2:
                symbol = levelData2[r][c];
            }

            levelData[r][c] = symbol;

            switch (symbol)
            {
            case CellSymbol_Hero:
                heroIndex = unitsCount;
            case CellSymbol_Goomba:
            case CellSymbol_Mushroom:
                UnitType unitType = GetUnitTypeFromCell(symbol);
                unitsData[unitsCount].type = unitType;
                unitsData[unitsCount].x = float(c);
                unitsData[unitsCount].y = float(r);
                unitsData[unitsCount].health = 1;
                unitsData[unitsCount].xSpeed = 0.0;
                unitsData[unitsCount].ySpeed = 0.0;
                unitsData[unitsCount].xOrder = UnitOrder_None;
                unitsData[unitsCount].yOrder = UnitOrder_None;
                unitsCount++;

                break;
            }
        }
    }
    unitsData[heroIndex].health = prevLevelHeroHealth;
    prevLevelHeroHealth = 1;
}

void Render()
{
    RenderSystemClear();

    for (int r = 0; r < LEVEL_ROWS; r++)
    {
        for (int c = 0; c < LEVEL_COLS; c++)
        {
            unsigned char symbol = levelData[r][c];
            unsigned char renderSymbol = GetRenderSymbol(symbol);
            ConsoleColor symbolColor = GetRenderSymbolColor(symbol);
            ConsoleColor backgroundColor = GetRenderSymbolBackgroundColor(symbol);

            if (symbol == CellSymbol_Hero)
            {
                symbolColor = GetRenderHeroColor(unitsData[heroIndex].health);
            }

            RenderSystemDrawChar(c, r, renderSymbol, symbolColor, backgroundColor);
        }
    }

    char textBuffer[32];
    sprintf_s(textBuffer, "FPS: %d", fps);
    RenderSystemDrawText(0, 0, textBuffer, ConsoleColor_DarkGrey, GetRenderSymbolColor(CellSymbol_SteelWall));
    int lenght = sprintf_s(textBuffer, "Your score: %04d", score);
    RenderSystemDrawText(SCREEN_WIDTH - lenght, 0, textBuffer, ConsoleColor_DarkGrey, GetRenderSymbolColor(CellSymbol_SteelWall));

    RenderSystemFlush();
}

UnitData* GetUnitAt(int x, int y)
{
    for (int u = 0; u < unitsCount; u++)
    {
        if (unitsData[u].health <= 0)
        {
            continue;
        }

        if (int(unitsData[u].x) == x && int(unitsData[u].y) == y)
        {
            return &unitsData[u];
        }
    }
    return 0;
}

void KillUnit(UnitData* unit)
{
    unit->health = 0;
    int col = int(unit->x);
    int row = int(unit->y);
    levelData[row][col] = CellSymbol_Empty;
}

bool MoveUnitTo(UnitData* unit, float newX, float newY)
{
    if (unit->health <= 0)
    {
        return false;
    }

    bool canMove = false;
    int newCol = int(newX);
    int newRow = int(newY);
    int oldCol = int(unit->x);
    int oldRow = int(unit->y);

    unsigned char unitSymbol = levelData[oldRow][oldCol];
    unsigned char destinationSymbol = levelData[newRow][newCol];

    int directionCol = newCol - oldCol;
    int directionRow = newRow - oldRow;

    switch (destinationSymbol)
    {
    case CellSymbol_Empty:
        canMove = true;
        break;
    case CellSymbol_Abyss:
        KillUnit(unit);
        break;
    }

    if (unit->type == UnitType_Hero) //hero actions
    {
        switch (destinationSymbol)
        {
        case CellSymbol_Exit:
            currentLevel++;
            if (currentLevel < maxLevels) {
                //save data
                prevLevelHeroHealth = unit->health;

                Initialize();
            }
            else
            {
                isGameActive = false;
            }
            break;
        case CellSymbol_Box:
            if (directionRow < 0)
            {
                levelData[newRow - 1][newCol] = CellSymbol_Crystal;
                levelData[newRow][newCol] = CellSymbol_OpenedBox;
            }
            break;
        case CellSymbol_MushroomBox:
            if (directionRow < 0)
            {
                levelData[newRow - 1][newCol] = CellSymbol_Mushroom;
                levelData[newRow][newCol] = CellSymbol_OpenedBox;
                UnitType unitType = GetUnitTypeFromCell(CellSymbol_Mushroom);
                unitsData[unitsCount].type = unitType;
                unitsData[unitsCount].x = float(newCol);
                unitsData[unitsCount].y = float(newRow - 1);
                unitsData[unitsCount].health = 1;
                unitsData[unitsCount].xSpeed = 0.0;
                unitsData[unitsCount].ySpeed = 0.0;
                unitsData[unitsCount].xOrder = UnitOrder_None;
                unitsData[unitsCount].yOrder = UnitOrder_None;
                unitsCount++;
            }
            break;
        case CellSymbol_Crystal:
            canMove = true;
            score += GetScoreForSymbol(destinationSymbol);
            break;
        case CellSymbol_Mushroom:
        {
            canMove = true;
            UnitData* mushroom = GetUnitAt(newCol, newRow);
            if (mushroom != 0)
            {
                score += GetScoreForSymbol(destinationSymbol);
                KillUnit(mushroom);
                if (unit->health != 2)
                {
                    unit->health = 2;
                }
            }
            break;
        }
        case CellSymbol_BrickWall:
            if (directionRow < 0 && unit->health > 1)
            {
                score += GetScoreForSymbol(destinationSymbol);
                levelData[newRow][newCol] = CellSymbol_Empty;
            }
            break;
        case CellSymbol_Goomba:
            if (directionRow > 0)
            {
                UnitData* goomba = GetUnitAt(newCol, newRow);
                if (goomba != 0)
                {
                    score += GetScoreForSymbol(destinationSymbol);
                    KillUnit(goomba);
                    unit->ySpeed = -GetUnitJumpSpeed(unit->type);
                }
            }
            break;
        }
    }
    else
    { //monsters actions
        switch (destinationSymbol)
        {
        case CellSymbol_Hero:
            if (unit->type == UnitType_Mushroom) {
                score += GetScoreForSymbol(levelData[oldRow][oldCol]);
                KillUnit(unit);
                if (unitsData[heroIndex].health != 2)
                {
                    unitsData[heroIndex].health = 2;
                }
            }
            else {
                unitsData[heroIndex].health--;
                if (unit->xOrder == UnitOrder_Backward)
                {
                    unit->xOrder = UnitOrder_Forward;
                }
                else {
                    unit->xOrder = UnitOrder_Backward;
                }
            }
            break;
        default:
            UnitType type = GetUnitTypeFromCell(destinationSymbol);
            if (type != UnitType_None) {
                if (unit->xOrder == UnitOrder_Backward)
                {
                    unit->xOrder = UnitOrder_Forward;
                }
                else {
                    unit->xOrder = UnitOrder_Backward;
                }
            }
            break;
        }
    }

    if (canMove)
    {
        levelData[oldRow][oldCol] = CellSymbol_Empty;
        unit->x = newX;
        unit->y = newY;
        levelData[newRow][newCol] = unitSymbol;
    }

    return canMove;
}

void UpdateUnit(UnitData* unit, float deltaTime)
{
    int col = int(unit->x);
    int row = int(unit->y);

    //Y ORDERS
    if (unit->yOrder == UnitOrder_Backward)
    {
        if (unit->y >= (row + cellEndValue)
        && levelData[row + 1][col] != CellSymbol_Empty
        && levelData[row + 1][col] != CellSymbol_Abyss)
        {
            unit->ySpeed = -GetUnitJumpSpeed(unit->type);
        }
    }

    //X ORDERS
    unit->xSpeed = GetUnitSpeedX(unit->type) * unit->xOrder;

    float deltaX = unit->xSpeed * deltaTime;
    float deltaY = unit->ySpeed * deltaTime;
    float newX = unit->x + deltaX;
    float newY = unit->y + deltaY;
    int newCol = int(newX);
    int newRow = int(newY);

    if (newRow != row)
    {
        if (MoveUnitTo(unit, unit->x, newY))
        {
            row = int(unit->y);
        }
        else if(newRow > row)
        {
            unit->y = row + cellEndValue;
            if (unit->ySpeed > 0.0)
            {
                unit->ySpeed = 0;
            }
        }
        else
        {
            unit->y = row + cellBeginValue;
            if (unit->ySpeed < 0.0)
            {
                unit->ySpeed = 0;
            }
        }
    }
    else {
        unit->y = newY;
    }

    if (newCol != col)
    {
        if (MoveUnitTo(unit, newX, unit->y))
        {
            col = int(unit->x);
        }
        else if (newCol > col)
        {
            unit->x = col + cellEndValue;
            if (unit->xSpeed > 0.0)
            {
                unit->xSpeed = 0;
            }
        }
        else
        {
            unit->x = col + cellBeginValue;
            if (unit->xSpeed < 0.0)
            {
                unit->xSpeed = 0;
            }
        }
    }
    else {
        unit->x = newX;
    }

    //gravity
    unit->ySpeed += GRAVITY_SPEED * deltaTime;
    if (unit->ySpeed > GRAVITY_SPEED)
    {
        unit->ySpeed = GRAVITY_SPEED;
    }
}

void UpdateAI()
{
    for (int u = 0; u < unitsCount; u++)
    {
        if (u == heroIndex || unitsData[u].health <= 0)
        {
            continue;
        }

        int col = int(unitsData[u].x);
        int row = int(unitsData[u].y);

        if (unitsData[u].xOrder == UnitOrder_None)
        {
            if (unitsData[u].type == UnitType_Mushroom)
            {
                if (levelData[row][col + 1] == CellSymbol_Empty)
                {
                    unitsData[u].xOrder = UnitOrder_Forward;
                }
                else
                {
                    unitsData[u].xOrder = UnitOrder_Backward;
                }
            }
            else
            {
                if (levelData[row][col - 1] == CellSymbol_Empty)
                {
                    unitsData[u].xOrder = UnitOrder_Backward;
                }
                else
                {
                    unitsData[u].xOrder = UnitOrder_Forward;
                }
            }
        }
        else if (unitsData[u].xOrder == UnitOrder_Backward)
        {
            unsigned char cellLeft = levelData[row][col - 1];
            UnitType type = GetUnitTypeFromCell(cellLeft);

            if (unitsData[u].x <= (col + cellBeginValue) && cellLeft != CellSymbol_Empty && type == UnitType_None)
            {
                unitsData[u].xOrder = UnitOrder_Forward;
            }
        }
        else if(unitsData[u].xOrder == UnitOrder_Forward)
        {
            unsigned char cellRight = levelData[row][col + 1];
            UnitType type = GetUnitTypeFromCell(cellRight);

            if (unitsData[u].x >= (col + cellEndValue) && cellRight != CellSymbol_Empty && type == UnitType_None)
            {
                unitsData[u].xOrder = UnitOrder_Backward;
            }
        }
    }
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

    if (isKeyDown(VK_UP))
    {
        unitsData[heroIndex].yOrder = UnitOrder_Backward;
    }
    else
    {
        unitsData[heroIndex].yOrder = UnitOrder_None;
    }

    if (isKeyDown(VK_LEFT))
    {
        unitsData[heroIndex].xOrder = UnitOrder_Backward;
    }
    else if (isKeyDown(VK_RIGHT))
    {
        unitsData[heroIndex].xOrder = UnitOrder_Forward;
    }
    else
    {
        unitsData[heroIndex].xOrder = UnitOrder_None;
    }

    for (int u = 0; u < unitsCount; u++)
    {
        UpdateUnit(&unitsData[u], deltaTime);
    }

    UpdateAI();

    if (unitsData[heroIndex].health <= 0)
    {
        Initialize();
    }
}

void Shutdown()
{
    RenderSystemDrawText(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "Game over...", ConsoleColor_Red, ConsoleColor_Blue);
}
