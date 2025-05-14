#include "shader.h"
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>


static unsigned int current_shader = 0;

static char* load_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) return NULL;
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);
    char* buffer = (char*)malloc(length + 1);
    if (!buffer) return NULL;
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

static unsigned int compile_shader(unsigned int type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, NULL, info);
        printf("[SHADER ERROR] %s\n", info);
    }

    return shader;
}

unsigned int shader_create_from_files(const char* vert_path, const char* frag_path) {
    char* vertex_src = load_file(vert_path);
    char* fragment_src = load_file(frag_path);
    if (!vertex_src || !fragment_src) {
        printf("[SHADER] Failed to load shader files.\n");
        return 0;
    }

    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_src);
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_src);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(program, 512, NULL, info);
        printf("[SHADER LINK ERROR] %s\n", info);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    free(vertex_src);
    free(fragment_src);
    return program;
}

unsigned int shader_get_active(void) {
    return current_shader;
}

void shader_set_active(unsigned int shader_id) {
    current_shader = shader_id;
}
