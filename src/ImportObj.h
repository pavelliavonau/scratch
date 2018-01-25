#ifndef IMPORTOBJ_H
#define IMPORTOBJ_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

class OBJLoader
{
public:
	bool loadVertices(const char * path, std::vector <glm::vec3> & out_vertices);
private: // TODO: remove
	bool loadVerticesUV(const char * path, std::vector <glm::vec3> & out_vertices, std::vector <glm::vec2> & out_uvs);
	bool loadVerticesUVNormal(const char * path, std::vector <glm::vec3> & out_vertices, std::vector <glm::vec2> & out_uvs, std::vector <glm::vec3> & out_normals);

private:
	bool parseFile(const char*path);
	void collectVertices(std::vector <glm::vec3> & out_vertices);
	void collectUVs(std::vector <glm::vec2> & out_uvs);
	void collectNormals(std::vector <glm::vec3> & out_normals);
	std::string getFacePattern();
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
};

#endif // IMPORTOBJ_H
