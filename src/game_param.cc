// CPP
#include <string>
#include <SDL2/SDL.h>

// LOCAL
#include "game_param.h"

int GRID_SIZE = 10;
int GRIDWIDTH = GRID_SIZE;
int GRIDHEIGHT= GRID_SIZE;

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 600;

const int GAME_PANEL_WIDTH = 500;
const int GAME_PANEL_HEIGHT= 500;

const int FONT_SIZE = 22;

#define COLOURS \
COLOUR("White",   0, 255, 255, 255, 255) \
COLOUR("Purple",  1, 204, 153, 255, 255) \
COLOUR("Yellow",  2, 255, 255, 153, 255) \
COLOUR("Red",     3, 255, 153, 153, 255) \
COLOUR("Green",   4, 153, 255, 153, 255) \
COLOUR("Blue",    5, 102, 178, 255, 255) \
COLOUR("Black",   6,   0,   0,   0, 255)


#define COLOUR(name, num, r, g, b, a) {r, g, b, a},
const SDL_Color kColors[] =
{
  COLOURS
};
#undef COLOUR

#define COLOUR(name, num, r, g, b, a) name,
const std::string kColorNames[] =
{
  COLOURS
};
#undef COLOUR


// SEASRN__.ttf
// Capture_it.ttf
// DroidSans.ttf
// DroidSans-Bold.ttf

const std::string FONT_FILE = "Resources/DroidSans.ttf";