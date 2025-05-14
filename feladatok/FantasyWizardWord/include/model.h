#ifndef MODEL_H
#define MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Ez a struktúra tartalmazza a betöltött 3D modell összes szükséges OpenGL azonosítóját és adatait
typedef struct Model {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    unsigned int texture_id;
    unsigned int index_count;
    bool has_texture;
    float color[3];
    float alpha;         // <-- új mező
    bool use_lighting;   // új mező, pl. emisszív modellekhez false
} Model;


// Több textúrás (multi-material) modellek támogatása
typedef struct {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    unsigned int texture_id;
    unsigned int index_count;
} Submesh;

typedef struct {
    Submesh* submeshes;
    unsigned int submesh_count;
    float alpha;
    bool use_lighting;
} MultiModel;


#ifdef __cplusplus
}
#endif

#endif // MODEL_H
