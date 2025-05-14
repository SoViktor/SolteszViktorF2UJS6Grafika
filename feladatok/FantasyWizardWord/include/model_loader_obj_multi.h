#ifndef MODEL_LOADER_OBJ_MULTI_H
#define MODEL_LOADER_OBJ_MULTI_H

#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

// Multi-material OBJ modell betoltese (.obj + .mtl + diffuse texturek)
// A base_texture_dir azt a mappat adja meg, ahol a mtl-ben levo texture fajlokat keressuk
MultiModel* load_obj_multi(const char* obj_path, const char* texture_folder);




// Felszabaditas
void free_obj_multi(MultiModel* model);

#ifdef __cplusplus
}
#endif

#endif // MODEL_LOADER_OBJ_MULTI_H
