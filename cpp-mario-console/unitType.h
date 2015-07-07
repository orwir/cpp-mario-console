#pragma once

#include "level.h"

enum UnitType
{
    UnitType_None,
    UnitType_Hero,
    UnitType_Goomba
};

UnitType GetUnitTypeFromCell(unsigned char symbol)
{
    switch (symbol)
    {
    case CellSymbol_Hero:   return UnitType_Hero;
    case CellSymbol_Goomba: return UnitType_Goomba;
    }
    return UnitType_None;
}

int GetUnitSpeedX(UnitType unit)
{
    switch (unit)
    {
    case UnitType_Hero:   return 10;
    case UnitType_Goomba: return 2;
    }
    return 0;
}

int GetUnitJumpSpeed(UnitType unit)
{
    switch (unit)
    {
    case UnitType_Hero: return 17;
    }
    return 0;
}
