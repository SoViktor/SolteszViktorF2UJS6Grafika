#pragma once
#include <SDL.h>
#include "vector.h"

void game_init(void);
void game_update(float dt);
void game_handle_event(const SDL_Event* e);
vec3 game_get_player_position(void);
