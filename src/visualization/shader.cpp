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

    bool Shader::loadFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
        // Return true for now to fix warning
        return true;
    }

    
} // namespace visualization    