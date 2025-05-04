// visualization/text_renderer.h
#ifndef VISUALIZATION_TEXT_RENDERER_H
#define VISUALIZATION_TEXT_RENDERER_H

#include <memory>
#include <string>

#include "font_manager.h"
#include "render_types.h"
#include "shader_manager.h"

namespace visualization {

class TextRenderer {
public:
    TextRenderer(std::shared_ptr<FontManager> fontManager,
                 std::shared_ptr<ShaderManager> shaderManager);
    ~TextRenderer();

    // Delete copy and move operations
    TextRenderer(const TextRenderer&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;
    TextRenderer(TextRenderer&&) = delete;
    TextRenderer& operator=(TextRenderer&&) = delete;

    // Initialization
    bool initialize(RenderMode mode = RenderMode::Normal);
    void cleanup();

    // Text rendering
    void drawText(const std::string& text, float x, float y, const Vector4& color);

    // Information
    bool isInitialized() const;

private:
    // Keep references to the managers
    std::shared_ptr<FontManager> fontManager_;
    std::shared_ptr<ShaderManager> shaderManager_;

    // Implementation details
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace visualization

#endif  // VISUALIZATION_TEXT_RENDERER_H