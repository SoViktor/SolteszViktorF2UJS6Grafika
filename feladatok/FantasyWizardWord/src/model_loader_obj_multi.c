
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model_loader_obj_multi.h"
#include "texture_loader.h"
#include "mtl_parser.h"
#include <GL/glew.h>

#define MAX_VERTICES 1000000
#define MAX_INDICES  2000000
#define MAX_LINE_LENGTH 1024

typedef struct {
    float* vertices;
    unsigned int* indices;
    int vertex_count;
    int index_count;
    int texture_id;
    char material_name[128];
} RawSubmesh;

static RawSubmesh* submeshes = NULL;
static int submesh_count = 0;

static float positions[MAX_VERTICES][3];
static float texcoords[MAX_VERTICES][2];
static int position_count = 0;
static int texcoord_count = 0;

static int current_submesh = -1;

static void start_new_submesh(const char* material_name, int texture_id) {
    submeshes = realloc(submeshes, sizeof(RawSubmesh) * (submesh_count + 1));
    RawSubmesh* sm = &submeshes[submesh_count++];
    memset(sm, 0, sizeof(RawSubmesh));
    sm->vertices = malloc(sizeof(float) * MAX_VERTICES * 5);
    sm->indices = malloc(sizeof(unsigned int) * MAX_INDICES);
    sm->texture_id = texture_id;
    strncpy(sm->material_name, material_name, 127);
    current_submesh = submesh_count - 1;
}

static void parse_face_line(const char* line) {
    unsigned int vi[3], ti[3], ni[3];
    int parsed = sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u",
                        &vi[0], &ti[0], &ni[0],
                        &vi[1], &ti[1], &ni[1],
                        &vi[2], &ti[2], &ni[2]);
    if (parsed != 9) {
        printf("[OBJ_MULTI] SKIP line (parsed = %d): %s", parsed, line);
        return;
    }

    RawSubmesh* sm = &submeshes[current_submesh];
    for (int i = 0; i < 3; i++) {
        int pi = vi[i] - 1;
        int tii = ti[i] - 1;
        sm->vertices[sm->vertex_count * 5 + 0] = positions[pi][0];
        sm->vertices[sm->vertex_count * 5 + 1] = positions[pi][1];
        sm->vertices[sm->vertex_count * 5 + 2] = positions[pi][2];
        sm->vertices[sm->vertex_count * 5 + 3] = texcoords[tii][0];
        sm->vertices[sm->vertex_count * 5 + 4] = texcoords[tii][1];
        sm->indices[sm->index_count++] = sm->vertex_count++;
    }
}

MultiModel* load_obj_multi(const char* obj_path, const char* texture_folder) {

    FILE* file = fopen(obj_path, "r");
    if (!file) {
        fprintf(stderr, "Failed to open OBJ: %s\n", obj_path);
        return NULL;
    }

    char mtl_path[512];
    strncpy(mtl_path, obj_path, sizeof(mtl_path));
    char* dot = strrchr(mtl_path, '.');
    if (dot) {
        strcpy(dot, ".mtl");  // lecseréljük a kiterjesztést
    } else {
        strcat(mtl_path, ".mtl");
    }


    // Anyagok beolvasása
    MTLMaterialList materials = mtl_parse_file(mtl_path);

    if (materials.count == 0) {
        printf("[WARN] No materials found in MTL file: %s\n", mtl_path);
    }
    
 
    submeshes = NULL;
    submesh_count = 0;
    position_count = 0;
    texcoord_count = 0;

    char line[MAX_LINE_LENGTH];
    char current_mtl[128] = "";

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            sscanf(line + 2, "%f %f %f",
                   &positions[position_count][0],
                   &positions[position_count][1],
                   &positions[position_count][2]);
            position_count++;
        } else if (strncmp(line, "vt ", 3) == 0) {
            sscanf(line + 3, "%f %f",
                   &texcoords[texcoord_count][0],
                   &texcoords[texcoord_count][1]);
            texcoord_count++;
        } else if (strncmp(line, "usemtl ", 7) == 0) {
            sscanf(line + 7, "%s", current_mtl);


            char texture_name[256];
            if (!find_material_texture(&materials, current_mtl, texture_name)) {
                printf("[WARN] Material '%s' not found in .mtl file, using default texture\n", current_mtl);
                snprintf(texture_name, sizeof(texture_name), "%s.png", current_mtl); // fallback
            }
            
            // Textúra betöltése
            char texture_path[512];
            snprintf(texture_path, sizeof(texture_path), "%s/%s", texture_folder, texture_name);
            int tex_id = load_texture(texture_path);
            if (tex_id == 0) {
                printf("[WARN] Failed to load texture: %s\n", texture_path);
                tex_id = load_texture("assets/textures/default.png");
            }

            printf("[MTL] usemtl %s -> %s\n", current_mtl, texture_path);

            // Új submesh indítása
            start_new_submesh(current_mtl, tex_id);
            
        } else if (strncmp(line, "f ", 2) == 0) {
            parse_face_line(line);
        }
    }

    fclose(file);

    MultiModel* model = malloc(sizeof(MultiModel));
    model->submesh_count = submesh_count;
    model->submeshes = malloc(sizeof(Submesh) * submesh_count);

    for (int i = 0; i < submesh_count; i++) {
        RawSubmesh* sm = &submeshes[i];
        glGenVertexArrays(1, &model->submeshes[i].vao);
        glBindVertexArray(model->submeshes[i].vao);
        
        glGenBuffers(1, &model->submeshes[i].vbo);
        glBindBuffer(GL_ARRAY_BUFFER, model->submeshes[i].vbo);
        glBufferData(GL_ARRAY_BUFFER, sm->vertex_count * 5 * sizeof(float), sm->vertices, GL_STATIC_DRAW);
        
        glGenBuffers(1, &model->submeshes[i].ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->submeshes[i].ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sm->index_count * sizeof(unsigned int), sm->indices, GL_STATIC_DRAW);
        
        // attribútum beállítás: pozíció (3 float), textúra (2 float)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0);
        
        model->submeshes[i].index_count = sm->index_count;
        model->submeshes[i].texture_id = sm->texture_id;
        }

    free(submeshes); // struktúrák felszabadítása, de a pointereket megtartjuk a model-ben
    mtl_free_material_list(materials);


    return model;
}
