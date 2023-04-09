#pragma once
#include "View.hpp"
#include "Model.hpp"
#include "Material.hpp"
#include "LightPoint.hpp"
#include "FrameBuffer.hpp"

class World
{
public:
	explicit World() : Frame(Config::FrameSize), WorldShader(Config::VertexCorePath, Config::FragmentCorePath) {}
	void Draw();
	void Update(const ImVec2& screenSize, float deltaTime);

	void AddModel(int id) { Models.emplace_back(id); }
	void RemoveModel(int id);

	[[nodiscard]] void* GetTexture() const { return Frame.GetTexture(); }
	[[nodiscard]] View* GetView() { return &CameraView; }
	[[nodiscard]] Model* GetModel(int id);
	[[nodiscard]] LightPoint* GetLight() { return &Light; }

private:
	std::vector<Model> Models{};
	FrameBuffer Frame;
	Shader WorldShader;

	LightPoint Light{};
	Material Material{};
	View CameraView{};
};
