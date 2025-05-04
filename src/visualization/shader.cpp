#include "visualization/shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

namespace visualization {

Shader::Shader() : programId_(glCreateProgram()), vertexShaderId_(0), fragmentShaderId_(0) {
}

Shader::~Shader() {
    glDeleteProgram(programId_);
}

bool Shader::loadFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
    // Implement loading shaders from files
    return false;  // Placeholder implementation
}

void Shader::use() const {
    glUseProgram(programId_);
}

void Shader::setUniform(const std::string& name, const Matrix4& matrix) {
    int location = glGetUniformLocation(programId_, name.c_str());
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void Shader::setUniform(const std::string& name, const Vector4& color) {
    int location = glGetUniformLocation(programId_, name.c_str());
    if (location != -1) {
        glUniform4fv(location, 1, glm::value_ptr(color));
    }
}

void Shader::setUniform(const std::string& name, float value) {
    int location = glGetUniformLocation(programId_, name.c_str());
    if (location != -1) {
        glUniform1f(location, value);
    }
}

bool Shader::compileShader(unsigned int& shader, const std::string& source,
                           const std::string& type) {
    // Implement shader compilation
    return false;  // Placeholder implementation
}

}  // namespace visualization