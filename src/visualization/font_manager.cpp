#include "visualization/font_manager.h"

#include <GL/glew.h>

#include <algorithm>
#include <iostream>

namespace visualization {

struct FontManager::Impl {
    RenderMode renderMode = RenderMode::Normal;
    bool initialized = false;
    std::map<char, Character> characters;
};

FontManager::FontManager() : impl_(std::make_unique<Impl>()) {
}

FontManager::~FontManager() {
    cleanup();
}

bool FontManager::initialize(RenderMode mode) {
    impl_->renderMode = mode;

    if (mode == RenderMode::Headless) {
        impl_->initialized = true;
        return true;
    }

    // Load fonts
    if (!loadSystemFonts()) {
        std::cerr << "Failed to load any system fonts. Creating fallback font." << std::endl;
        createFallbackFont();
    }

    impl_->initialized = true;
    return true;
}

void FontManager::cleanup() {
    if (impl_->renderMode == RenderMode::Headless) {
        impl_->characters.clear();
        impl_->initialized = false;
        return;
    }

    // Delete all character textures
    for (auto& pair : impl_->characters) {
        glDeleteTextures(1, &pair.second.textureID);
    }

    impl_->characters.clear();
    impl_->initialized = false;
}

bool FontManager::loadSystemFonts() {
    // Skip actual font loading in headless mode
    if (impl_->renderMode == RenderMode::Headless) {
        return true;
    }

    // Initialize FreeType
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return false;
    }

    // Load font
    FT_Face face;
    if (!loadFonts(ft, face)) {
        FT_Done_FreeType(ft);
        return false;
    }

    // Set font size
    FT_Set_Pixel_Sizes(face, 0, 24);  // Using 24 as a reasonable default size

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 ASCII characters
    for (unsigned char c = 0; c < 128; c++) {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph for character " << c << std::endl;
            continue;
        }

        // Generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character
        Character character = {texture,
                               glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                               glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                               static_cast<unsigned int>(face->glyph->advance.x)};
        impl_->characters.insert(std::pair<char, Character>(c, character));
    }

    // Clean up FreeType resources
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return true;
}

bool FontManager::loadFonts(FT_Library ft, FT_Face& face) {
// Platform-specific font paths
#if defined(__APPLE__)
    const char* fontPaths[] = {
        "/System/Library/Fonts/HelveticaNeue.ttc", "/System/Library/Fonts/Geneva.ttf",
        "/System/Library/Fonts/SFNSMono.ttf", "/System/Library/Fonts/Helvetica.ttc"};
#elif defined(_WIN32)
    const char* fontPaths[] = {"C:\\Windows\\Fonts\\arial.ttf", "C:\\Windows\\Fonts\\calibri.ttf"};
#else  // Linux
    const char* fontPaths[] = {"/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
                               "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"};
#endif

    const int numPaths = sizeof(fontPaths) / sizeof(fontPaths[0]);

    // Try loading fonts in order until one succeeds
    for (int i = 0; i < numPaths; i++) {
        std::cout << "Trying to load font: " << fontPaths[i] << std::endl;
        if (FT_New_Face(ft, fontPaths[i], 0, &face) == 0) {
            std::cout << "Successfully loaded font: " << fontPaths[i] << std::endl;
            return true;
        }
    }

    std::cerr << "ERROR::FREETYPE: Failed to load any font" << std::endl;
    return false;
}

void FontManager::createFallbackFont() {
    // Skip in headless mode
    if (impl_->renderMode == RenderMode::Headless) {
        return;
    }

    // Create a simple fallback character - just a square for each character
    for (unsigned char c = 0; c < 128; c++) {
        // Create a minimal character entry with a tiny texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Create a small 8x8 white square
        unsigned char buffer[64];
        std::fill_n(buffer, 64, 255);  // Fill with white

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 8, 8, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character with minimal dimensions
        Character character = {
            texture, glm::ivec2(8, 8),  // Size
            glm::ivec2(0, 8),           // Bearing
            8 << 6                      // Advance (8 pixels, shifted by 6 bits - FreeType format)
        };

        impl_->characters.insert(std::pair<char, Character>(c, character));
    }
}

const Character* FontManager::getCharacter(char c) const {
    auto it = impl_->characters.find(c);
    if (it != impl_->characters.end()) {
        return &it->second;
    }
    return nullptr;
}

bool FontManager::hasCharacter(char c) const {
    return impl_->characters.find(c) != impl_->characters.end();
}

bool FontManager::isInitialized() const {
    return impl_->initialized;
}

}  // namespace visualization