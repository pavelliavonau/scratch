#include "ImportObj.h"

#include <string>
#include <cstring>
#include <iostream>

bool OBJLoader::loadVertices(const char *path, std::vector<glm::vec3> &out_vertices)
{
	std::vector<glm::vec2> outUVs;
	std::vector<glm::vec3> outNormals;
	return parseFile(path, out_vertices, outUVs, outNormals);
}

bool OBJLoader::loadVerticesUV(const char *path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs)
{
	std::vector<glm::vec3> outNormals;
	return parseFile(path, out_vertices, out_uvs, outNormals);
}

bool OBJLoader::loadVerticesUVNormal(const char *path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals)
{
	return parseFile(path, out_vertices, out_uvs, out_normals);
}

bool OBJLoader::parseFaceVertices(FILE * file, unsigned int* vertexIndex, unsigned int*, unsigned int*)
{
	int matches = fscanf(file, "%u %u %u\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
	if(matches == 3)
		return true;
	else
	{
		std::cout << "loadVertices failed" << std::endl;;
		return false;
	}
}

bool OBJLoader::parseFaceVerticesUV(FILE * file, unsigned int* vertexIndex, unsigned int* uvIndex, unsigned int*)
{
	int matches = fscanf(file, "%u/%u %u/%u %u/%u\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
	if(matches == 6)
		return true;
	else
	{
		std::cout << "loadVerticesUV failed" << std::endl;;
		return false;
	}
}

bool OBJLoader::parseFaceVerticesUVNormal(FILE * file, unsigned int* vertexIndex, unsigned int* uvIndex, unsigned int* normalIndex)
{
	int matches = fscanf(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
	if(matches == 9)
		return true;
	else
	{
		std::cout << "loadVerticesUVNormal failed" << std::endl;;
		return false;
	}
}

template<typename T>
void addDataOfFace(std::vector<T>& dest, unsigned int* indexes, std::vector<T>& dataList)
{
	for(auto i = 0u; i < 3; i++)
	{
		auto itemIndex = indexes[i];
		auto item = dataList[itemIndex - 1];
		dest.push_back(item);
	}
}

bool OBJLoader::parseFile(const char *path,
						  std::vector<glm::vec3>& outVertices,
						  std::vector<glm::vec2>& outUVs,
						  std::vector<glm::vec3>& outNormals)
{
	std::vector<glm::vec3> verticesList;
	std::vector<glm::vec2> uvsList;
	std::vector<glm::vec3> normalsList;

	faceReader_t faceReader = OBJLoader::parseFaceVertices;

	FILE * file = fopen(path, "r");
	if( file == nullptr )
	{
		printf("Impossible to open the file !\n");
		fclose(file);
		return false;
	}

	while( 1 )
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if ( strcmp( lineHeader, "v" ) == 0 )
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			verticesList.push_back(vertex);
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 )
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uvsList.push_back(uv);
			faceReader = OBJLoader::parseFaceVerticesUV;
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 )
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			normalsList.push_back(normal);
			faceReader = OBJLoader::parseFaceVerticesUVNormal;
		}
		else if ( strcmp( lineHeader, "f" ) == 0 )
		{
			unsigned int vertexIndices[3], uvIndices[3], normalIndices[3];
			if (!faceReader(file, vertexIndices, uvIndices, normalIndices))
			{
				printf("File can't be read by our simple parser : ( Try exporting with other options.\n");
				fclose(file);
				return false;
			}

			if(!verticesList.empty())
				addDataOfFace(outVertices, vertexIndices, verticesList);
			if(!uvsList.empty())
				addDataOfFace(outUVs, uvIndices, uvsList);
			if(!normalsList.empty())
				addDataOfFace(outNormals, normalIndices, normalsList);
		}
	}

	fclose(file);
	return true;
}
