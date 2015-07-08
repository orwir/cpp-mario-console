#pragma once

#include "unitOrder.h"
#include "unitType.h"

struct UnitData
{
    UnitType type;
    int health;

    float x;
    float y;

    float xSpeed;
    float ySpeed;
    
    UnitOrder xOrder;
    UnitOrder yOrder;
};

struct Player
{
    int index;
    int score;
};
