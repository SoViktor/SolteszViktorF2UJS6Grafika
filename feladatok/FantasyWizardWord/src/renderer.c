#include "renderer.h"
#include "shader.h"
#include "texture_loader.h"
#include "model.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>

static GLuint vao = 0;
static GLuint vbo = 0;
static GLuint ebo = 0;
static GLuint shader_program = 0;
static GLuint terrain_texture = 0;
static size_t num_indices = 0;
static GLuint model_shader_program = 0;


bool renderer_init(void) {
    printf("[RENDERER] Initialized.\n");
    shader_program = shader_create_from_files("assets/shaders/terrain.vert", "assets/shaders/terrain.frag");
    if (shader_program == 0) return false;

    terrain_texture = load_texture("assets/textures/Terrain/terrain_atlas.png");
    if (terrain_texture == 0) return false;

    printf("[DEBUG] terrain_texture ID = %u\n", terrain_texture);


    return true;
}

void renderer_upload_mesh(const float* vertices, size_t vertex_count,
                          const unsigned int* indices, size_t index_count) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // pozíció

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // UV

    glBindVertexArray(0);
    num_indices = index_count;
}

void renderer_render(const float view[16], const float proj[16]) {
    if (vao == 0 || shader_program == 0 || terrain_texture == 0) return;
    glUseProgram(shader_program);
    shader_set_active(shader_program);

    GLuint shader_id = shader_program;


    glUniformMatrix4fv(glGetUniformLocation(shader_program, "uView"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "uProj"), 1, GL_FALSE, proj);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, terrain_texture);
    glUniform1i(glGetUniformLocation(shader_program, "uTexture"), 0);

    glUniform1i(glGetUniformLocation(shader_id, "uHasTexture"), 1);
    glUniform3f(glGetUniformLocation(shader_id, "uColor"), 1.0f, 1.0f, 1.0f); // nem használt, de muszáj
    glUniform1i(glGetUniformLocation(shader_id, "uTexture"), 0); // ha textúrázva van a terrain


    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
}

void renderer_render_model(const Model* model, const float view[16], const float proj[16]) {
    if (!model || model->vao == 0 || model->index_count == 0) return;

    glUseProgram(shader_get_active()); // feltételezve hogy shader aktív

    GLuint shader_id = shader_get_active();
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "uView"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "uProj"), 1, GL_FALSE, proj);

    glUniform1i(glGetUniformLocation(shader_id, "uHasTexture"), model->has_texture ? 1 : 0);
    glUniform3f(glGetUniformLocation(shader_id, "uColor"),
                model->color[0], model->color[1], model->color[2]);
    glUniform1f(glGetUniformLocation(shader_id, "uAlpha"), model->alpha);
    glUniform1i(glGetUniformLocation(shader_id, "uUseLighting"), model->use_lighting ? 1 : 0);

    if (model->has_texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model->texture_id);
        glUniform1i(glGetUniformLocation(shader_id, "uTexture"), 0);
    }

    glBindVertexArray(model->vao);
    glDrawElements(GL_TRIANGLES, model->index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (model->has_texture) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glUseProgram(0);

}

bool renderer_model_init(void) {
    model_shader_program = shader_create_from_files("assets/shaders/model.vert", "assets/shaders/model.frag");
    if (model_shader_program == 0) {
        printf("[RENDERER] Failed to load model shader.\n");
        return false;
    }
    return true;
}

GLuint renderer_get_model_shader(void) {
    return model_shader_program;
}



void renderer_shutdown(void) {
    if (ebo) glDeleteBuffers(1, &ebo);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
    if (shader_program) glDeleteProgram(shader_program);
    if (terrain_texture) glDeleteTextures(1, &terrain_texture);
    vao = vbo = ebo = shader_program = terrain_texture = 0;
}
