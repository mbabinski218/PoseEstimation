#include "Model.hpp"

Model::Model() :
	MeshShader(std::make_shared<Shader>(Config::VertexCorePath, Config::FragmentCorePath)),
	Frame(std::make_unique<FrameBuffer>(1280, 720))
{

}

void Model::Draw() const
{
	MeshShader->Bind();
	Frame->Bind();

	for(const auto& mesh : Meshes)
		mesh.Draw();

	Frame->Unbind();
	MeshShader->Unbind();
}

void Model::Update(const ImVec2& screenSize)
{
	MeshShader->Bind();

	for (auto& mesh : Meshes)
		mesh.Update(screenSize, MeshShader);

	MeshShader->Unbind();
}
