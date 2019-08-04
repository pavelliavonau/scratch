#ifndef IMPORTOBJ_H
#define IMPORTOBJ_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

class OBJLoader
{
public:
	OBJLoader() = delete;

	static bool loadVertices        (const char * path, std::vector <glm::vec3> & out_vertices);
	static bool loadVerticesUV      (const char * path, std::vector <glm::vec3> & out_vertices, std::vector <glm::vec2> & out_uvs);
	static bool loadVerticesUVNormal(const char * path, std::vector <glm::vec3> & out_vertices, std::vector <glm::vec2> & out_uvs, std::vector <glm::vec3> & out_normals);

private:
	using faceReader_t = std::add_pointer<bool(FILE *, unsigned int*, unsigned int*, unsigned int*)>::type;

	static bool parseFile(const char*path,
						  std::vector<glm::vec3>& outVertices,
						  std::vector<glm::vec2>& outUVs,
						  std::vector<glm::vec3>& outNormals);

	static bool parseFaceVertices        (FILE *, unsigned int*, unsigned int*, unsigned int*);
	static bool parseFaceVerticesUV      (FILE *, unsigned int*, unsigned int*, unsigned int*);
	static bool parseFaceVerticesUVNormal(FILE *, unsigned int*, unsigned int*, unsigned int*);
};

#endif // IMPORTOBJ_H
