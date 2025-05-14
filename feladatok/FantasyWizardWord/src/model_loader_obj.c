// model_loader_obj.c – bővített OBJ parserrel (v/vt/vn támogatás)

#include "model_loader_obj_colored.h"
#include "model_loader_obj_textured.h"
#include "model.h"
#include "texture_loader.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Egyszerű OBJ fájl beolvasás (támogatja a v/vt/vn formátumot is) ---
typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float u, v;
} Vec2;

typedef struct {
    Vec3* positions;
    Vec2* uvs;
    unsigned int* indices;
    size_t vertex_count;
    size_t index_count;
} ParsedOBJ;

static bool parse_obj_file(const char* path, ParsedOBJ* out) {
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("[OBJ] Failed to open: %s\n", path);
        return false;
    }

    Vec3 temp_positions[10000];
    Vec2 temp_uvs[10000];
    unsigned int temp_pos_count = 0, temp_uv_count = 0;

    float* final_vertices = malloc(sizeof(float) * 30000 * 5);
    unsigned int* final_indices = malloc(sizeof(unsigned int) * 30000);
    size_t vcount = 0, icount = 0;

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "v ", 2) == 0) {
            Vec3 v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            temp_positions[temp_pos_count++] = v;
        } else if (strncmp(line, "vt ", 3) == 0) {
            Vec2 uv;
            sscanf(line, "vt %f %f", &uv.u, &uv.v);
            temp_uvs[temp_uv_count++] = uv;
        } else if (strncmp(line, "f ", 2) == 0) {
            unsigned int vi[3], ti[3];
            int parsed = sscanf(line, "f %u/%u/%*u %u/%u/%*u %u/%u/%*u",
                &vi[0], &ti[0], &vi[1], &ti[1], &vi[2], &ti[2]);
            if (parsed == 6) {
                for (int i = 0; i < 3; ++i) {
                    Vec3 pos = temp_positions[vi[i] - 1];
                    Vec2 uv = temp_uvs[ti[i] - 1];
                    final_vertices[vcount * 5 + 0] = pos.x;
                    final_vertices[vcount * 5 + 1] = pos.y;
                    final_vertices[vcount * 5 + 2] = pos.z;
                    final_vertices[vcount * 5 + 3] = uv.u;
                    final_vertices[vcount * 5 + 4] = uv.v;
                    final_indices[icount++] = vcount++;
                }
            }
        }
    }
    fclose(f);

    out->positions = (Vec3*)malloc(sizeof(Vec3) * vcount);
    out->uvs = (Vec2*)malloc(sizeof(Vec2) * vcount);
    out->indices = (unsigned int*)malloc(sizeof(unsigned int) * icount);
    for (size_t i = 0; i < vcount; ++i) {
        out->positions[i].x = final_vertices[i * 5 + 0];
        out->positions[i].y = final_vertices[i * 5 + 1];
        out->positions[i].z = final_vertices[i * 5 + 2];
        out->uvs[i].u = final_vertices[i * 5 + 3];
        out->uvs[i].v = final_vertices[i * 5 + 4];
    }
    memcpy(out->indices, final_indices, sizeof(unsigned int) * icount);
    out->vertex_count = vcount;
    out->index_count = icount;

    free(final_vertices);
    free(final_indices);

    return true;
}

static Model* create_model_from_parsed(const ParsedOBJ* obj, unsigned int texture_id, const float color[3], float alpha, bool use_lighting, bool has_texture) {
    GLuint vao, vbo, ebo;
    size_t stride = has_texture ? 5 : 3;
    float* vertex_data = malloc(sizeof(float) * obj->vertex_count * stride);

    for (size_t i = 0; i < obj->vertex_count; ++i) {
        vertex_data[i * stride + 0] = obj->positions[i].x;
        vertex_data[i * stride + 1] = obj->positions[i].y;
        vertex_data[i * stride + 2] = obj->positions[i].z;
        if (has_texture) {
            vertex_data[i * stride + 3] = obj->uvs[i].u;
            vertex_data[i * stride + 4] = obj->uvs[i].v;
        }
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * obj->vertex_count * stride, vertex_data, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * obj->index_count, obj->indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);

    if (has_texture) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
    }

    glBindVertexArray(0);

    Model* model = malloc(sizeof(Model));
    model->vao = vao;
    model->vbo = vbo;
    model->ebo = ebo;
    model->texture_id = texture_id;
    model->index_count = obj->index_count;
    model->has_texture = has_texture;
    model->color[0] = color ? color[0] : 1.0f;
    model->color[1] = color ? color[1] : 1.0f;
    model->color[2] = color ? color[2] : 1.0f;
    model->alpha = alpha;
    model->use_lighting = use_lighting;

    free(vertex_data);
    return model;
}

Model* load_obj_textured(const char* obj_path, const char* mtl_path_unused, const char* texture_path) {
    (void)mtl_path_unused;
    ParsedOBJ parsed;
    if (!parse_obj_file(obj_path, &parsed)) return NULL;
    GLuint tex_id = load_texture(texture_path);
    return create_model_from_parsed(&parsed, tex_id, NULL, 1.0f, true, true);
}

Model* load_obj_colored(const char* obj_path, const float color[3], float alpha, bool use_lighting) {
    ParsedOBJ parsed;
    if (!parse_obj_file(obj_path, &parsed)) return NULL;
    return create_model_from_parsed(&parsed, 0, color, alpha, use_lighting, false);
}
