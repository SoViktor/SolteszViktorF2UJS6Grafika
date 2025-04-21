#include "model_registry.h"
#include "model_loader.h"
#include "shader.h"
#include <stdio.h>
#include <stdbool.h>
#include <GL/glew.h>

static Model g_models[MODEL_COUNT];

bool model_registry_init(void) {
    unsigned int shader_program = shader_load("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

    g_models[MODEL_WIZARD] = model_load_obj("assets/models/Wizard.obj");
    g_models[MODEL_WIZARD].shader_program = shader_program;

    g_models[MODEL_CRYSTAL1] = model_load_obj("assets/models/Crystals/Crystal1.obj");
    g_models[MODEL_CRYSTAL1].shader_program = shader_program;

    g_models[MODEL_CRYSTAL2] = model_load_obj("assets/models/Crystals/Crystal2.obj");
    g_models[MODEL_CRYSTAL2].shader_program = shader_program;

    g_models[MODEL_CRYSTAL3] = model_load_obj("assets/models/Crystals/Crystal3.obj");
    g_models[MODEL_CRYSTAL3].shader_program = shader_program;

    g_models[MODEL_CRYSTAL4] = model_load_obj("assets/models/Crystals/Crystal4.obj");
    g_models[MODEL_CRYSTAL4].shader_program = shader_program;

    g_models[MODEL_CRYSTAL5] = model_load_obj("assets/models/Crystals/Crystal5.obj");
    g_models[MODEL_CRYSTAL5].shader_program = shader_program;

    g_models[MODEL_CRYSTAL6] = model_load_obj("assets/models/Crystals/Crystal6.obj");
    g_models[MODEL_CRYSTAL6].shader_program = shader_program;

    g_models[MODEL_CRYSTAL7] = model_load_obj("assets/models/Crystals/Crystal7.obj");
    g_models[MODEL_CRYSTAL7].shader_program = shader_program;

    g_models[MODEL_CRYSTAL8] = model_load_obj("assets/models/Crystals/Crystal8.obj");
    g_models[MODEL_CRYSTAL8].shader_program = shader_program;

    g_models[MODEL_CRYSTAL9] = model_load_obj("assets/models/Crystals/Crystal9.obj");
    g_models[MODEL_CRYSTAL9].shader_program = shader_program;

    g_models[MODEL_CRYSTAL10] = model_load_obj("assets/models/Crystals/Crystal10.obj");
    g_models[MODEL_CRYSTAL10].shader_program = shader_program;

    g_models[MODEL_CRYSTAL11] = model_load_obj("assets/models/Crystals/Crystal11.obj");
    g_models[MODEL_CRYSTAL11].shader_program = shader_program;

    g_models[MODEL_CRYSTAL12] = model_load_obj("assets/models/Crystals/Crystal12.obj");
    g_models[MODEL_CRYSTAL12].shader_program = shader_program;
    
    for (int i = 0; i < MODEL_COUNT; ++i) {
        if (g_models[i].texture == 0) {
            printf("[INFO] model_registry: Model %d has no base texture (instance texture expected)\n", i);
        }
    }
    





    return true;
}
void model_registry_shutdown(void) {
    for (int i = 0; i < MODEL_COUNT; ++i) {
        glDeleteVertexArrays(1, &g_models[i].vao);
        glDeleteBuffers(1, &g_models[i].vbo);
        if (g_models[i].texture) {
            glDeleteTextures(1, &g_models[i].texture);
        }
    }
}

Model* model_registry_get(ModelType type) {
    if (type >= 0 && type < MODEL_COUNT) {
        return &g_models[type];
    }
    return NULL;
}
