// visualization/font_manager.h
#ifndef VISUALIZATION_FONT_MANAGER_H
#define VISUALIZATION_FONT_MANAGER_H

#include <ft2build.h>

#include "render_types.h"
#include FT_FREETYPE_H
#include <map>
#include <memory>
#include <string>

namespace visualization {

class FontManager {
public:
    FontManager();
    ~FontManager();

    // Delete copy and move operations
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
    FontManager(FontManager&&) = delete;
    FontManager& operator=(FontManager&&) = delete;

    // Initialization
    bool initialize(RenderMode mode = RenderMode::Normal);
    void cleanup();

    // Font loading
    bool loadSystemFonts();
    void createFallbackFont();

    // Character retrieval
    const Character* getCharacter(char c) const;
    bool hasCharacter(char c) const;

    // Information
    bool isInitialized() const;

private:
    // Platform-specific font loading
    bool loadFonts(FT_Library ft, FT_Face& face);

    // Implementation details
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace visualization

#endif  // VISUALIZATION_FONT_MANAGER_H