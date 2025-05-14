#pragma once
#include <stddef.h>
#include <stdbool.h>
#include "model.h"

bool renderer_init(void);
void renderer_upload_mesh(const float* vertices, size_t vertex_count,
                          const unsigned int* indices, size_t index_count);
void renderer_render(const float view[16], const float proj[16]);
void renderer_render_model(const Model* model, const float view[16], const float proj[16]);
bool renderer_model_init(void);
unsigned int renderer_get_model_shader(void);
void renderer_shutdown(void);
