#include "model_loader_multi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <SDL_image.h>
#include "vector.h"

#define MAX_VERTICES 100000
#define MAX_TEXCOORDS 100000
#define MAX_MATERIALS 8

typedef struct {
    char name[64];
    char texture_file[256];
    GLuint texture_id;
} MaterialInfo;

typedef struct {
    int v_idx;
    int vt_idx;
    int material_idx;
} FaceIndex;

GLuint load_texture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        fprintf(stderr, "Failed to load texture %s: %s\n", path, IMG_GetError());
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
                 surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_FreeSurface(surface);
    return tex;
}

MultiModel model_load_obj_multi(const char* path) {
    MultiModel model = {0};
    vec3* positions = malloc(sizeof(vec3) * MAX_VERTICES);
    vec2* texcoords = malloc(sizeof(vec2) * MAX_TEXCOORDS);
    FaceIndex* indices = malloc(sizeof(FaceIndex) * MAX_VERTICES);
    MaterialInfo materials[MAX_MATERIALS];
    int pos_count = 0, tex_count = 0, index_count = 0, material_count = 0;

    char base_path[256];
    strcpy(base_path, path);
    char* last_slash = strrchr(base_path, '/');
    if (last_slash) *(last_slash + 1) = '\0';

    FILE* obj = fopen(path, "r");
    if (!obj) {
        fprintf(stderr, "Failed to open OBJ file: %s\n", path);
        return model;
    }

    char mtl_file[256] = {0};
    char line[256];
    int current_material = -1;

    while (fgets(line, sizeof(line), obj)) {
        if (strncmp(line, "v ", 2) == 0) {
            float x, y, z;
            sscanf(line + 2, "%f %f %f", &x, &y, &z);
            positions[pos_count++] = vec3_create(x, y, z);
        } else if (strncmp(line, "vt ", 3) == 0) {
            float u, v;
            sscanf(line + 3, "%f %f", &u, &v);
            texcoords[tex_count++] = vec2_create(u, 1.0f - v);
        } else if (strncmp(line, "f ", 2) == 0) {
            int v[3], t[3];
            sscanf(line + 2, "%d/%d %d/%d %d/%d", &v[0], &t[0], &v[1], &t[1], &v[2], &t[2]);
            for (int i = 0; i < 3; ++i) {
                indices[index_count++] = (FaceIndex){ v[i]-1, t[i]-1, current_material };
            }
        } else if (strncmp(line, "usemtl ", 7) == 0) {
            char mat_name[64];
            sscanf(line + 7, "%s", mat_name);
            for (int i = 0; i < material_count; ++i) {
                if (strcmp(materials[i].name, mat_name) == 0) {
                    current_material = i;
                    break;
                }
            }
        } else if (strncmp(line, "mtllib ", 7) == 0) {
            sscanf(line + 7, "%s", mtl_file);
        }
    }
    fclose(obj);

    if (mtl_file[0]) {
        char full_mtl_path[512];
        snprintf(full_mtl_path, sizeof(full_mtl_path), "%s%s", base_path, mtl_file);
        FILE* mtl = fopen(full_mtl_path, "r");
        if (mtl) {
            char line[256];
            char current_name[64] = "";
            while (fgets(line, sizeof(line), mtl)) {
                if (strncmp(line, "newmtl ", 7) == 0) {
                    sscanf(line + 7, "%s", current_name);
                } else if (strncmp(line, "map_Kd ", 7) == 0) {
                    if (material_count < MAX_MATERIALS) {
                        MaterialInfo* m = &materials[material_count++];
                        strcpy(m->name, current_name);
                        char tex_file[256];
                        sscanf(line + 7, "%s", tex_file);
                        snprintf(m->texture_file, sizeof(m->texture_file), "%s%s", base_path, tex_file);
                        m->texture_id = load_texture(m->texture_file);
                    }
                }
            }
            fclose(mtl);
        }
    }

    float* vertex_data = malloc(sizeof(float) * index_count * 5);
    int offset = 0;
    int ranges[MAX_MATERIALS] = {0};

    for (int i = 0; i < index_count; ++i) {
        vec3 pos = positions[indices[i].v_idx];
        vec2 uv = texcoords[indices[i].vt_idx];

        vertex_data[offset++] = pos.x;
        vertex_data[offset++] = pos.y;
        vertex_data[offset++] = pos.z;
        vertex_data[offset++] = uv.x;
        vertex_data[offset++] = uv.y;
        ranges[indices[i].material_idx]++;
    }

    model.vertex_count = index_count;
    model.material_count = material_count;
    glGenVertexArrays(1, &model.vao);
    glGenBuffers(1, &model.vbo);
    glBindVertexArray(model.vao);
    glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * offset, vertex_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

    glBindVertexArray(0);

    int start = 0;
    for (int i = 0; i < material_count; ++i) {
        model.materials[i].start_index = start;
        model.materials[i].count = ranges[i];
        model.materials[i].texture = materials[i].texture_id;
        start += ranges[i];
    }

    free(positions);
    free(texcoords);
    free(indices);
    free(vertex_data);

    return model;
}
