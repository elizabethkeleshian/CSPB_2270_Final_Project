#ifndef VISUALIZATION_SHADER_H
#define VISUALIZATION_SHADER_H

#include <string>
#include <glm/glm.hpp>
#include "scene_graph/types.h"

namespace visualization {
using namespace std;
using namespace scene_graph;
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

    // Shader management
    bool loadFromFile(const string& vertexPath, const string& fragmentPath);
    void use() const;

    // Uniform setters
    void setUniform(const string& name, const Matrix4& matrix);
    void setUniform(const string& name, const Vector4& color);
    void setUniform(const string& name, float value);

private:
    unsigned int programId_;
    unsigned int vertexShaderId_;
    unsigned int fragmentShaderId_; 

    bool compileShader(unsigned int& shader, const string& source, const string& type);
};

} // namespace visualization

#endif // VISUALIZATION_SHADER_H 