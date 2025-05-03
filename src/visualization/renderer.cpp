#include "visualization/renderer.h"
#include "scene_graph/circle.h"
#include "scene_graph/rectangle.h"
#include "types.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <memory>
#include FT_FREETYPE_H

namespace visualization {

/**
 * @brief Private implementation struct for the renderer
 *
 * Using the pImpl pattern to hide OpenGL details and reduce
 * header dependencies. This keeps our public interface clean
 * and improves compilation times in the rest of the codebase.
 */
struct Renderer::Impl {
  int viewportWidth = 800;
  int viewportHeight = 600;
  bool initialized = false;

  unsigned int rectangleVAO = 0;
  unsigned int circleVAO = 0;
  unsigned int shaderProgram = 0;

  // Text rendering
  unsigned int textVAO = 0;
  unsigned int textVBO = 0;
  unsigned int textShaderProgram = 0;

  // Character structure for text rendering
  struct Character {
    unsigned int textureID; // ID handle of the glyph texture
    glm::ivec2 size;        // Size of glyph
    glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
    unsigned int advance;   // Offset to advance to next glyph
  };

  std::map<char, Character> characters;
  bool textInitialized = false;
};

Renderer::Renderer() : impl_(std::make_unique<Impl>()) {};

Renderer::~Renderer() { cleanup(); }

/**
 * @brief Initializes the renderer with OpenGL resources
 *
 * Sets up shaders, VAOs, and buffers needed for drawing our
 * shapes. We've kept this simple with basic vertex and fragment
 * shaders since our 2D editing needs are minimal.
 *
 * @return true if initialization succeeded, false otherwise
 */
bool Renderer::initialize() {
  // Initialize GLEW - must happen after GL context is created
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW\n";
    return false;
  }

  // Create a simple shader program
  const char *vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        
        uniform mat4 model;
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
        }
    )";

  const char *fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        
        uniform vec4 color;
        
        void main() {
            FragColor = color;
        }
    )";

  // Compile shaders
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cerr << "Vertex shader compilation failed: " << infoLog << '\n';
    return false;
  }

  // Compile fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  // Check for compilation errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cerr << "Fragment shader compilation failed: " << infoLog << '\n';
    return false;
  }

  // Link shaders
  impl_->shaderProgram = glCreateProgram();
  glAttachShader(impl_->shaderProgram, vertexShader);
  glAttachShader(impl_->shaderProgram, fragmentShader);
  glLinkProgram(impl_->shaderProgram);

  // Check for linking errors
  glGetProgramiv(impl_->shaderProgram, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    glGetProgramInfoLog(impl_->shaderProgram, 512, nullptr, infoLog);
    std::cerr << "Shader program linking failed: " << infoLog << '\n';
    return false;
  }

  // Delete shaders as they are already linked to the program
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Rectangle geometry - centered unit square makes transforms intuitive
  float rectangleVertices[] = {
      // positions
      -0.5F, -0.5F, // bottom left
      0.5F,  -0.5F, // bottom right
      0.5F,  0.5F,  // top right
      -0.5F, 0.5F   // top left
  };

  // Rectangle indices - two triangles forming a quad
  unsigned int rectangleIndices[] = {
      0, 1, 2, // first triangle
      2, 3, 0  // second triangle
  };

  unsigned int VBO, EBO;
  glGenVertexArrays(1, &impl_->rectangleVAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(impl_->rectangleVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices),
               rectangleIndices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Circle - approximated as polygon with triangles from center
  // 32 segments gives smooth appearance at reasonable performance
  const int circleSegments = 32;
  float circleVertices[circleSegments * 2 + 2];
  circleVertices[0] = 0.0f; // center x
  circleVertices[1] = 0.0f; // center y

  for (int i = 0; i <= circleSegments; i++) {
    float angle = 2.0f * M_PI * i / circleSegments;
    circleVertices[2 + i * 2] = 0.5f * cos(angle);     // x
    circleVertices[2 + i * 2 + 1] = 0.5f * sin(angle); // y
  }

  unsigned int circleIndices[circleSegments * 3];
  for (int i = 0; i < circleSegments; i++) {
    circleIndices[i * 3] = 0; // center
    circleIndices[i * 3 + 1] = i + 1;
    circleIndices[i * 3 + 2] = (i + 1) % circleSegments + 1;
  }

  unsigned int circleVBO, circleEBO;
  glGenVertexArrays(1, &impl_->circleVAO);
  glGenBuffers(1, &circleVBO);
  glGenBuffers(1, &circleEBO);

  glBindVertexArray(impl_->circleVAO);

  glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circleEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(circleIndices), circleIndices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Enable alpha blending for semi-transparent shapes
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Initialize text rendering
  initTextRendering();

  impl_->initialized = true;
  return true;
}

bool Renderer::initTextRendering() {
  // Text shader program
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

  // Compile text shaders
  unsigned int textVertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(textVertexShader, 1, &textVertexShaderSource, nullptr);
  glCompileShader(textVertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(textVertexShader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(textVertexShader, 512, nullptr, infoLog);
    std::cerr << "Text vertex shader compilation failed: " << infoLog << '\n';
    return false;
  }

  unsigned int textFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(textFragmentShader, 1, &textFragmentShaderSource, nullptr);
  glCompileShader(textFragmentShader);

  glGetShaderiv(textFragmentShader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(textFragmentShader, 512, nullptr, infoLog);
    std::cerr << "Text fragment shader compilation failed: " << infoLog << '\n';
    return false;
  }

  // Create shader program
  impl_->textShaderProgram = glCreateProgram();
  glAttachShader(impl_->textShaderProgram, textVertexShader);
  glAttachShader(impl_->textShaderProgram, textFragmentShader);
  glLinkProgram(impl_->textShaderProgram);

  glGetProgramiv(impl_->textShaderProgram, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    glGetProgramInfoLog(impl_->textShaderProgram, 512, nullptr, infoLog);
    std::cerr << "Text shader program linking failed: " << infoLog << '\n';
    return false;
  }

  // Delete shaders
  glDeleteShader(textVertexShader);
  glDeleteShader(textFragmentShader);

  // Initialize FreeType
  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    std::cerr << "ERROR::FREETYPE: Could not init FreeType Library"
              << std::endl;
    return false;
  }

  // Load font
  FT_Face face;
  // Try to load a system font - this path may need to be adjusted based on your
  // system
  const char *fontPath = "/System/Library/Fonts/HelveticaNeue.ttc";
  if (FT_New_Face(ft, fontPath, 0, &face)) {
    std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
    // Try a fallback font path
    fontPath = "/System/Library/Fonts/Geneva.ttf";
    if (FT_New_Face(ft, fontPath, 0, &face)) {
      // Try another common macOS font
      fontPath = "/System/Library/Fonts/SFNSMono.ttf";
      if (FT_New_Face(ft, fontPath, 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load fallback font"
                  << std::endl;
        return false;
      }
    }
  }

  // Set font size
  FT_Set_Pixel_Sizes(face, 0, 20);

  // Disable byte-alignment restriction
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Load first 128 ASCII characters
  for (unsigned char c = 0; c < 128; c++) {
    // Load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
      continue;
    }

    // Generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Store character
    Impl::Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)};
    impl_->characters.insert(std::pair<char, Impl::Character>(c, character));
  }

  // Clean up FreeType resources
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

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

  impl_->textInitialized = true;
  return true;
}

/**
 * @brief Releases all OpenGL resources
 *
 * Important for clean shutdown and preventing resource leaks.
 * This also allows reinitialization if needed.
 */
void Renderer::cleanup() {
  if (impl_->initialized) {
    glDeleteVertexArrays(1, &impl_->rectangleVAO);
    glDeleteVertexArrays(1, &impl_->circleVAO);
    glDeleteProgram(impl_->shaderProgram);

    // Clean up text rendering resources
    if (impl_->textInitialized) {
      for (auto &ch : impl_->characters) {
        glDeleteTextures(1, &ch.second.textureID);
      }
      glDeleteVertexArrays(1, &impl_->textVAO);
      glDeleteBuffers(1, &impl_->textVBO);
      glDeleteProgram(impl_->textShaderProgram);
    }

    impl_->initialized = false;
    impl_->textInitialized = false;
  }
}

/**
 * @brief Starts a new frame by clearing the screen
 *
 * Sets a pleasing blue-green background color that makes
 * our scene objects stand out visually.
 */
void Renderer::beginFrame() {
  glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::endFrame() {
  // Finish any pending operations for the frame
  glFlush();
}

void Renderer::setViewport(int width, int height) {
  impl_->viewportWidth = width;
  impl_->viewportHeight = height;
  glViewport(0, 0, width, height);
}

void Renderer::renderShape(const scene_graph::Shape &shape) {
  if (!impl_->initialized) {
    std::cerr << "Renderer not initialized!\n";
    return;
  }

  glUseProgram(impl_->shaderProgram);

  unsigned int modelLoc = glGetUniformLocation(impl_->shaderProgram, "model");
  unsigned int projLoc =
      glGetUniformLocation(impl_->shaderProgram, "projection");
  unsigned int colorLoc = glGetUniformLocation(impl_->shaderProgram, "color");

  // Create projection matrix (simple orthographic)
  Matrix4 projection = glm::ortho(
      -10.0f, 10.0f, -10.0f * impl_->viewportHeight / impl_->viewportWidth,
      10.0f * impl_->viewportHeight / impl_->viewportWidth, -1.0f, 1.0f);

  // Set uniforms
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE,
                     glm::value_ptr(shape.getGlobalTransform().getMatrix()));

  const Vector4 &color = shape.getColor();
  glUniform4f(colorLoc, color.r, color.g, color.b, color.a);

  // Draw shape based on type
  if (const auto *rect = dynamic_cast<const scene_graph::Rectangle *>(&shape)) {
    const Vector2 &size = rect->getSize();

    // Create model matrix that scales to the correct size
    Matrix4 sizeScale =
        glm::scale(Matrix4(1.0f), glm::vec3(size.x, size.y, 1.0f));
    glUniformMatrix4fv(
        modelLoc, 1, GL_FALSE,
        glm::value_ptr(shape.getGlobalTransform().getMatrix() * sizeScale));

    // Draw rectangle
    glBindVertexArray(impl_->rectangleVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  } else if (const auto *circle =
                 dynamic_cast<const scene_graph::Circle *>(&shape)) {
    float radius = circle->getRadius();

    // Create model matrix that scales to the correct size
    Matrix4 sizeScale = glm::scale(
        Matrix4(1.0f), glm::vec3(radius * 2.0f, radius * 2.0f, 1.0f));
    glUniformMatrix4fv(
        modelLoc, 1, GL_FALSE,
        glm::value_ptr(shape.getGlobalTransform().getMatrix() * sizeScale));

    // Draw circle
    glBindVertexArray(impl_->circleVAO);
    glDrawElements(GL_TRIANGLES, 32 * 3, GL_UNSIGNED_INT, 0);

    // Unbind VAO
    glBindVertexArray(0);
  }
}

/**
 * @brief Renders text at the specified position with the given color
 *
 * Renders actual text with the specified font at the given position and color
 *
 * @param text The text to render
 * @param x The x-coordinate in scene space
 * @param y The y-coordinate in scene space
 * @param color The color of the text
 */
void Renderer::drawText(const std::string &text, float x, float y,
                        const Vector4 &color) {
  if (!impl_->initialized || !impl_->textInitialized) {
    return;
  }

  // Activate text shader program
  glUseProgram(impl_->textShaderProgram);

  // Create projection matrix (simple orthographic)
  Matrix4 projection = glm::ortho(
      -10.0f, 10.0f, -10.0f * impl_->viewportHeight / impl_->viewportWidth,
      10.0f * impl_->viewportHeight / impl_->viewportWidth, -1.0f, 1.0f);

  // Set projection and color uniforms
  glUniformMatrix4fv(
      glGetUniformLocation(impl_->textShaderProgram, "projection"), 1, GL_FALSE,
      glm::value_ptr(projection));

  glUniform4f(glGetUniformLocation(impl_->textShaderProgram, "textColor"),
              color.r, color.g, color.b, color.a);

  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(impl_->textVAO);

  // Convert original scene coordinates to scaling factor
  float scale = 0.003f; // Adjust as needed for text size

  // Iterate through characters
  float xpos = x;
  for (char c : text) {
    auto it = impl_->characters.find(c);
    if (it == impl_->characters.end()) {
      continue; // Skip if character not found
    }

    Impl::Character ch = it->second;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    // Calculate xpos and ypos for each character
    float xpos_offset = xpos + ch.bearing.x * scale;
    float ypos_offset = y - (ch.size.y - ch.bearing.y) * scale;

    // Update VBO for each character
    float vertices[6][4] = {{xpos_offset, ypos_offset + h, 0.0f, 0.0f},
                            {xpos_offset, ypos_offset, 0.0f, 1.0f},
                            {xpos_offset + w, ypos_offset, 1.0f, 1.0f},

                            {xpos_offset, ypos_offset + h, 0.0f, 0.0f},
                            {xpos_offset + w, ypos_offset, 1.0f, 1.0f},
                            {xpos_offset + w, ypos_offset + h, 1.0f, 0.0f}};

    // Bind texture and update VBO
    glBindTexture(GL_TEXTURE_2D, ch.textureID);
    glBindBuffer(GL_ARRAY_BUFFER, impl_->textVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Advance cursor for next glyph
    xpos += (ch.advance >> 6) * scale; // Advance is in 1/64 pixels
  }

  // Unbind
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace visualization