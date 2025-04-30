#ifndef VISUALIZATION_SHADER_H
#define VISUALIZATION_SHADER_H

#include <string>
#include <glm/glm.hpp>

namespace visualization {

class Shader {
public:
    Shader();
    ~Shader();

    // Shader management
    bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
    void use() const;

    // Uniform setters
    void setUniform(const std::string& name, const glm::mat4& matrix);
    void setUniform(const std::string& name, const glm::vec4& color);
    void setUniform(const std::string& name, float value);

private:
    unsigned int programId_;
    bool compileShader(unsigned int& shader, const std::string& source, const std::string& type);
};

} // namespace visualization

#endif // VISUALIZATION_SHADER_H 