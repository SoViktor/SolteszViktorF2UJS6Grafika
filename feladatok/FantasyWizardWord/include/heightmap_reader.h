#pragma once

float* read_heightmap(const char* filename, int* out_width, int* out_height);

float get_height_at(float x, float z); // új
