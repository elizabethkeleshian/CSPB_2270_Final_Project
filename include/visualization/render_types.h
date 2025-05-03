// visualization/render_types.h
#ifndef VISUALIZATION_RENDER_TYPES_H
#define VISUALIZATION_RENDER_TYPES_H

#include <glm/glm.hpp>

namespace visualization {

// Character glyph data for font rendering
struct Character {
  unsigned int textureID; // ID handle of the glyph texture
  glm::ivec2 size;        // Size of glyph
  glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
  unsigned int advance;   // Offset to advance to next glyph
};

// Rendering modes
enum class RenderMode { Normal, Headless };

} // namespace visualization

#endif // VISUALIZATION_RENDER_TYPES_H