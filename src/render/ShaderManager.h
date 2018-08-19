#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <string>
#include <map>
#include <GL/glew.h>

class ShaderManager
{
using shader_key = std::string;

private:
    ShaderManager() {}

public:
    static ShaderManager& instance();

    GLuint& get_vertex_shader(const std::string &shader_source_path);
    GLuint& get_fragment_shader(const std::string &shader_source_path);
    void clean_shaders();
    void set_shaders_root(const std::string& path_prefix);

private:
    GLuint& get_shader(const std::string &shader_source_path, int shader_type);
    std::string readShaderSource(const std::string &shader_source_path) const;

    std::map<shader_key, GLuint> shaders;
    std::string shaders_root;
};

#endif // SHADER_MANAGER_H
