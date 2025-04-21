#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>
#include "camera.h"

bool renderer_init(void);
void renderer_clear(void);
void renderer_draw(vec3 player_position);
void renderer_shutdown(void);
Camera* renderer_get_camera(void);


#endif // RENDERER_H
