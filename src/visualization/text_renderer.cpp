#include "visualization/text_renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace visualization {

struct TextRenderer::Impl {
  bool initialized = false;
  unsigned int textVAO = 0;
  unsigned int textVBO = 0;
  std::string shaderName = "text";
};

TextRenderer::TextRenderer(std::shared_ptr<FontManager> fontManager,
                           std::shared_ptr<ShaderManager> shaderManager)
    : fontManager_(fontManager), shaderManager_(shaderManager),
      impl_(std::make_unique<Impl>()) {}

TextRenderer::~TextRenderer() { cleanup(); }

bool TextRenderer::initialize(RenderMode mode) {
  std::cout << "TextRenderer initialization starting..." << std::endl;

  // Make sure dependencies are valid
  if (!fontManager_) {
    std::cerr << "Error: FontManager is null in TextRenderer::initialize"
              << std::endl;
    return false;
  }

  if (!shaderManager_) {
    std::cerr << "Error: ShaderManager is null in TextRenderer::initialize"
              << std::endl;
    return false;
  }

  if (!impl_) {
    std::cerr
        << "Error: Implementation (impl_) is null in TextRenderer::initialize"
        << std::endl;
    return false;
  }
  if (mode == RenderMode::Headless) {
    impl_->initialized = true;
    return true;
  }

  std::cout << "TextRenderer dependencies validated successfully" << std::endl;

  if (glfwGetCurrentContext() == nullptr) {
    std::cerr << "Error: No current OpenGL context in TextRenderer::initialize"
              << std::endl;
    return false;
  }

  // Create shader program for text rendering
  const char *textVertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
        out vec2 TexCoords;
        
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
            TexCoords = vertex.zw;
        }
    )";

  const char *textFragmentShaderSource = R"(
        #version 330 core
        in vec2 TexCoords;
        out vec4 color;
        
        uniform sampler2D text;
        uniform vec4 textColor;
        
        void main() {
            vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
            color = textColor * sampled;
        }
    )";

  if (!shaderManager_->createShaderProgram(impl_->shaderName,
                                           textVertexShaderSource,
                                           textFragmentShaderSource)) {
    std::cerr << "Failed to create text shader program" << std::endl;
    return false;
  }
  std::cout << "TextRenderer shader program created successfully" << std::endl;

  // Configure VAO/VBO for text rendering
  glGenVertexArrays(1, &impl_->textVAO);
  glGenBuffers(1, &impl_->textVBO);
  glBindVertexArray(impl_->textVAO);
  glBindBuffer(GL_ARRAY_BUFFER, impl_->textVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  impl_->initialized = true;
  std::cout << "TextRenderer initialized successfully" << std::endl;
  return true;
}

void TextRenderer::cleanup() {
  if (impl_->initialized && impl_->textVAO != 0) {
    glDeleteVertexArrays(1, &impl_->textVAO);
    glDeleteBuffers(1, &impl_->textVBO);
    impl_->textVAO = 0;
    impl_->textVBO = 0;
  }

  impl_->initialized = false;
}

void TextRenderer::drawText(const std::string &text, float x, float y,
                            const Vector4 &color) {
  if (!impl_->initialized || !fontManager_->isInitialized()) {
    return;
  }

  // Skip rendering in headless mode
  if (shaderManager_->isHeadlessMode()) {
    return;
  }

  // Activate shader
  shaderManager_->useShader(impl_->shaderName);

  // Set projection matrix (simple orthographic)
  Matrix4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
  shaderManager_->setUniformMatrix4fv(impl_->shaderName, "projection",
                                      projection);

  // Set text color
  shaderManager_->setUniform4f(impl_->shaderName, "textColor", color);

  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(impl_->textVAO);

  // Scale for text size
  float scale = 0.01f;

  // Iterate through characters
  float xpos = x;
  for (char c : text) {
    const Character *ch = fontManager_->getCharacter(c);
    if (!ch) {
      continue;
    }

    float w = ch->size.x * scale;
    float h = ch->size.y * scale;

    // Calculate position for each character
    float xpos_offset = xpos + ch->bearing.x * scale;
    float ypos_offset = y - (ch->size.y - ch->bearing.y) * scale;

    // Update VBO for each character
    float vertices[6][4] = {{xpos_offset, ypos_offset + h, 0.0f, 0.0f},
                            {xpos_offset, ypos_offset, 0.0f, 1.0f},
                            {xpos_offset + w, ypos_offset, 1.0f, 1.0f},

                            {xpos_offset, ypos_offset + h, 0.0f, 0.0f},
                            {xpos_offset + w, ypos_offset, 1.0f, 1.0f},
                            {xpos_offset + w, ypos_offset + h, 1.0f, 0.0f}};

    // Bind character texture
    glBindTexture(GL_TEXTURE_2D, ch->textureID);

    // Update VBO and render
    glBindBuffer(GL_ARRAY_BUFFER, impl_->textVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Advance for next glyph
    xpos += (ch->advance >> 6) * scale;
  }

  // Unbind
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

bool TextRenderer::isInitialized() const { return impl_->initialized; }

} // namespace visualization