#ifndef VISUALIZATION_SHADER_H
#define VISUALIZATION_SHADER_H

#include <glm/glm.hpp>
#include <string>

#include "types.h"

namespace visualization {

/**
 * @brief The Shader class is responsible for loading and managing shaders.
 *
 * This class handles the loading of vertex and fragment shaders from files,
 * compiling them, and setting uniforms.
 */
class Shader {
public:
    Shader();
    ~Shader();

    // delete move constructor and assignment operator
    Shader(Shader&&) = delete;
    Shader& operator=(Shader&&) = delete;

    // delete copy constructor and assignment operator
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // Shader management
    bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
    void use() const;

    // Uniform setters
    void setUniform(const std::string& name, const Matrix4& matrix);
    void setUniform(const std::string& name, const Vector4& color);
    void setUniform(const std::string& name, float value);

private:
    unsigned int programId_;
    unsigned int vertexShaderId_;
    unsigned int fragmentShaderId_;

    bool compileShader(unsigned int& shader, const std::string& source, const std::string& type);
};

}  // namespace visualization

#endif  // VISUALIZATION_SHADER_H