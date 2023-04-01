#pragma once
#include "assimp/scene.h"
#include "Model.hpp"
#include "Vertex.hpp"

class Loader
{
public:
	static void LoadModel(const std::string& path, Model& model);

private:
	static void ProcessNode(Model& model, const aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes);
	static void SetVertexBoneDataToDefault(Vertex& vertex);
	static Mesh ProcessMesh(Model& model, const aiMesh* mesh, const aiScene* scene);
	static void SetVertexBoneData(Vertex& vertex, int boneId, float weight);
	static void ExtractBoneWeight(Model& model, std::vector<Vertex>& vertices, const aiMesh* mesh, const aiScene* scene);
};
