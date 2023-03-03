#include "Mesh.hpp"

Mesh::Mesh(const std::string& modelObjPath, const char* vertexFilePath, const char* fragmentFilePath) :
	ModelShader(std::make_unique<Shader>(vertexFilePath, fragmentFilePath)),
	FBuffer(std::make_unique<FrameBuffer>(1280, 720)),
	ViewMatrix(glm::mat4(0.f)),
	Focus(glm::vec3( 0.0f, 0.0f, 0.0f )),
	Position(glm::vec3( 0.0f, 0.0f, 0.0f )),
	Fov(45.0f), Aspect(1.3f), Near(0.1f), Far(100.0f),
	Pitch(0.0f), Yaw(0.0f), Roll(0.0f),
	Distance(5.0f)
{
	const auto path = std::filesystem::current_path().string() + modelObjPath;
    ObjLoader::Load(path, Vertices, Indices);
    
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

glm::quat Mesh::GetDirection() const
{
	return glm::quat
	{
		glm::vec3{-Pitch, -Yaw, Roll}
	};
}

glm::vec3 Mesh::GetForward() const
{
    glm::vec3 cForward = { 0.0f, 0.0f, -1.0f };
    return glm::rotate(GetDirection(),  cForward);
}

void Mesh::UpdateViewMatrix()
{
    Position = Focus - GetForward() * Distance;
    const glm::quat orientation = GetDirection();

    ViewMatrix = glm::translate(glm::mat4(1.0f), Position) * glm::toMat4(orientation);
    ViewMatrix = glm::inverse(ViewMatrix);
}

void Mesh::Update()
{
    ModelShader->Bind();

    UpdateViewMatrix();

    ModelShader->SetMat4(glm::mat4(1.0f), "model");
    ModelShader->SetMat4(ViewMatrix, "view");
    ModelShader->SetMat4(glm::perspective(Fov, Aspect, Near, Far), "projection");
    ModelShader->SetVec3(glm::vec3(0, 0, 3), "camPos");

	//ModelShader->SetVec3(glm::vec3( 1.5f, 3.5f, 3.0f ), "lightPosition");
	//ModelShader->SetVec3(glm::vec3(1.0f, 1.0f, 1.0f) * 100.0f, "lightColor");

	//ModelShader->SetVec3(glm::vec3(1.0f, 0.0f, 0.0f), "albedo");
	//ModelShader->SetF1(0.2f, "roughness");
	//ModelShader->SetF1(0.1f, "metallic");
	//ModelShader->SetF1(1.0f, "ao");

    ModelShader->Unbind();
}
