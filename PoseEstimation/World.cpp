#include "World.hpp"

World::World() :
	Frame(1280, 720),
	WorldShader(Config::VertexCorePath, Config::FragmentCorePath),
	ViewMatrix(glm::mat4(0.0f)),
	Forward(glm::vec3(0.0f, 0.0f, -0.5f)),
	Aspect(1.3f),
	Near(0.1f), Far(100.0f),
	RotationSpeed(100.0f),
	CurrentPos(glm::vec2(0.0f, 0.0f))
{
	SetToDefault();
}

void World::Draw() const
{
	WorldShader.Bind();
	Frame.Bind();

	for (const auto& model : Models)
		model.Draw();

	Frame.Unbind();
	WorldShader.Unbind();
}

void World::Update(const ImVec2& screenSize, const float deltaTime)
{
	Aspect = screenSize.x / screenSize.y;
	UpdateViewMatrix();

	for (auto& model : Models)
		model.Update(deltaTime);

	SendToShader();
}

void World::SendToShader()
{
	WorldShader.Bind();

	// Light
	Light.SetPosition(LightPosition);
	Light.SendToShader(WorldShader);

	// Camera
	WorldShader.SetMat4(ViewMatrix, "ViewMatrix");
	WorldShader.SetMat4(glm::perspective(glm::radians<float>(static_cast<float>(Fov)), Aspect, Near, Far), "ProjectionMatrix");
	WorldShader.SetVec3(glm::vec3(0.0f, 0.0f, 3.0f), "cameraPos");

	// Model
	for (auto& model : Models)
		model.SendToShader(WorldShader);

	// Material
	Material.SendToShader(WorldShader);

	WorldShader.Unbind();
}

glm::quat World::GetDirection() const
{
	const auto pitchInRadians = glm::radians<float>(static_cast<float>(Pitch));
	const auto yawInRadians = glm::radians<float>(static_cast<float>(Yaw));
	const auto rollInRadians = glm::radians<float>(static_cast<float>(Roll)) + glm::radians(180.0f);

	return glm::quat{ glm::vec3{-pitchInRadians, -yawInRadians, rollInRadians} };
}

glm::vec3 World::GetForward() const
{
	return rotate(GetDirection(), Forward);
}

void World::UpdateViewMatrix()
{
	const auto position = Focus + glm::vec3(0.0f, 0.873f, 0.0f) - GetForward() * Distance;
	const glm::quat orientation = GetDirection();

	ViewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
	ViewMatrix = glm::inverse(ViewMatrix);
}

void World::OnMouseMove(const double x, const double y, const Button& button)
{
	const auto pos = glm::vec2(x, y);

	if (button == Button::RIGHT)
	{
		//const auto delta = (pos - CurrentPos) * 0.004f;
		//const auto temp = Rotation.y + delta.x * RotationSpeed;

		//if (temp >= 180.0f)
		//	Rotation.y = 180.0f;
		//else if (temp <= -180.0f)
		//	Rotation.y = -180.0f;
		//else
		//	Rotation.y = temp;
	}

	CurrentPos = pos;
}

void World::SetToDefault()
{
	DefaultFov();
	DefaultDistance();
	DefaultPitch();
	DefaultYaw();
	DefaultRoll();
	DefaultFocus();
	DefaultLightPosition();
}

void World::RemoveModel(int modelId)
{
	throw std::logic_error("Not implemented");
}

Model* World::GetModel(const int id)
{
	const auto iterator = std::ranges::find_if(Models, [&](const Model& model)
	{
		return model.GetId() == id;
	});

	if (iterator == Models.end())
		return nullptr;

	return &*iterator;
}
