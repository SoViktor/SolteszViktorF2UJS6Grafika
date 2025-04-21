#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include "model_registry.h"

bool map_loader_load(const char* path, ModelInstance** out_instances, int* out_count);
void map_loader_free(ModelInstance* instances);

#endif
