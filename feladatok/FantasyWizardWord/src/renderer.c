#include "renderer.h"
#include "camera.h"
#include "matrix.h"
#include "model_registry.h"
#include "map_loader.h"
#include "model_loader_multi.h"
#include "light.h"
#include <GL/glew.h>
#include <SDL.h>
#include <stdio.h>

static Camera camera;
static float projection[16];

static ModelInstance* instances = NULL;
static int instance_count = 0;
static Light main_light;


bool renderer_init(void) {
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        fprintf(stderr, "GLEW Init Error: %s\n", glewGetErrorString(glew_status));
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    unsigned int shader_program = shader_load("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
    
    if (!model_registry_init()) return false;

    camera_init(&camera,
        vec3_create(0.0f, 2.0f, 5.0f),   // camera position
        vec3_create(0.0f, 0.0f, 0.0f),   // target
        vec3_create(0.0f, 1.0f, 0.0f));  // up vector

    int width = 800, height = 600; // vagy ha ez máshonnan jön, onnan vedd
    float aspect = (float)width / (float)height;
    mat4_perspective(projection, 3.14159f / 4.0f, aspect, 0.1f, 100.0f);
    
    if (!map_loader_load("assets/maps/map1.txt", &instances, &instance_count)) {
        fprintf(stderr, "⚠️  Map load failed\n");
        return false;
    }

    light_init(&main_light,
        LIGHT_POINT,
        vec3_create(5.0f, 10.0f, 5.0f),   // pozíció
        vec3_create(0.0f, -1.0f, 0.0f),   // irány (nem használt most)
        vec3_create(1.0f, 1.0f, 1.0f),    // szín (fehér)
        1.0f);                            // intenzitás

    
    // Debug log: csak egyszer
    for (int i = 0; i < instance_count; ++i) {
        Model* m = model_registry_get(instances[i].type);
        printf("[RENDER-ONCE] Model %d at position (%.2f, %.2f, %.2f), texture ID: %u\n",                
            instances[i].type,
            instances[i].position.x,
            instances[i].position.y,
            instances[i].position.z,
            instances[i].texture_id);
        }


    return true;
}

void renderer_clear(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


    void renderer_draw(vec3 player_position) {
        float view[16];
        camera_update(&camera, player_position);
        camera_get_view_matrix(&camera, view);
    
        for (int i = 0; i < instance_count; ++i) {
            Model* m = model_registry_get(instances[i].type);
            if (!m) continue;

//            printf("[RENDERER] Drawing model %d at position (%.2f, %.2f, %.2f), vertices: %d, texture ID: %u\n",
//            instances[i].type,
//            instances[i].position.x,
//            instances[i].position.y,
//            instances[i].position.z,
//            m->vertex_count,
//            m->texture);

    
            glUseProgram(m->shader_program);

            glUniform1i(glGetUniformLocation(m->shader_program, "texture_diffuse"), 0);
            glUniformMatrix4fv(glGetUniformLocation(m->shader_program, "view"), 1, GL_FALSE, view);
            glUniformMatrix4fv(glGetUniformLocation(m->shader_program, "projection"), 1, GL_FALSE, projection);

            // 👇 ÚJ: kamera pozíció világításhoz
            vec3 cam_pos = camera_get_position(&camera);
            glUniform3f(glGetUniformLocation(m->shader_program, "viewPos"),
                        cam_pos.x, cam_pos.y, cam_pos.z);

            // Fény beállítások a shadernek
            if (main_light.enabled) {
                glUniform3f(glGetUniformLocation(m->shader_program, "lightPos"),
                            main_light.position.x, main_light.position.y, main_light.position.z);

                glUniform3f(glGetUniformLocation(m->shader_program, "lightColor"),
                            main_light.color.x, main_light.color.y, main_light.color.z);

                glUniform1f(glGetUniformLocation(m->shader_program, "lightIntensity"),
                            main_light.intensity);
            }


            vec3 pos = instances[i].type == MODEL_WIZARD ? player_position : instances[i].position;
            float s = instances[i].scale;
            float model_matrix[16] = {
                s, 0, 0, 0,
                0, s, 0, 0,
                0, 0, s, 0,
                pos.x, pos.y, pos.z, 1
            };
            glUniformMatrix4fv(glGetUniformLocation(m->shader_program, "model"), 1, GL_FALSE, model_matrix);
    
            if (instances[i].texture_id != 0) {
                glBindTexture(GL_TEXTURE_2D, instances[i].texture_id);
            } else if (m->texture != 0) {
                glBindTexture(GL_TEXTURE_2D, m->texture);
            }
             

            glBindVertexArray(m->vao);
            glDrawArrays(GL_TRIANGLES, 0, m->vertex_count);



        }
    
        glBindVertexArray(0);
        glUseProgram(0);
    }
    


void renderer_shutdown(void) {
    model_registry_shutdown();

    map_loader_free(instances);

}

Camera* renderer_get_camera(void){
    return &camera;
}
