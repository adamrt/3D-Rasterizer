#pragma once

#include <vector>
#include "../lib/tinyobjloader/tiny_obj_loader.h"
#include "vector4.h"

struct Mesh {
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;
tinyobj::attrib_t attrib;

Mesh (std::string filePath);
Vector4 rotation = {0, 0, 0, 0};
Vector4 translation = {0, 0, 0, 0};
Vector4 scale = {1, 1, 1, 0};
};
