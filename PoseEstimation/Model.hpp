#pragma once
#include "Mesh.hpp"
#include "Bone.hpp"

class Model
{
public:
	explicit Model();
	[[nodiscard]] void* GetTexture() const { return Frame->GetTexture(); }
	[[nodiscard]] std::vector<Mesh>& GetMeshes() { return Meshes; }
	[[nodiscard]] std::map<std::string, BoneInfo>& GetBoneInfoMap() { return BoneInfoMap; }
	[[nodiscard]] int& GetBoneCounter() { return BoneCounter; }
	void Draw() const;
	void Update(const ImVec2& screenSize);
	void OnMouseMove(double x, double y, const Button& button) const {}

private:
	std::shared_ptr<Shader> MeshShader;
	std::unique_ptr<FrameBuffer> Frame;

	std::vector<Mesh> Meshes{};
	std::map<std::string, BoneInfo> BoneInfoMap{};
	int BoneCounter = 0;
};
