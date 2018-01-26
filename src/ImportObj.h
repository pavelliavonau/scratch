#ifndef IMPORTOBJ_H
#define IMPORTOBJ_H

#include <vector>
#include <string>
#include <functional>

#include <glm/glm.hpp>

class OBJLoader
{
public:
	bool loadVertices(const char * path, std::vector <glm::vec3> & out_vertices);
	bool loadVerticesUV(const char * path, std::vector <glm::vec3> & out_vertices, std::vector <glm::vec2> & out_uvs);
	bool loadVerticesUVNormal(const char * path, std::vector <glm::vec3> & out_vertices, std::vector <glm::vec2> & out_uvs, std::vector <glm::vec3> & out_normals);

private:
	bool parseFile(const char*path);
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::function<bool(FILE *, unsigned int*, unsigned int*, unsigned int*)> parseFace = [] (FILE *, unsigned int* , unsigned int* , unsigned int* ) -> bool { return true; };
};

#endif // IMPORTOBJ_H
