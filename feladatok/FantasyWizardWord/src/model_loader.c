#include "model_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <SDL_image.h>
#include "vector.h"

#define MAX_VERTICES 10000
#define MAX_TEXCOORDS 10000

typedef struct {
    int v_idx;
    int vt_idx;
} FaceIndex;

Model model_load_obj(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Failed to open OBJ file: %s\n", path);
        return (Model){0};
    }

    // --- Fájl elérési út prefix ---
    char base_path[256] = {0};
    strcpy(base_path, path);
    char* last_slash = strrchr(base_path, '/');
    if (last_slash) *(last_slash + 1) = '\0';

    // --- Adattárolók ---
    vec3* positions = malloc(sizeof(vec3) * MAX_VERTICES);
    vec2* texcoords = malloc(sizeof(vec2) * MAX_TEXCOORDS);
    FaceIndex* indices = malloc(sizeof(FaceIndex) * MAX_VERTICES);

    int pos_count = 0;
    int tex_count = 0;
    int index_count = 0;

    char mtl_filename[256] = {0};
    char line[256];

    // --- OBJ beolvasás ---
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            float x, y, z;
            sscanf(line + 2, "%f %f %f", &x, &y, &z);
            positions[pos_count++] = vec3_create(x, y, z);
        } else if (strncmp(line, "vt ", 3) == 0) {
            float u, v;
            sscanf(line + 3, "%f %f", &u, &v);
            texcoords[tex_count++] = (vec2){ u, 1.0f - v }; // flip V
        } else if (strncmp(line, "f ", 2) == 0) {
            int v[3] = {0}, t[3] = {-1, -1, -1 };
            int count = sscanf(line + 2,
                "%d/%d/%*d %d/%d/%*d %d/%d/%*d",
                &v[0], &t[0], &v[1], &t[1], &v[2], &t[2]);
            
            if (count == 6)
            {
                for (int i = 0; i < 3; ++i) {
                    indices[index_count++] = (FaceIndex){ v[i] - 1, t[i] - 1 };
                }
            }
            else{
                fprintf(stderr, "⚠️  Invalid face format or missing UV: %s", line);
            }
            
            
        } else if (strncmp(line, "mtllib ", 7) == 0) {
            sscanf(line + 7, "%s", mtl_filename);
        }
    }
    fclose(file);

    // --- Textúra betöltés ---
    char texture_path[256] = {0};
    if (mtl_filename[0]) {
        char mtl_path[256];
        snprintf(mtl_path, sizeof(mtl_path), "%s%s", base_path, mtl_filename);

        FILE* mtl = fopen(mtl_path, "r");
        if (mtl) {
            while (fgets(line, sizeof(line), mtl)) {
                if (strncmp(line, "map_Kd ", 7) == 0) {
                    char tex_file[128];
                    sscanf(line + 7, "%s", tex_file);
                    snprintf(texture_path, sizeof(texture_path), "%s%s", base_path, tex_file);
                    break;
                }
            }
            fclose(mtl);
        } else {
            fprintf(stderr, "Warning: could not open MTL file %s\n", mtl_path);
        }
    }

    // --- Vertex + UV buffer ---
    float* vertex_data = malloc(sizeof(float) * index_count * 5); // 3 pos + 2 uv
    for (int i = 0; i < index_count; ++i) {
        vec3 pos = positions[indices[i].v_idx];
        vec2 uv = (indices[i].vt_idx >= 0) ? texcoords[indices[i].vt_idx] : (vec2){0.0f, 0.0f};

        vertex_data[i * 5 + 0] = pos.x;
        vertex_data[i * 5 + 1] = pos.y;
        vertex_data[i * 5 + 2] = pos.z;
        vertex_data[i * 5 + 3] = uv.x;
        vertex_data[i * 5 + 4] = uv.y;
    }

    // --- OpenGL objektumok ---
    Model m = {0};
    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);

    glBindVertexArray(m.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * index_count * 5, vertex_data, GL_STATIC_DRAW);

    // pozíció
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    // UV
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

    // --- Textúra betöltése ---
    if (texture_path[0]) {
        SDL_Surface* surface = IMG_Load(texture_path);
        if (!surface) {
            printf("[INFO] model_load_obj: no texture found at %s (ignored, instance texture may be used)\n", texture_path);
        }
         else {
            glGenTextures(1, &m.texture);
            glBindTexture(GL_TEXTURE_2D, m.texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
                         surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
                         GL_UNSIGNED_BYTE, surface->pixels);
            glGenerateMipmap(GL_TEXTURE_2D);
            SDL_FreeSurface(surface);
        }
    }

    glBindVertexArray(0);

    // --- Végső adatok ---
    m.vertex_count = index_count;
    m.shader_program = 0;

    // --- Takarítás ---
    free(positions);
    free(texcoords);
    free(indices);
    free(vertex_data);

    return m;
}
