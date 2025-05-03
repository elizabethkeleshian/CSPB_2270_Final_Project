#include "visualization/shader_manager.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <unordered_map>

namespace visualization {

struct ShaderManager::Impl {
  RenderMode renderMode = RenderMode::Normal;
  bool initialized = false;

  // Map of shader program names to OpenGL program IDs
  std::unordered_map<std::string, unsigned int> shaderPrograms;
};

ShaderManager::ShaderManager() : impl_(std::make_unique<Impl>()) {}

ShaderManager::~ShaderManager() { cleanup(); }

bool ShaderManager::initialize(RenderMode mode) {
  impl_->renderMode = mode;
  impl_->initialized = true;
  return true;
}

bool ShaderManager::isHeadlessMode() const {
  return impl_->renderMode == RenderMode::Headless;
}

void ShaderManager::cleanup() {
  if (impl_->renderMode == RenderMode::Headless) {
    impl_->shaderPrograms.clear();
    impl_->initialized = false;
    return;
  }

  // Delete all shader programs
  for (auto &[name, program] : impl_->shaderPrograms) {
    if (program != 0) {
      glDeleteProgram(program);
    }
  }

  impl_->shaderPrograms.clear();
  impl_->initialized = false;
}

bool ShaderManager::createShaderProgram(const std::string &name,
                                        const std::string &vertexSource,
                                        const std::string &fragmentSource) {
  // Skip creating shaders in headless mode
  if (impl_->renderMode == RenderMode::Headless) {
    // In headless mode, just register the name and return success
    impl_->shaderPrograms[name] = 0;
    return true;
  }

  // Compile vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  if (!compileShader(vertexShader, vertexSource, "vertex")) {
    glDeleteShader(vertexShader);
    return false;
  }

  // Compile fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  if (!compileShader(fragmentShader, fragmentSource, "fragment")) {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return false;
  }

  // Create and link shader program
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check linking status
  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cerr << "Shader program '" << name << "' linking failed: " << infoLog
              << std::endl;

    // Clean up resources
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    return false;
  }

  // Cleanup the individual shaders (they're now linked to the program)
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Store the shader program
  impl_->shaderPrograms[name] = shaderProgram;

  return true;
}

// Helper method for shader compilation
bool ShaderManager::compileShader(unsigned int &shader,
                                  const std::string &source,
                                  const std::string &type) {
  const char *shaderSource = source.c_str();
  glShaderSource(shader, 1, &shaderSource, nullptr);
  glCompileShader(shader);

  // Check compilation status
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << type << " shader compilation failed: " << infoLog << std::endl;
    return false;
  }

  return true;
}

void ShaderManager::useShader(const std::string &name) {
  if (impl_->renderMode == RenderMode::Headless) {
    return;
  }

  auto it = impl_->shaderPrograms.find(name);
  if (it != impl_->shaderPrograms.end()) {
    glUseProgram(it->second);
  } else {
    std::cerr << "Shader program '" << name << "' not found!" << std::endl;
  }
}

void ShaderManager::setUniformMatrix4fv(const std::string &shader,
                                        const std::string &name,
                                        const Matrix4 &matrix) {
  if (impl_->renderMode == RenderMode::Headless) {
    return;
  }

  auto it = impl_->shaderPrograms.find(shader);
  if (it != impl_->shaderPrograms.end()) {
    unsigned int location = glGetUniformLocation(it->second, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }
}

void ShaderManager::setUniform4f(const std::string &shader,
                                 const std::string &name, const Vector4 &vec) {
  if (impl_->renderMode == RenderMode::Headless) {
    return;
  }

  auto it = impl_->shaderPrograms.find(shader);
  if (it != impl_->shaderPrograms.end()) {
    unsigned int location = glGetUniformLocation(it->second, name.c_str());
    glUniform4f(location, vec.r, vec.g, vec.b, vec.a);
  }
}

void ShaderManager::setUniform1f(const std::string &shader,
                                 const std::string &name, float value) {
  if (impl_->renderMode == RenderMode::Headless) {
    return;
  }

  auto it = impl_->shaderPrograms.find(shader);
  if (it != impl_->shaderPrograms.end()) {
    unsigned int location = glGetUniformLocation(it->second, name.c_str());
    glUniform1f(location, value);
  }
}

unsigned int ShaderManager::getShaderProgram(const std::string &name) const {
  auto it = impl_->shaderPrograms.find(name);
  if (it != impl_->shaderPrograms.end()) {
    return it->second;
  }
  return 0;
}

bool ShaderManager::isInitialized() const { return impl_->initialized; }

} // namespace visualization