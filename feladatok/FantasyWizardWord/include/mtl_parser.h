#ifndef MTL_PARSER_H
#define MTL_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Egy anyag neve + diffuse textura (map_Kd)
typedef struct {
    char name[64];
    char texture_file[128]; // csak a fájlnév, nem teljes elérési út
} MTLMaterial;

typedef struct {
    MTLMaterial* materials;
    int count;
} MTLMaterialList;

// .mtl fajl beolvasasa
MTLMaterialList mtl_parse_file(const char* mtl_path);

// Felszabaditas
void mtl_free_material_list(MTLMaterialList list);

bool find_material_texture(const MTLMaterialList* list, const char* name, char* out_texture_file);

#ifdef __cplusplus
}
#endif

#endif // MTL_PARSER_H
