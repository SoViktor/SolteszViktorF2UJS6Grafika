#include "engine.h"
#include "renderer.h"
#include "camera.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>

static SDL_Window* window = NULL;
static SDL_GLContext gl_context;
static vec3 player_position = { 0.0f, 0.0f, 0.0f };


bool engine_init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        SDL_Quit();
        return false;
    }

    if (TTF_Init() != 0) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        printf("SDL_GL_CreateContext Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    if (!renderer_init()) return false;

    return true;
}

void engine_run(void) {
    Camera* cam = renderer_get_camera();
    vec3 move_dir = vec3_create(0.0f, 0.0f, 0.0f);


    bool running = true;
    SDL_Event event;

    bool right_mouse_down = false;
    int last_mouse_x = 0, last_mouse_y = 0;

    while (running) {
        float speed = 0.1f;
        player_position = vec3_add(player_position, vec3_scale(move_dir, speed));

        camera_update(cam, player_position);


        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    right_mouse_down = true;
                    last_mouse_x = event.button.x;
                    last_mouse_y = event.button.y; 
                }
                
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    right_mouse_down = false;
                }
                break;

            case SDL_MOUSEMOTION:
                if (right_mouse_down)
                {
                    int dx = event.motion.x - last_mouse_x;
                    int dy = event.motion.y - last_mouse_y;

                    camera_orbit(cam, dx * 0.01f, -dy * 0.01f);

                    last_mouse_x = event.motion.x;
                    last_mouse_y = event.motion.y;
                }
                break;

            case SDL_MOUSEWHEEL:
                camera_zoom(cam, -event.wheel.y * 0.5f);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_w: move_dir.z = -1.0f; break;
                    case SDLK_s: move_dir.z =  1.0f; break;
                    case SDLK_a: move_dir.x = -1.0f; break;
                    case SDLK_d: move_dir.x =  1.0f; break;
                    case SDLK_r: camera_reset(cam, player_position); break;
                }
                break;
            
            case SDL_KEYUP:
            switch (event.key.keysym.sym) {
                case SDLK_w:
                case SDLK_s: move_dir.z = 0.0f; break;
                case SDLK_a:
                case SDLK_d: move_dir.x = 0.0f; break;
            }
            break;
            }
        }

        renderer_clear();
        renderer_draw(player_position);

        SDL_GL_SwapWindow(window);
    }
}

void engine_shutdown(void) {
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    renderer_shutdown();

}
