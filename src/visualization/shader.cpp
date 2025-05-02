#include "../../include/visualization/shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <string>

namespace visualization {
    using namespace std;

    Shader::Shader() {
        programId_ = glCreateProgram();
    }

    Shader::~Shader() {
        glDeleteProgram(programId_);
    }

    
} // namespace visualization    