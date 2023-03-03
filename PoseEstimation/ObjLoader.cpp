#include "ObjLoader.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

void ObjLoader::Load(const std::string& path, std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
        throw std::exception((warn + err).c_str());

    vertices.clear();
    indices.clear();

    std::unordered_map<Vertex, uint32_t> uniqueVertices;
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};

            if (index.vertex_index >= 0)
            {
                vertex.Position =
                {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                };

                auto colorIndex = 3 * index.vertex_index + 2;
                if (colorIndex < attrib.colors.size())
                {
                    vertex.Color =
                    {
                        attrib.colors[colorIndex - 2],
                        attrib.colors[colorIndex - 1],
                        attrib.colors[colorIndex - 0],
                    };
                }
                else
                {
                    vertex.Color = { 200.f, 200.f, 200.f }; // default color
                }
            }

            if (index.normal_index >= 0)
            {
                vertex.Normal =
                {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                };
            }

            if (index.texcoord_index >= 0)
            {
                vertex.Texcoord =
                {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1],
                };
            }

            if (!uniqueVertices.contains(vertex))
            {
                uniqueVertices[vertex] = static_cast<GLuint>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }
}