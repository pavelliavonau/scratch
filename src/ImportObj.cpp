#include "ImportObj.h"

#include <string>
#include <cstring>
#include <iostream>

template<class T>
void collectOutput(std::vector<T>& dest, std::vector<unsigned int>& indexesSource, std::vector<T>& tempSource)
{
	for(auto i = 0u; i < indexesSource.size(); i++)
	{
		auto itemIndex = indexesSource[i];
		auto item = tempSource[itemIndex - 1];
		dest.push_back(item);
	}
}

bool OBJLoader::loadVertices(const char *path, std::vector<glm::vec3> &out_vertices)
{
	this->parseFace = [] (FILE * file, unsigned int* vertexIndex, unsigned int*, unsigned int*) -> bool
	{
		int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
		if(matches == 3)
			return true;
		else
		{
			std::cout << "loadVertices failed" << std::endl;;
			return false;
		}
	};

	if(!parseFile(path))
		return false;

	collectOutput(out_vertices, vertexIndices, temp_vertices);
	return true;
}

bool OBJLoader::loadVerticesUV(const char *path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs)
{
	this->parseFace = [] (FILE * file, unsigned int* vertexIndex, unsigned int* uvIndex, unsigned int*) -> bool
	{
		int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
		if(matches == 6)
			return true;
		else
		{
			std::cout << "loadVerticesUV failed" << std::endl;;
			return false;
		}
	};

	if(!parseFile(path))
		return false;

	collectOutput(out_vertices, vertexIndices, temp_vertices);
	collectOutput(out_uvs, uvIndices, temp_uvs);
	return true;
}

bool OBJLoader::loadVerticesUVNormal(const char *path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals)
{
	this->parseFace = [] (FILE * file, unsigned int* vertexIndex, unsigned int* uvIndex, unsigned int* normalIndex) -> bool
	{
		int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
		if(matches == 9)
			return true;
		else
		{
			std::cout << "loadVerticesUVNormal failed" << std::endl;;
			return false;
		}
	};

	if(!parseFile(path))
		return false;

	collectOutput(out_vertices, vertexIndices, temp_vertices);
	collectOutput(out_uvs, uvIndices, temp_uvs);
	collectOutput(out_normals, normalIndices, temp_normals);
	return true;
}

bool OBJLoader::parseFile(const char *path)
{
	FILE * file = fopen(path, "r");
	if( file == NULL )
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
			temp_vertices.push_back(vertex);
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 )
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 )
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 )
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			if (!parseFace(file, vertexIndex, uvIndex, normalIndex))
			{
				printf("File can't be read by our simple parser : ( Try exporting with other options.\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	fclose(file);
	return true;
}
