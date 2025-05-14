#include "game.h"
#include "engine.h" // camera

#include "vector.h"
#include "camera.h"
#include "heightmap_reader.h"

#include <GL/glew.h>
#include <math.h>


static vec3 player_position = { 32.0f, 25, 32.0f };
static float player_speed = 50.0f;


extern Camera camera;
bool cheat_mode = false;
bool cheat_camera_enabled = false;
extern bool wireframe_mode; // NE static



void game_init(void) {
    // későbbi inicializációk, pl. animációk
}

vec3 game_get_player_position(void) {
    return player_position;
}



void game_handle_event(const SDL_Event* e) {
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_o:
                camera.distance = 80.0f;
                camera_update(&camera, game_get_player_position());
                printf("[CHEAT] Kamera távolság: 80\n");
                break;

            case SDLK_p:
                cheat_mode = !cheat_mode;
                cheat_camera_enabled = cheat_mode;
                printf("[CHEAT] Szabad kamera mód: %s\n", cheat_mode ? "BE" : "KI");
                break;

            case SDLK_r:
                camera_reset(&camera, game_get_player_position());
                break;

            case SDLK_F2:
                wireframe_mode = !wireframe_mode;
                glPolygonMode(GL_FRONT_AND_BACK, wireframe_mode ? GL_LINE : GL_FILL);
                printf("[DEBUG] Wireframe mode: %s\n", wireframe_mode ? "ON" : "OFF");
                break;

            case SDLK_F3:
                printf("[DEBUG] Camera: pos=(%.2f, %.2f, %.2f), azimuth=%.2f°, elev=%.2f°, dist=%.2f\n",
                    camera.position.x, camera.position.y, camera.position.z,
                    camera.azimuth * 57.3f, camera.elevation * 57.3f, camera.distance);
                break;
        }
    }
}

void game_update(float dt) {
if (cheat_mode) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // Kamera nézeti irányának újraszámítása orbit alapján:
    vec3 forward = {
        cosf(camera.elevation) * sinf(camera.azimuth),
        sinf(camera.elevation),
        cosf(camera.elevation) * cosf(camera.azimuth)
    };
    forward = vec3_normalize(forward);

    vec3 right = vec3_normalize(vec3_cross(forward, camera.up));
    vec3 move = {0};

    if (state[SDL_SCANCODE_W]) move = vec3_add(move, forward);
    if (state[SDL_SCANCODE_S]) move = vec3_sub(move, forward);
    if (state[SDL_SCANCODE_A]) move = vec3_sub(move, right);
    if (state[SDL_SCANCODE_D]) move = vec3_add(move, right);
    if (state[SDL_SCANCODE_SPACE]) move.y += 1.0f;
    if (state[SDL_SCANCODE_LSHIFT]) move.y -= 1.0f;

    move = vec3_scale(vec3_normalize(move), player_speed * dt);
    camera.position = vec3_add(camera.position, move);
    camera.target = vec3_add(camera.position, forward); // frissíti target-et is
    return;
}


    // normál mód – csak XZ síkon mozgunk
    const Uint8* state = SDL_GetKeyboardState(NULL);

    vec3 forward = vec3_sub(camera.target, camera.position);
    forward.y = 0.0f;
    forward = vec3_normalize(forward);

    vec3 right = vec3_cross(forward, camera.up);
    right = vec3_normalize(right);

    vec3 move = {0};
    if (state[SDL_SCANCODE_W]) move = vec3_add(move, forward);
    if (state[SDL_SCANCODE_S]) move = vec3_sub(move, forward);
    if (state[SDL_SCANCODE_A]) move = vec3_sub(move, right);
    if (state[SDL_SCANCODE_D]) move = vec3_add(move, right);

    player_position.x += move.x * player_speed * dt;
    player_position.z += move.z * player_speed * dt;

    // Később: player_position.y = get_height(player_position.x, player_position.z);
}

