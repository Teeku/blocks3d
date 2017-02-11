#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <stdlib.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>

#include "game.h"
#include "utils.h"

#define LogInput(msg, ...) SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, msg, ##__VA_ARGS__)

void ProcessInput(GameState *gs);

#endif // INPUT_H_INCLUDED
