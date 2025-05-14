#include "multi_model_instance.h"
#include "matrix.h"
#include "shader.h"
#include "renderer.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_MULTI_INSTANCES 512
#define SCALE_Y_CORRECTION 0
#define SCALE_XZ_CORRECTION 0

typedef struct {
    MultiModel* model;
    vec3 position;
    float scale;
    float rotation_deg;
} MultiModelInstance;

static MultiModelInstance instances[MAX_MULTI_INSTANCES];
static size_t instance_count = 0;

void multi_model_instance_add(MultiModel* model, vec3 position, float scale, float rotation_deg) {
    printf("[ADD] Adding instance for model with %u submeshes\n", model->submesh_count);

    if (instance_count >= MAX_MULTI_INSTANCES) return;
    instances[instance_count].model = model;
    instances[instance_count].position = position;
    instances[instance_count].scale = scale;
    instances[instance_count].rotation_deg = rotation_deg;
    instance_count++;

    printf("[INSTANCE] Added MultiModel instance at %.2f %.2f %.2f\n", 
        position.x, position.y, position.z);
}

void multi_model_instance_clear_all(void) {
    instance_count = 0;
}

void multi_model_instance_render_all(const float view[16], const float proj[16]) {
    static bool logged_once = false;

    if (!logged_once) {
        printf("[RENDER] MultiModel: %zu instances\n", instance_count);
        GLuint shader_id = shader_get_active();
        printf("[RENDER] Active shader ID: %u\n", shader_id);
        logged_once = true;
    }

    GLuint shader_id = shader_get_active();
    if (shader_id == 0) {
        return;
    }

    glUseProgram(shader_id);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "uView"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "uProj"), 1, GL_FALSE, proj);

    for (size_t i = 0; i < instance_count; ++i) {
        const MultiModelInstance* inst = &instances[i];
        const MultiModel* model = inst->model;

        float model_matrix[16], t[16], r[16], s[16], temp[16];

        mat4_identity(model_matrix);
        mat4_scale_uniform(s, inst->scale);
        mat4_rotate_y(r, inst->rotation_deg);
        mat4_translate(t, inst->position.x, inst->position.y, inst->position.z);

        mat4_mul(temp, r, s);               // rot * scale
        mat4_mul(model_matrix, t, temp);   // translate * (rot * scale)




        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uModel"), 1, GL_FALSE, model_matrix);
        glUniform1f(glGetUniformLocation(shader_id, "uAlpha"), model->alpha);
        glUniform1i(glGetUniformLocation(shader_id, "uUseLighting"), model->use_lighting ? 1 : 0);

        for (unsigned int j = 0; j < model->submesh_count; ++j) {
            const Submesh* sm = &model->submeshes[j];

            if (!logged_once) {
                printf("[RENDER] Submesh %u: VAO=%u, indices=%u, texID=%u\n",
                       j, sm->vao, sm->index_count, sm->texture_id);
            }

            if (sm->vao == 0 || sm->index_count == 0) {
                if (!logged_once) {
                    printf("[WARN] Submesh %u is not drawable!\n", j);
                }
                continue;
            }

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, sm->texture_id);
            glUniform1i(glGetUniformLocation(shader_id, "uTexture"), 0);
            glUniform1i(glGetUniformLocation(shader_id, "uHasTexture"), sm->texture_id != 0 ? 1 : 0);

            glBindVertexArray(sm->vao);
            glDrawElements(GL_TRIANGLES, sm->index_count, GL_UNSIGNED_INT, 0);
        }
    }

    glUseProgram(0);
}
