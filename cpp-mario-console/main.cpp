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
Player mario;
Player luigi;

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
    mario.score = 0;
    luigi.score = 0;
}

void AddUnit(UnitType type, int row, int col)
{
    unitsData[unitsCount].type = type;
    unitsData[unitsCount].x = float(col);
    unitsData[unitsCount].y = float(row);
    unitsData[unitsCount].health = 1;
    unitsData[unitsCount].xSpeed = 0.0;
    unitsData[unitsCount].ySpeed = 0.0;
    unitsData[unitsCount].xOrder = UnitOrder_None;
    unitsData[unitsCount].yOrder = UnitOrder_None;
    unitsCount++;
}

void Initialize()
{
    clockLastFrame = clock();

    unitsCount = 0;
    mario.index = 0;
    luigi.index = 0;

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
            case CellSymbol_Mario:
            case CellSymbol_Luigi:
                if (symbol == CellSymbol_Mario)
                {
                    mario.index = unitsCount;
                }
                else
                {
                    luigi.index = unitsCount;
                }
            case CellSymbol_Goomba:
            case CellSymbol_Mushroom:
            case CellSymbol_Koopa:
            case CellSymbol_TurnedKoopa:
            case CellSymbol_JumpedKoopa:
                UnitType unitType = GetUnitTypeFromCell(symbol);
                AddUnit(unitType, r, c);
                break;
            }
        }
    }
}

bool IsCellVisible(int row, int col)
{
    float mr = unitsData[mario.index].y;
    float mc = unitsData[mario.index].x;
    float lr = unitsData[luigi.index].y;
    float lc = unitsData[luigi.index].x;

    return (sqrt(pow(mr-row, 2)*2 + pow(mc-col, 2)) < 15 && unitsData[mario.index].health > 0)
        || (sqrt(pow(lr-row, 2)*2 + pow(lc-col, 2)) < 15 && unitsData[luigi.index].health > 0);
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

            if (symbol == CellSymbol_Mario)
            {
                symbolColor = GetRenderHeroColor(unitsData[mario.index].health);
            }
            if (symbol == CellSymbol_Luigi)
            {
                symbolColor = GetRenderHeroColor(unitsData[luigi.index].health);
            }

            if (IsCellVisible(r, c))
            {
                RenderSystemDrawChar(c, r, renderSymbol, symbolColor, backgroundColor);
            }
            else
            {
                RenderSystemDrawChar(c, r, '#', ConsoleColor_DarkGrey, ConsoleColor_DarkGrey);
            }
        }
    }

    char textBuffer[32];
    sprintf_s(textBuffer, "FPS: %d", fps);
    RenderSystemDrawText(0, 0, textBuffer, ConsoleColor_DarkGrey, GetRenderSymbolColor(CellSymbol_SteelWall));
    int lenght = sprintf_s(textBuffer, "Mario: %04d   Luigi: %04d", mario.score, luigi.score);
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
    if (unit->type == UnitType_Koopa || unit->type == UnitType_JumpedKoopa)
    {
        levelData[row][col] = CellSymbol_TurnedKoopa;
        AddUnit(UnitType_TurnedKoopa, row, col);
    }
    else {
        levelData[row][col] = CellSymbol_Empty;
    }
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

    unsigned char floor = levelData[newRow + 1][newCol];
    if (floor == CellSymbol_Spring)
    {
        unit->yOrder = UnitOrder_Backward;
    }

    if (unit->type == UnitType_Hero) //hero actions
    {
        Player* player = (levelData[oldRow][oldCol] == CellSymbol_Mario) ? &mario : &luigi;

        switch (destinationSymbol)
        {
        case CellSymbol_Exit:
            currentLevel++;
            if (currentLevel < maxLevels) {
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
                AddUnit(UnitType_Mushroom, newRow - 1, newCol);
            }
            break;
        case CellSymbol_Crystal:
            canMove = true;
            player->score += GetScoreForSymbol(destinationSymbol);
            break;
        case CellSymbol_Mushroom:
        {
            canMove = true;
            UnitData* mushroom = GetUnitAt(newCol, newRow);
            if (mushroom != 0)
            {
                player->score += GetScoreForSymbol(destinationSymbol);
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
                player->score += GetScoreForSymbol(destinationSymbol);
                levelData[newRow][newCol] = CellSymbol_Empty;
            }
            break;
        case CellSymbol_Goomba:
        case CellSymbol_Koopa:
        case CellSymbol_JumpedKoopa:
            if (directionRow > 0)
            {
                UnitData* monster = GetUnitAt(newCol, newRow);
                if (monster != 0)
                {
                    player->score += GetScoreForSymbol(destinationSymbol);
                    KillUnit(monster);
                    unit->ySpeed = -GetUnitJumpSpeed(unit->type);
                }
            }
            else if(directionRow == 0)
            {
                unitsData[player->index].health--;
                if (unitsData[player->index].health <= 0)
                {
                    KillUnit(&unitsData[player->index]);
                }
            }
            break;
        case CellSymbol_TurnedKoopa:
        {
            UnitData* turnedKoopa = GetUnitAt(newCol, newRow);
            if (turnedKoopa != 0)
            {
                if (directionRow == 0)
                {
                    turnedKoopa->xOrder = (directionCol < 0) ? UnitOrder_Backward : UnitOrder_Forward;
                }
                else if (directionRow > 0)
                {
                    turnedKoopa->xOrder = UnitOrder_None;
                    unit->ySpeed = -GetUnitJumpSpeed(unit->type);
                }
            }
            break;
        }
        }
    }
    else
    { //monsters actions
        Player* player = (levelData[newRow][newCol] == CellSymbol_Mario) ? &mario : &luigi;
        
        switch (destinationSymbol)
        {
        case CellSymbol_Mario:
        case CellSymbol_Luigi:
            if (unit->type == UnitType_Mushroom) {
                player->score += GetScoreForSymbol(levelData[oldRow][oldCol]);
                KillUnit(unit);
                if (unitsData[player->index].health != 2)
                {
                    unitsData[player->index].health = 2;
                }
            }
            else if(directionRow > 0 || directionCol != 0)
            {
                unitsData[player->index].health--;
                if (unit->xOrder == UnitOrder_Backward)
                {
                    unit->xOrder = UnitOrder_Forward;
                }
                else {
                    unit->xOrder = UnitOrder_Backward;
                }
                if (unitsData[player->index].health <= 0)
                {
                    KillUnit(&unitsData[player->index]);
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
                else
                {
                    unit->xOrder = UnitOrder_Backward;
                }
                if (unit->type == UnitType_TurnedKoopa && type != UnitType_TurnedKoopa)
                {
                    UnitData* metUnit = GetUnitAt(newCol, newRow);
                    KillUnit(metUnit);
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
            unit->yOrder = UnitOrder_None;
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
        if (u == mario.index || u == luigi.index || unitsData[u].health <= 0)
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
            else if(unitsData[u].type != UnitType_TurnedKoopa)
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

        if (unitsData[u].type == UnitType_JumpedKoopa)
        {
            unitsData[u].yOrder = UnitOrder_Backward;
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

    //MARIO CONTROL
    if (isKeyDown(VK_UP))
    {
        unitsData[mario.index].yOrder = UnitOrder_Backward;
    }
    else
    {
        int row = int(unitsData[mario.index].y);
        int col = int(unitsData[mario.index].x);
        if (levelData[row + 1][col] != CellSymbol_Spring)
        {
            unitsData[mario.index].yOrder = UnitOrder_None;
        }
    }

    if (isKeyDown(VK_LEFT))
    {
        unitsData[mario.index].xOrder = UnitOrder_Backward;
    }
    else if (isKeyDown(VK_RIGHT))
    {
        unitsData[mario.index].xOrder = UnitOrder_Forward;
    }
    else
    {
        unitsData[mario.index].xOrder = UnitOrder_None;
    }

    //LUIGI CONTROL
    if (isKeyDown('W'))
    {
        unitsData[luigi.index].yOrder = UnitOrder_Backward;
    }
    else
    {
        int row = int(unitsData[luigi.index].y);
        int col = int(unitsData[luigi.index].x);
        if (levelData[row + 1][col] != CellSymbol_Spring)
        {
            unitsData[luigi.index].yOrder = UnitOrder_None;
        }
    }

    if (isKeyDown('A'))
    {
        unitsData[luigi.index].xOrder = UnitOrder_Backward;
    }
    else if (isKeyDown('D'))
    {
        unitsData[luigi.index].xOrder = UnitOrder_Forward;
    }
    else
    {
        unitsData[luigi.index].xOrder = UnitOrder_None;
    }

    for (int u = 0; u < unitsCount; u++)
    {
        UpdateUnit(&unitsData[u], deltaTime);
    }

    UpdateAI();

    if (unitsData[mario.index].health <= 0 && unitsData[luigi.index].health <= 0 || isKeyDown('R'))
    {
        Initialize();
    }
}

void Shutdown()
{
    RenderSystemDrawText(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "Game over...", ConsoleColor_Red, ConsoleColor_Blue);
}
