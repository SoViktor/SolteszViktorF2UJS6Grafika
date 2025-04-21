#include "model.h"
#include <GL/glew.h>

void draw_multimodel(MultiModel* model, float model_matrix[16], float view_matrix[16], float projection_matrix[16]) {
    if (!model || model->material_count == 0) return;

    glBindVertexArray(model->vao);

    for (int i = 0; i < model->material_count; ++i) {
        glUseProgram(model->shader_program);

        glUniformMatrix4fv(glGetUniformLocation(model->shader_program, "model"), 1, GL_FALSE, model_matrix);
        glUniformMatrix4fv(glGetUniformLocation(model->shader_program, "view"), 1, GL_FALSE, view_matrix);
        glUniformMatrix4fv(glGetUniformLocation(model->shader_program, "projection"), 1, GL_FALSE, projection_matrix);

        glBindTexture(GL_TEXTURE_2D, model->materials[i].texture);
        glDrawArrays(GL_TRIANGLES, model->materials[i].start_index, model->materials[i].count);
    }

    glBindVertexArray(0);
    glUseProgram(0);
} 
