#include "stdafx.h"
#include "Loader.hpp"

void Loader::LoadModel(const std::string& path, Model& model)
{
    const auto fullPath = std::filesystem::current_path().string() + path;

    Assimp::Importer importer;
    const auto scene = importer.ReadFile(fullPath, aiProcess_Triangulate | aiProcess_ValidateDataStructure | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        throw std::exception(importer.GetErrorString());

    // process assimp's root node recursively
    ProcessNode(model, scene->mRootNode, scene, model.GetMeshes());
}

void Loader::ProcessNode(Model& model, const aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes)
{
    for (unsigned i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        const auto mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(model, mesh, scene));
    }

    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(model, node->mChildren[i], scene, meshes);
    }
}

void Loader::SetVertexBoneDataToDefault(Vertex& vertex)
{
    for (int i = 0; i < Config::MaxBoneInfluence; i++)
    {
        vertex.BoneIds[i] = -1;
        vertex.Weights[i] = 0.0f;
    }
}

Mesh Loader::ProcessMesh(Model& model, const aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // walk through each of the mesh's vertices
    for (unsigned i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex{};

        SetVertexBoneDataToDefault(vertex);

        vertex.Position = Converter::ToGlmVec3(mesh->mVertices[i]);

        if (mesh->HasNormals())
			vertex.Normal = Converter::ToGlmVec3(mesh->mNormals[i]);

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.Texcoord = vec;
        }
        else
            vertex.Texcoord = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned i = 0; i < mesh->mNumFaces; i++)
    {
        const auto face = mesh->mFaces[i];

        // retrieve all indices of the face and store them in the indices vector
        for (unsigned j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    ExtractBoneWeight(model, vertices, mesh, scene);
    return Mesh(vertices, indices);
}

void Loader::SetVertexBoneData(Vertex& vertex, const int boneId, const float weight)
{
    for (int i = 0; i < Config::MaxBoneInfluence; ++i)
    {
        if (vertex.BoneIds[i] < 0)
        {
            vertex.Weights[i] = weight;
            vertex.BoneIds[i] = boneId;
            break;
        }
    }
}

void Loader::ExtractBoneWeight(Model& model, std::vector<Vertex>& vertices, const aiMesh* mesh, const aiScene* scene)
{
    auto& boneInfoMap = model.GetBoneInfoMap();
    auto& boneCounter = model.GetBoneCounter();

    for (unsigned boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneId = -1;
        const auto boneName = mesh->mBones[boneIndex]->mName.C_Str();

        if (!boneInfoMap.contains(boneName))
        {
            BoneInfo newBoneInfo{};
            newBoneInfo.Id = boneCounter;
            newBoneInfo.Offset = Converter::ToGlmMatrix(mesh->mBones[boneIndex]->mOffsetMatrix);
            boneInfoMap[boneName] = newBoneInfo;

        	boneId = boneCounter;
            boneCounter++;
        }
        else
        {
            boneId = boneInfoMap[boneName].Id;
        }

        if(boneId == -1) 
            continue;

        const auto weights = mesh->mBones[boneIndex]->mWeights;
        const auto numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (unsigned weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
	        const auto vertexId = weights[weightIndex].mVertexId;
            const auto weight = weights[weightIndex].mWeight;

            if (vertexId >= vertices.size())
                continue;

            SetVertexBoneData(vertices[vertexId], boneId, weight);
        }
    }
}