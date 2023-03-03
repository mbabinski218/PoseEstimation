#include "Mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

Mesh::Mesh(const std::string& modelObjPath, const char* vertexFilePath, const char* fragmentFilePath, const ImVec2& modelSize) :
	ModelShader(std::make_unique<Shader>(vertexFilePath, fragmentFilePath)),
	FBuffer(std::make_unique<FrameBuffer>(modelSize.x, modelSize.y)),
	ModelMatrix(glm::mat4(1.f)),
	Position(glm::vec3(0, 0, 3)),
	Origin(glm::vec3(0.f)),
	Rotation(glm::vec3(0.f)), Scale(glm::vec3(1.f))
{
	auto path = std::filesystem::current_path().string() + modelObjPath;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) 
        throw std::exception((warn + err).c_str());

    Vertices.clear();
    Indices.clear();

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
                    vertex.Color = { 1.f, 1.f, 1.f }; // default color
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

            if (uniqueVertices.count(vertex) == 0) 
            {
                uniqueVertices[vertex] = static_cast<GLuint>(Vertices.size());
                Vertices.push_back(vertex);
            }
            Indices.push_back(uniqueVertices[vertex]);
        }
    }

    VIBuffer = std::make_unique<VertexIndexBuffer>(Vertices, Indices);
}

void* Mesh::GetTexture() const
{
	return FBuffer->GetTexture();
}

void Mesh::Render() const
{
	ModelShader->Bind();
	FBuffer->Bind();
	VIBuffer->Bind();

	VIBuffer->Draw();

	ModelShader->Unbind();
	VIBuffer->Unbind();
	FBuffer->Unbind();
}

glm::quat get_direction()
{
    float mPitch = 0.0f;
    float mYaw = 0.0f;
    return glm::quat(glm::vec3(-mPitch, -mYaw, 0.0f));
}

glm::vec3 get_forward()
{
    glm::vec3 cForward = { 0.0f, 0.0f, -1.0f };
    return glm::rotate(get_direction(),  cForward);
}

glm::mat4 Matrix()
{
    glm::vec3 mFocus = { 0.0f, 0.0f, 0.0f };
    glm::mat4 mViewMatrix;
    glm::vec3(0, 0, 3) = mFocus - get_forward() * 5.0f;

    glm::quat orientation = get_direction();
    mViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 3)) * glm::toMat4(orientation);
    mViewMatrix = glm::inverse(mViewMatrix);
    return mViewMatrix;
}

void Mesh::Update()
{
    ModelShader->Bind();

    glm::mat4 model{ 1.0f };
    ModelShader->SetMat4(model, "model");
    ModelShader->SetMat4(Matrix(), "view");
    ModelShader->SetMat4(glm::perspective(45.0f, 1.3f, 0.1f, 100.0f), "projection");
    ModelShader->SetVec3(glm::vec3(0, 0, 3), "camPos");

	ModelShader->SetVec3(glm::vec3( 1.5f, 3.5f, 3.0f ), "lightPosition");
	ModelShader->SetVec3(glm::vec3(1.0f, 1.0f, 1.0f) * 100.0f, "lightColor");

	ModelShader->SetVec3(glm::vec3(1.0f, 0.0f, 0.0f), "albedo");
	ModelShader->SetF1(0.2f, "roughness");
	ModelShader->SetF1(0.1f, "metallic");
	ModelShader->SetF1(1.0f, "ao");

    ModelShader->Unbind();
}