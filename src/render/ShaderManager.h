#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <map>
#include <memory>
#include <string>

#include <GL/glew.h>

class ShaderManager
{
	using shader_key = std::string;

private:
	ShaderManager() {}
	~ShaderManager() { this->exiting = true; }

public:
	static ShaderManager &instance();

	GLuint get_vertex_shader(const std::string &shader_source_path);
	GLuint get_fragment_shader(const std::string &shader_source_path);
	std::shared_ptr<GLuint> get_shader_program(const std::string &vertex_shader_path,
	                                           const std::string &fragment_shader_path);

	void set_shaders_root(const std::string &path_prefix);

private:
	GLuint get_shader(const std::string &shader_source_path, int shader_type);
	std::string readShaderSource(const std::string &shader_source_path) const;

	std::map<shader_key, std::shared_ptr<GLuint>> shaders;
	std::map<std::pair<shader_key, shader_key>, std::shared_ptr<GLuint>> programs;
	std::string shaders_root;
	bool exiting = false;
};

#endif // SHADER_MANAGER_H
