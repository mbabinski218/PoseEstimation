#include "ObjLoader.hpp"

std::vector<Vertex> ObjLoader::Load(const std::string& path)
{
	// Vertex portions
	std::vector<glm::fvec3> vertexPositions;
	std::vector<glm::fvec2> vertexTexcoords;
	std::vector<glm::fvec3> vertexNormals;

	// Face vectors
	std::vector<GLint> vertexPositionIndices;
	std::vector<GLint> vertexTexcoordIndices;
	std::vector<GLint> vertexNormalIndices;

	std::vector<Vertex> vertices;
	std::ifstream file(path);
	std::stringstream ss;
	std::string line;

	if (!file)
		throw std::exception("Obj file error");

	while(std::getline(file, line))
	{
		ss.clear();
		ss.str(line);

		std::string prefix;
		ss >> prefix;

		if (prefix == "v")
		{
			glm::fvec3 temp;
			ss >> temp.x >> temp.y >> temp.z;
			vertexPositions.push_back(temp);
		}
		else if (prefix == "vt")
		{
			glm::fvec2 temp;
			ss >> temp.x >> temp.y;
			vertexTexcoords.push_back(temp);
		}
		else if (prefix == "vn")
		{
			glm::fvec3 temp;
			ss >> temp.x >> temp.y >> temp.z;
			vertexNormals.push_back(temp);
		}
		else if (prefix == "f")
		{
			GLint temp = 0;
			int counter = 0;

			while(ss >> temp)
			{
				if (counter == 0)
					vertexPositionIndices.push_back(temp);
				else if (counter == 1)
					vertexTexcoordIndices.push_back(temp);
				else if (counter == 2)
					vertexNormalIndices.push_back(temp);

				if(ss.peek() == '/')
				{
					counter++;
					ss.ignore(1, '/');
				}
				else if(ss.peek() == ' ')
				{
					counter++;
					ss.ignore(1, ' ');
				}

				if (counter > 2)
					counter = 0;
			}
		}
	}

	vertices.resize(vertexPositionIndices.size(), Vertex());

	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].Position = vertexPositions[vertexPositionIndices[i] - 1];
		vertices[i].Texcoord = vertexTexcoords[vertexTexcoordIndices[i] - 1];
		vertices[i].Normal = vertexNormals[vertexNormalIndices[i] - 1];
		vertices[i].Color = glm::vec3(1, 1, 1);
	}

	return vertices;
}