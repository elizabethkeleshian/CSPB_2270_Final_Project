#include "visualization/shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace visualization {

Shader::Shader()
    : programId_(glCreateProgram()), vertexShaderId_(0), fragmentShaderId_(0) {}

Shader::~Shader() { glDeleteProgram(programId_); }

} // namespace visualization