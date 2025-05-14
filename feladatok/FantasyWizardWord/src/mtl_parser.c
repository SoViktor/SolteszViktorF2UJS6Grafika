#include "mtl_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_MATERIALS 16

MTLMaterialList mtl_parse_file(const char* mtl_path) {
    MTLMaterialList list;
    list.materials = (MTLMaterial*)malloc(sizeof(MTLMaterial) * MAX_MATERIALS);
    list.count = 0;

    FILE* file = fopen(mtl_path, "r");
    if (!file) {
        printf("[MTL] Failed to open: %s\n", mtl_path);
        return list;
    }

    char line[512];
    MTLMaterial current = {"", ""};
    bool in_material = false;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "newmtl ", 7) == 0) {
            if (in_material && list.count < MAX_MATERIALS) {
                list.materials[list.count++] = current;
            }
            sscanf(line + 7, "%63s", current.name);
            current.texture_file[0] = '\0';
            in_material = true;
        } else if (strncmp(line, "map_Kd ", 7) == 0) {
            sscanf(line + 7, "%127s", current.texture_file);
        }
    }

    if (in_material && list.count < MAX_MATERIALS) {
        list.materials[list.count++] = current;
    }

    fclose(file);
    return list;
}

bool find_material_texture(const MTLMaterialList* list, const char* name, char* out_texture_file) {
    for (int i = 0; i < list->count; ++i) {
        if (strcmp(list->materials[i].name, name) == 0) {
            strncpy(out_texture_file, list->materials[i].texture_file, 127);
            out_texture_file[127] = '\0'; // biztosítjuk, hogy null-terminált
            return true;
        }
    }
    return false;
}


void mtl_free_material_list(MTLMaterialList list) {
    if (list.materials) {
        free(list.materials);
    }
}
