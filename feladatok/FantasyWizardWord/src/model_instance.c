#include "model_instance.h"
#include "renderer.h"
#include "matrix.h"
#include "shader.h"
#include "model_instance.h"
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <stdio.h>

#define MAX_INSTANCES 1024
#define SCALE_Y_CORRECTION 0
#define SCALE_XZ_CORRECTION 0

static ModelInstance instances[MAX_INSTANCES];
static size_t instance_count = 0;

void model_instance_add(Model* model, vec3 position, float scale, float rotation_deg) {
    if (instance_count >= MAX_INSTANCES) {
        return;
    }

    instances[instance_count].model = model;
    instances[instance_count].position = position;
    instances[instance_count].scale = scale;
    instances[instance_count].rotation_deg = rotation_deg;
    instance_count++;
}

void model_instance_clear_all(void) {
    instance_count = 0;
}

void model_instance_render_all(const float view[16], const float proj[16]) {
    
 

    shader_set_active(renderer_get_model_shader());
    glUseProgram(renderer_get_model_shader());
    
    GLuint shader_id =  renderer_get_model_shader();
    if (shader_id == 0) return;

    glUniformMatrix4fv(glGetUniformLocation(shader_id, "uView"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "uProj"), 1, GL_FALSE, proj);


    for (size_t i = 0; i < instance_count; ++i) {
        const ModelInstance* inst = &instances[i];

    
        float model_matrix[16];
        mat4_identity(model_matrix);
    
        float t[16], r[16], s[16], temp[16];
        mat4_identity(model_matrix);
        mat4_scale_uniform(s, inst->scale);
        mat4_rotate_y(r, inst->rotation_deg);
        mat4_translate(t, inst->position.x, inst->position.y, inst->position.z);

        mat4_mul(temp, r, s);               // rot * scale
        mat4_mul(model_matrix, t, temp);   // translate * (rot * scale)



    
        glUseProgram(shader_id);
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uModel"), 1, GL_FALSE, model_matrix);
        renderer_render_model(inst->model, view, proj);
    }
    
}
