#pragma once
unsigned int shader_create_from_files(const char* vert_path, const char* frag_path);
unsigned int shader_get_active(void);
void shader_set_active(unsigned int shader_id);
