#ifndef TEXTURE_REGISTRY_H
#define TEXTURE_REGISTRY_H

#include <GL/glew.h>

GLuint texture_registry_get_or_load(const char* path);
void texture_registry_shutdown(void);

#endif
