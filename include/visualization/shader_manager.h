// visualization/shader_manager.h
#ifndef VISUALIZATION_SHADER_MANAGER_H
#define VISUALIZATION_SHADER_MANAGER_H

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "render_types.h"
#include "types.h"

namespace visualization {

class ShaderManager {
public:
    ShaderManager();
    ~ShaderManager();

    // Delete copy and move operations
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
    ShaderManager(ShaderManager&&) = delete;
    ShaderManager& operator=(ShaderManager&&) = delete;

    // Initialization
    bool initialize(RenderMode mode = RenderMode::Normal);
    void cleanup();

    // Shader program management
    bool createShaderProgram(const std::string& name, const std::string& vertexSource,
                             const std::string& fragmentSource);

    // Shader usage
    void useShader(const std::string& name);

    // Uniform setters
    void setUniformMatrix4fv(const std::string& shader, const std::string& name,
                             const Matrix4& matrix);
    void setUniform4f(const std::string& shader, const std::string& name, const Vector4& vec);
    void setUniform1f(const std::string& shader, const std::string& name, float value);

    // Getters
    unsigned int getShaderProgram(const std::string& name) const;
    bool isInitialized() const;
    bool isHeadlessMode() const;

private:
    // Helper methods
    bool compileShader(unsigned int& shader, const std::string& source, const std::string& type);

    // Implementation details
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace visualization

#endif  // VISUALIZATION_SHADER_MANAGER_H