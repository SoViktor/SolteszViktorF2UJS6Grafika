#ifndef MODEL_LOADER_OBJ_TEXTURED_H
#define MODEL_LOADER_OBJ_TEXTURED_H

#include "model.h" // A Model* típus miatt, ezt már definiáltnak feltételezzük

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Textúrázott OBJ modell betöltése.
 * A modell cache-elhető fájlnév alapján, de textúra külön választható (pl. színezett kristály variánsokhoz).
 *
 * @param obj_path        Az .obj fájl elérési útvonala.
 * @param mtl_path        A .mtl fájl elérési útvonala (lehet NULL, ha nem használjuk).
 * @param texture_path    A textúra elérési útvonala (PNG, JPG stb.).
 * @return                A betöltött Model* struktúra, vagy NULL hiba esetén.
 */
Model* load_obj_textured(const char* obj_path, const char* mtl_path, const char* texture_path);

#ifdef __cplusplus
}
#endif

#endif // MODEL_LOADER_OBJ_TEXTURED_H
