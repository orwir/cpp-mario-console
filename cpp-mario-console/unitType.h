#pragma once

#include "level.h"

enum UnitType
{
    UnitType_None,
    UnitType_Hero,
    UnitType_Goomba,
    UnitType_Mushroom,
    UnitType_Koopa,
    UnitType_TurnedKoopa,
    UnitType_JumpedKoopa
};

UnitType GetUnitTypeFromCell(unsigned char symbol)
{
    switch (symbol)
    {
    case CellSymbol_Mario:       return UnitType_Hero;
    case CellSymbol_Luigi:       return UnitType_Hero;
    case CellSymbol_Goomba:      return UnitType_Goomba;
    case CellSymbol_Mushroom:    return UnitType_Mushroom;
    case CellSymbol_Koopa:       return UnitType_Koopa;
    case CellSymbol_TurnedKoopa: return UnitType_TurnedKoopa;
    case CellSymbol_JumpedKoopa: return UnitType_JumpedKoopa;
    }
    return UnitType_None;
}

int GetUnitSpeedX(UnitType unit)
{
    switch (unit)
    {
    case UnitType_Hero:        return 10;
    case UnitType_Goomba:      return 2;
    case UnitType_Mushroom:    return 6;
    case UnitType_Koopa:       return 3;
    case UnitType_JumpedKoopa: return 3;
    case UnitType_TurnedKoopa: return 12;
    }
    return 0;
}

int GetUnitJumpSpeed(UnitType unit)
{
    switch (unit)
    {
    case UnitType_Hero:        return 17;
    case UnitType_TurnedKoopa: return 0;
    }
    return 14;
}
