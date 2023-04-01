#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) :
	Vertices(std::move(vertices)),
	Indices(std::move(indices)),
	ViewMatrix(glm::mat4(0.0f)),
	ModelMatrix(glm::mat4(1.0f)), Forward(glm::vec3(0.0f, 0.0f, -0.5f)), Aspect(1.3f),
	Near(0.1f),	Far(100.0f),
	RotationSpeed(100.0f),
	CurrentPos(glm::vec2(0.0f, 0.0f))
{
	SetToDefault();
	Buffer = std::make_unique<VertexIndexBuffer>(Vertices, Indices);
}

Mesh::Mesh(const Mesh& copy) :
	Vertices(copy.Vertices),
	Indices(copy.Indices),
	ViewMatrix(copy.ViewMatrix),
	ModelMatrix(copy.ModelMatrix), Forward(copy.Forward), Aspect(copy.Aspect),
	Near(copy.Near), Far(copy.Far),
	RotationSpeed(copy.RotationSpeed),
	CurrentPos(copy.CurrentPos)
{
	SetToDefault();
	Buffer = std::make_unique<VertexIndexBuffer>(Vertices, Indices);
}

void Mesh::Draw() const
{
	Buffer->Bind();
	Buffer->Draw();
	Buffer->Unbind();
}

glm::quat Mesh::GetDirection() const
{
	const auto pitchInRadians = glm::radians<float>(static_cast<float>(Pitch));
	const auto yawInRadians = glm::radians<float>(static_cast<float>(Yaw));
	const auto rollInRadians = glm::radians<float>(static_cast<float>(Roll)) + glm::radians(180.0f);

	return glm::quat{ glm::vec3{-pitchInRadians, -yawInRadians, rollInRadians} };
}

glm::vec3 Mesh::GetForward() const
{
    return rotate(GetDirection(), Forward);
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
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.y), glm::vec3(0.0f, -1.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, Scale);
}

void Mesh::Update(const ImVec2& screenSize, const std::shared_ptr<Shader> shader)
{
	// Light
	Light.SetPosition(LightPosition);
	Light.SendToShader(*shader);

	// Camera
	Aspect = screenSize.x / screenSize.y;
	UpdateViewMatrix();
	shader->SetMat4(ViewMatrix, "ViewMatrix");
	shader->SetMat4(glm::perspective(glm::radians<float>(static_cast<float>(Fov)), Aspect, Near, Far), "ProjectionMatrix");
	shader->SetVec3(glm::vec3(0.0f, 0.0f, 3.0f), "cameraPos");

	// Model
	UpdateModel();
	shader->SetMat4(ModelMatrix, "ModelMatrix");

	// Material
	Material.SendToShader(*shader);
}

void Mesh::OnMouseMove(double x, double y, const Button& button)
{
	const auto pos = glm::vec2(x, y);

	if (button == Button::RIGHT)
	{
		const auto delta = (pos - CurrentPos) * 0.004f;
		const auto temp = Rotation.y + delta.x * RotationSpeed;

		if (temp >= 180.0f)
			Rotation.y = 180.0f;
		else if (temp <= -180.0f)
			Rotation.y = -180.0f;
		else
			Rotation.y = temp;
	}

	CurrentPos = pos;
}

void Mesh::SetToDefault()
{
	DefaultFov();
	DefaultDistance();
	DefaultPitch();
	DefaultYaw();
	DefaultRoll();
	DefaultFocus();
	DefaultLightPosition();
	DefaultRotation();
	DefaultScale();
}
