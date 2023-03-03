#include "Mesh.hpp"

Mesh::Mesh(const std::string& modelObjPath, const char* vertexFilePath, const char* fragmentFilePath, const ImVec2& modelSize) :
	ModelShader(std::make_unique<Shader>(vertexFilePath, fragmentFilePath)),
	FBuffer(std::make_unique<FrameBuffer>(modelSize.x, modelSize.y)),
	ViewMatrix(glm::mat4(0.f))
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

void Mesh::UpdateViewMatrix()
{
    glm::vec3 mFocus = { 0.0f, 0.0f, 0.0f };
    glm::vec3(0, 0, 3) = mFocus - get_forward() * 5.0f;

    glm::quat orientation = get_direction();
    ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 3)) * glm::toMat4(orientation);
    ViewMatrix = glm::inverse(ViewMatrix);
}

void Mesh::Update()
{
    ModelShader->Bind();

    UpdateViewMatrix();

    glm::mat4 model{ 1.0f };
    ModelShader->SetMat4(model, "model");
    ModelShader->SetMat4(ViewMatrix, "view");
    ModelShader->SetMat4(glm::perspective(45.0f, 1.3f, 0.1f, 100.0f), "projection");
    ModelShader->SetVec3(glm::vec3(0, 0, 3), "camPos");

	//ModelShader->SetVec3(glm::vec3( 1.5f, 3.5f, 3.0f ), "lightPosition");
	//ModelShader->SetVec3(glm::vec3(1.0f, 1.0f, 1.0f) * 100.0f, "lightColor");

	//ModelShader->SetVec3(glm::vec3(1.0f, 0.0f, 0.0f), "albedo");
	//ModelShader->SetF1(0.2f, "roughness");
	//ModelShader->SetF1(0.1f, "metallic");
	//ModelShader->SetF1(1.0f, "ao");

    ModelShader->Unbind();
}
