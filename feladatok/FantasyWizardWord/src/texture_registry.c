#include "texture_registry.h"
#include <SDL_image.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TEXTURES 128

typedef struct {
    char path[256];
    GLuint texture_id;
} TextureEntry;

static TextureEntry textures[MAX_TEXTURES];
static int texture_count = 0;

GLuint texture_registry_get_or_load(const char* path) {
    // már be van töltve?
    for (int i = 0; i < texture_count; ++i) {
        if (strcmp(textures[i].path, path) == 0) {
            return textures[i].texture_id;
        }
    }

    // új betöltése
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        fprintf(stderr, "Failed to load texture '%s': %s\n", path, IMG_GetError());
        return 0;
    }

    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
                 surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_FreeSurface(surface);

    // eltároljuk
    if (texture_count < MAX_TEXTURES) {
        strncpy(textures[texture_count].path, path, 255);
        textures[texture_count].texture_id = tex_id;
        texture_count++;
    } else {
        fprintf(stderr, "⚠️ Too many textures loaded (limit: %d)\n", MAX_TEXTURES);
    }

    return tex_id;
}

void texture_registry_shutdown(void) {
    for (int i = 0; i < texture_count; ++i) {
        glDeleteTextures(1, &textures[i].texture_id);
    }
    texture_count = 0;
}
