#ifndef MODEL_LOADER_OBJ_COLORED_H
#define MODEL_LOADER_OBJ_COLORED_H

#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Szín-alapú OBJ modell betöltése textúra nélkül.
 *
 * @param obj_path      Az .obj fájl elérési útvonala.
 * @param color         RGB szín (0.0 – 1.0 tartományban).
 * @param alpha         Átlátszóság értéke (1.0 = teljesen átlátszatlan, 0.0 = láthatatlan).
 * @param use_lighting  Használjon-e világítást (true), vagy sima színt (false, pl. emisszív hatás).
 * @return              Betöltött modell, vagy NULL hiba esetén.
 */
Model* load_obj_colored(const char* obj_path, const float color[3], float alpha, bool use_lighting);

#ifdef __cplusplus
}
#endif

#endif // MODEL_LOADER_OBJ_COLORED_H
