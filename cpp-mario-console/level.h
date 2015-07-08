#pragma once

#include "renderSystem.h"

const int LEVEL_COLS = SCREEN_WIDTH;
const int LEVEL_ROWS = SCREEN_HEIGHT;

const int GRAVITY_SPEED = 38;

const unsigned char CellSymbol_Empty       = ' ';
const unsigned char CellSymbol_BrickWall   = '#';
const unsigned char CellSymbol_SteelWall   = '@';
const unsigned char CellSymbol_Abyss       = 'a';
const unsigned char CellSymbol_Pipe        = 'p';
const unsigned char CellSymbol_Crystal     = '0';
const unsigned char CellSymbol_Box         = '?';
const unsigned char CellSymbol_OpenedBox   = '&';
const unsigned char CellSymbol_Mushroom    = '^';
const unsigned char CellSymbol_MushroomBox = '!';
const unsigned char CellSymbol_Hero        = 'h';
const unsigned char CellSymbol_Goomba      = 'g';
const unsigned char CellSymbol_Exit        = '|';
const unsigned char CellSymbol_Flag        = '>';
const unsigned char CellSymbol_Spring      = 's';
const unsigned char CellSymbol_Koopa       = 'k';
const unsigned char CellSymbol_TurnedKoopa = 't';
const unsigned char CellSymbol_JumpedKoopa = 'j';

const unsigned char levelData0[LEVEL_ROWS][LEVEL_COLS + 1] =
{
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
    "#        0                                                           g         #",
    "#     @                                                              @     |>  #",
    "#    @@     @                   ##?#     0 0 0                      @@     |   #",
    "#   @@@     @@      pppp                           pppp            @@@     |   #",
    "#  @@@@ g   @@@      pp    g         g g            pp            @@@@     |   #",
    "# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#",
    "#                                                                              #",
    "#                  0                                      0 0                  #",
    "#@@      #??#     ###            !                  ?### ######                #",
    "# @@                                                                           #",
    "# @@@                                                                          #",
    "# @@@@    ##      ######     ?   ?   ?    ?#        #             ##           #",
    "# @@@@@                                                                        #",
    "# @@@@@@              # g g g g           k               g g            #     #",
    "# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@##@    #",
    "#                                                                              #",
    "#                ?                                                           ###",
    "#                                                                              #",
    "#                                               pppp                    #!#    #",
    "#           ?  #!#?#                pppp         pp       pppp          # #    #",
    "#                        pppp        pp       #  pp        pp                  #",
    "#  h            g         pp    j    pp g        pp     g gpp        #   g     #",
    "# ##########################################s######################  ###########",
    "#a#################################################################aa###########"
};

const unsigned char levelData1[LEVEL_ROWS][LEVEL_COLS + 1] =
{
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
    "#        0                                                                     #",
    "#                                                                              #",
    "#                              !##?#                                @          #",
    "#   @@@      @      pppp                           pppp            @@@         #",
    "#  @@@@ g    @@      pp    g         g g   # #      pp  g     j   @@@@@      h #",
    "# @@@@@@@@@@@@@@@@@@@@@@@@s@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@##",
    "#                                       @      @                            #  #",
    "#                  0                    @ g  g @          0 0              0#  #",
    "# @      #??#     ###            !      @@@@@@@@    ?### ######          ## #  #",
    "# @                                                             #       #  g g #",
    "# @ @                #                                                  ########",
    "# @ @     ##      ######     ?   ?   ?    ?#        #             #####        #",
    "# @ @ @                                                                        #",
    "# @ @ @@    g   g     #   g g             k     #   g   #   g            #     #",
    "# @a@a@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#@@@@@@@@@@@@@@@@@@@@@##@    #",
    "# #                                                                       #    #",
    "# #              ?                                                        #    #",
    "# #                                              j                        #    #",
    "# #   |>                                        pppp                    #!#    #",
    "# #   |     ?  #?#?#                pppp         pp       pppp         ## #    #",
    "# #   |                  pppp        pp          pp        pp                  #",
    "# #   |         g         pp         pp g        pp     g gpp        #   g     #",
    "# #################################################################  ###########",
    "#a#################################################################aa###########"
};

const unsigned char levelData2[LEVEL_ROWS][LEVEL_COLS + 1] =
{
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
    "#        0   0  0                                                  #           #",
    "#     #      g               #                      ?#         ### #    ??     #",
    "#    @@@@@@@@@@@@@@@@@@@@@@@@@  #??#     0 0 0                #    #           #",
    "#     @                      @                     pppp      #  ####   ????    #",
    "#  @@   g   @@@      g  #    @       g g            pp      #    g     g     # #",
    "# @@@@@@@@@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #",
    "# @                         #                             0 0                  #",
    "# @                !       #        #    j      g #                        #####",
    "# @      #??#      ##     #   ######################?##########  g     g  #    #",
    "# @                      #    #                                ###########     #",
    "# @               # g   #    #                    #                            #",
    "# @       ##      ######## ###   ?   ?    ?#        #             ##           #",
    "# @   @        #      #        #                                               #",
    "# @ h @@   g       g  # g g g g           k               g g            #     #",
    "#a@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@##@    #",
    "#                                                                              #",
    "#                ?                                                           ###",
    "#                                                                              #",
    "#  |>                                           pppp                    #!#    #",
    "#  |    #   ?  #!#?#                pppp         pp       pppp          # #    #",
    "#  |                     pppp        pp          pp        pp                  #",
    "#  |            g         pp         pp g        pp     g gpp        #   g     #",
    "# ##############################  #################################  ###########",
    "#a##############################aa#################################aa###########"
};

unsigned char GetRenderSymbol(unsigned char symbol)
{
    switch (symbol)
    {
    case CellSymbol_Empty:       return ' ';
    case CellSymbol_BrickWall:   return 177;
    case CellSymbol_SteelWall:   return 178;
    case CellSymbol_Abyss:       return ' ';
    case CellSymbol_Pipe:        return 178;
    case CellSymbol_Crystal:     return 4;
    case CellSymbol_Box:         return '?';
    case CellSymbol_OpenedBox:   return 254;
    case CellSymbol_Mushroom:    return 6;
    case CellSymbol_MushroomBox: return '?';
    case CellSymbol_Hero:        return 2;
    case CellSymbol_Goomba:      return 2;
    case CellSymbol_Exit:        return 222;
    case CellSymbol_Flag:        return 16;
    case CellSymbol_Spring:      return 24;
    case CellSymbol_Koopa:       return 2;
    case CellSymbol_TurnedKoopa: return 2;
    case CellSymbol_JumpedKoopa: return 2;
    }
    return '?';
}

ConsoleColor GetRenderSymbolColor(unsigned char symbol)
{
    switch (symbol)
    {
    case CellSymbol_BrickWall:   return ConsoleColor_DarkYellow;
    case CellSymbol_SteelWall:   return ConsoleColor_White;
    case CellSymbol_Pipe:        return ConsoleColor_Green;
    case CellSymbol_Crystal:     return ConsoleColor_Cyan;
    case CellSymbol_Box:         return ConsoleColor_DarkRed;
    case CellSymbol_OpenedBox:   return ConsoleColor_Grey;
    case CellSymbol_Mushroom:    return ConsoleColor_Red;
    case CellSymbol_MushroomBox: return ConsoleColor_DarkRed;
    case CellSymbol_Hero:        return ConsoleColor_Yellow;
    case CellSymbol_Goomba:      return ConsoleColor_DarkRed;
    case CellSymbol_Exit:        return ConsoleColor_Green;
    case CellSymbol_Flag:        return ConsoleColor_White;
    case CellSymbol_Spring:      return ConsoleColor_White;
    case CellSymbol_Koopa:       return ConsoleColor_Green;
    case CellSymbol_TurnedKoopa: return ConsoleColor_DarkGreen;
    case CellSymbol_JumpedKoopa: return ConsoleColor_Green;
    }
    return ConsoleColor_Grey;
}

ConsoleColor GetRenderSymbolBackgroundColor(unsigned char symbol)
{
    switch (symbol)
    {
    case CellSymbol_BrickWall:   return ConsoleColor_Red;
    case CellSymbol_SteelWall:   return ConsoleColor_DarkGrey;
    case CellSymbol_Pipe:        return ConsoleColor_DarkGreen;
    case CellSymbol_Box:         return ConsoleColor_Yellow;
    case CellSymbol_OpenedBox:   return ConsoleColor_DarkGrey;
    case CellSymbol_MushroomBox: return ConsoleColor_Yellow;
    }
    return ConsoleColor_Black;
}

ConsoleColor GetRenderHeroColor(int heroHealth)
{
    switch (heroHealth)
    {
    case 2: return ConsoleColor_Red;
    }
    return ConsoleColor_Yellow;
}

int GetScoreForSymbol(unsigned char symbol)
{
    switch (symbol)
    {
    case CellSymbol_Box:         return 2;
    case CellSymbol_Crystal:     return 10;
    case CellSymbol_Mushroom:    return 13;
    case CellSymbol_Goomba:      return 18;
    case CellSymbol_Koopa:       return 20;
    case CellSymbol_JumpedKoopa: return 23;
    }
    return 0;
}
