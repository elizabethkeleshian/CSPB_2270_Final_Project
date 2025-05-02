#include "visualization/shader.h"
#include <gtest/gtest.h>
#include "scene_graph/types.h"

using namespace std;

namespace visualization {

class ShaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        shader = make_unique<Shader>();
    }

    std::unique_ptr<Shader> shader;
};

TEST_F(ShaderTest, LoadFromFile_ValidShaders) {
    EXPECT_TRUE(shader->loadFromFile("shaders/basic.vert", "shaders/basic.frag"));
}

TEST_F(ShaderTest, LoadFromFile_InvalidShaders) {
    EXPECT_FALSE(shader->loadFromFile("nonexistent.vert", "nonexistent.frag"));
}

TEST_F(ShaderTest, Use_ActivatesShader) {
    shader->loadFromFile("shaders/basic.vert", "shaders/basic.frag");
    shader->use();
    // TODO: Verify shader is active
}

TEST_F(ShaderTest, SetUniform_Matrix) {
    shader->loadFromFile("shaders/basic.vert", "shaders/basic.frag");
    Matrix4 matrix(1.0f);
    shader->setUniform("model", matrix);
    // TODO: Verify uniform is set
}

TEST_F(ShaderTest, SetUniform_Color) {
    shader->loadFromFile("shaders/basic.vert", "shaders/basic.frag");
    Vector4 color(1.0f, 0.0f, 0.0f, 1.0f);
    shader->setUniform("color", color);
    // TODO: Verify uniform is set
}

TEST_F(ShaderTest, SetUniform_Float) {
    shader->loadFromFile("shaders/basic.vert", "shaders/basic.frag");
    shader->setUniform("alpha", 0.5f);
    // TODO: Verify uniform is set
}

} // namespace visualization 