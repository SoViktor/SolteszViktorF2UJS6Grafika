#include "engine.h"
#include "camera.h"
#include "matrix.h"
#include "world_generator.h"
#include "renderer.h"
#include "model_instance.h"
#include "game.h"

#include <SDL.h>
#include <GL/glew.h>
#include <stdio.h>


static SDL_Window* window = NULL;
static SDL_GLContext gl_context;
Camera camera;
static float projection[16];

static unsigned int g_world_seed = 12345; // Globális world seed
static vec3 player_position = { 32.0f, 0.0f, 32.0f }; // kezdőpozíció a terepen
static float player_speed = 0.2f;
static bool cheat_mode = false;
static bool cheat_camera_enabled = false;

bool wireframe_mode = false;


bool engine_init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        printf("SDL_GL_CreateContext Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("GLEW Error: %s\n", glewGetErrorString(err));
        return false;
    }

    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    camera_init(&camera,
        vec3_create(0.0f, 2.0f, 5.0f),
        vec3_create(0.0f, 0.0f, 0.0f),
        vec3_create(0.0f, 1.0f, 0.0f));

    float aspect = (float)width / (float)height;
    mat4_perspective(projection, 3.14159f / 4.0f, aspect, 0.1f, 500.0f);
    if (!renderer_init()) return false;
    printf("[DEBUG] renderer_init() OK\n");

    if (!renderer_model_init()) return false;
    printf("[DEBUG]renderer_model_init() OK\n");
   
    world_set_seed(g_world_seed);
    world_generate();


    return true;
}



void engine_run(void) {
    printf("[DEBUG] engine_run loop started\n");

    bool running = true;

    camera.azimuth = 0.0f;
    camera.elevation = 0.0f;
    camera.distance = 80.0f;
    camera_update(&camera, game_get_player_position());

    SDL_Event event;
    bool right_mouse_down = false;
    int last_mouse_x = 0, last_mouse_y = 0;

    Uint32 last_time = SDL_GetTicks();

    while (running) {
        // Időalap számítás
        Uint32 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        while (SDL_PollEvent(&event)) {
            game_handle_event(&event); // delegálás a játéknak

            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        right_mouse_down = true;
                        last_mouse_x = event.button.x;
                        last_mouse_y = event.button.y;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        right_mouse_down = false;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (right_mouse_down) {
                        int dx = event.motion.x - last_mouse_x;
                        int dy = event.motion.y - last_mouse_y;
                        camera_orbit(&camera, dx * 0.01f, -dy * 0.01f);
                        last_mouse_x = event.motion.x;
                        last_mouse_y = event.motion.y;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    camera_zoom(&camera, -event.wheel.y * 0.5f);
                    break;
                case SDL_KEYDOWN:
                    break;
            }
        }

        game_update(delta_time); // játéklogika frissítése

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float view[16];
        camera_update(&camera, game_get_player_position());
        camera_get_view_matrix(&camera, view);

        renderer_render(view, projection);
        model_instance_render_all(view, projection);
        multi_model_instance_render_all(view, projection);

        SDL_GL_SwapWindow(window);
    }
}

void engine_shutdown(void) {
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);

    SDL_Quit();
}
