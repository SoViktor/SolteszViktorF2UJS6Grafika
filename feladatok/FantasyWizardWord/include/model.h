#ifndef MODEL_H
#define MODEL_H

typedef struct {
    unsigned int vao;
    unsigned int vbo;
    unsigned int tbo;
    unsigned int texture;
    unsigned int shader_program;
    int vertex_count;
} Model;


// Több textúrás modell (MultiModel)
typedef struct {
    unsigned int vao;
    unsigned int vbo;
    unsigned int shader_program;
    int vertex_count;
    int material_count;

    struct {
        int start_index;      // a glDrawArrays offset
        int count;            // hány vertex tartozik ehhez az anyaghoz
        unsigned int texture; // OpenGL texture ID
    } materials[8];           // max 8 anyag per modell
} MultiModel;

#endif // MODEL_H
