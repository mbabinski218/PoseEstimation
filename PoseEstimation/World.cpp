#include "World.hpp"

void World::Draw()
{
	WorldShader.Bind();
	Frame.Bind();

	Light.SendToShader(WorldShader);
	Material.SendToShader(WorldShader);
	CameraView.SendToShader(WorldShader);
	for (auto& model : Models)
	{
		model.SendToShader(WorldShader);
		model.Draw();
	}

	Frame.Unbind();
	WorldShader.Unbind();
}

void World::Update(const ImVec2& screenSize, const float deltaTime)
{
	CameraView.Update(screenSize);

	for (auto& model : Models)
		model.Update(deltaTime);
}

void World::RemoveModel(const int id)
{
	const auto subrange = std::ranges::remove_if(Models, [&](const Model& model)
	{
		return model.GetId() == id;
	});

	Models.erase(subrange.begin(), subrange.end());
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
