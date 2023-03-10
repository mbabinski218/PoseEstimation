#include "Mesh.hpp"

Mesh::Mesh(const std::string& modelObjPath, const char* vertexFilePath, const char* fragmentFilePath) :
	ModelShader(std::make_unique<Shader>(vertexFilePath, fragmentFilePath)),
	FBuffer(std::make_unique<FrameBuffer>(1280, 720)),
	ViewMatrix(glm::mat4(0.0f)),
	ModelMatrix(glm::mat4(1.0f)),
	Forward(glm::vec3(0.0f, 0.0f, -0.5f)),
	Aspect(1.3f), Near(0.1f), Far(100.0f),
	RotationSpeed(100.0f),
	CurrentPos(glm::vec2(0.0f, 0.0f))
{
	Reset();

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
	const auto pitchInRadians = glm::radians<float>(static_cast<float>(Pitch));
	const auto yawInRadians = glm::radians<float>(static_cast<float>(Yaw));
	const auto rollInRadians = glm::radians<float>(static_cast<float>(Roll)) + glm::radians(180.0f);

	return glm::quat
	{
		glm::vec3{-pitchInRadians, -yawInRadians, rollInRadians}
	};
}

glm::vec3 Mesh::GetForward() const
{
    return glm::rotate(GetDirection(), Forward);
}

void Mesh::UpdateViewMatrix()
{
    const auto position = Focus + glm::vec3(0.0f, 0.873f, 0.0f) - GetForward() * Distance;
    const glm::quat orientation = GetDirection();

    ViewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
    ViewMatrix = glm::inverse(ViewMatrix);
}

void Mesh::UpdateModel()
{
	ModelMatrix = glm::mat4(1.0f);
	//ModelMatrix = glm::translate(ModelMatrix, Origin);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.y), glm::vec3(0.0f, -1.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	//ModelMatrix = glm::translate(ModelMatrix, Position - Origin);
	ModelMatrix = glm::scale(ModelMatrix, Scale);
}

void Mesh::Update(const ImVec2& screenSize)
{
	ModelShader->Bind();

	// Light
	Light.SetPosition(LightPosition);
	Light.SendToShader(*ModelShader);

	// Camera
	Aspect = screenSize.x / screenSize.y;
	UpdateViewMatrix();
    ModelShader->SetMat4(ViewMatrix, "ViewMatrix");
    ModelShader->SetMat4(glm::perspective(glm::radians<float>(static_cast<float>(Fov)), Aspect, Near, Far), "ProjectionMatrix");
    ModelShader->SetVec3(glm::vec3(0.0f, 0.0f, 3.0f), "cameraPos");

	// Model
	UpdateModel();
    ModelShader->SetMat4(ModelMatrix, "ModelMatrix"); 

	// Material
	Material.SendToShader(*ModelShader);

	ModelShader->Unbind();
}

void Mesh::OnMouseMove(const double& x, const double& y, const Button& button)
{
	const auto pos = glm::vec2(x, y);

	if (button == Button::RIGHT)
	{
		const auto delta = (pos - CurrentPos) * 0.004f;
		const auto temp = Rotation.y + delta.x * RotationSpeed;

		if (temp >= 180)
			Rotation.y = 180;
		else if (temp <= -180)
			Rotation.y = -180;
		else
			Rotation.y = temp;
	}

	CurrentPos = pos;
}

void Mesh::Reset()
{
	ResetFov();
	ResetDistance();
	ResetPitch();
	ResetYaw();
	ResetRoll();
	ResetFocus();
	ResetLightPosition();
	ResetRotation();
	ResetScale();
}
