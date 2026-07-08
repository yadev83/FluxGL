#pragma once

#include <fluxgl/core/virtual_file_system.h>
#include <stb_truetype.h>

#include <vector>
#include <array>
#include <unordered_map>

namespace fluxgl {
    struct Texture;

    /**
     * @brief A Glyph is a compilation of UVs, offsets and bearing settings 
     * We use glyphs to find a character in a Font Atlas
     */
    struct Glyph {
        float u0, v0;
        float u1, v1;

        int width, height; // Width/Heighgt of the glyph (ignoring empty space around it)
        int bearingX, bearingY; // Bearing is the "empty space" on the Left / Top for a glyph, based on it's origin point

        float advance; // Is the full width including bearings of a glyph
        float sourceSize; // Internal font size used for the atlas rasterization
    };

    /**
     * @brief A FontAtlas is a matrix texture in which we find multiple smaller glyph textures
     * Using glyph UVs and settings, we can extract a character from a FontAtlas 
     */
    struct FontAtlas {
        Texture *texture = nullptr; // A texture used to hold the FontAtlas built from a font face

        int width, height;  // The width/height of our Atlas
        float fontSize;     // Necessary setting => Implies that changing font size means re-creating another Atlas for a given font

        std::unordered_map<char, Glyph> glyphs; // A storage for glyph settings associated with their char counterpart
    };

    struct TextMetrics {
        float width;
        float height;
        float offsetX;
        float offsetY;
    };

    class Font {
        private:
            FontAtlas m_atlas;
            bool generatePack(
                Buffer data, 
                int atlasWidth, 
                int atlasHeight, 
                float sourceSize,
                std::vector<unsigned char>& bitmap,
                std::array<stbtt_packedchar, 96>& chars
            );

        public:
            Font() = default;
            ~Font();

            inline const Glyph* getGlyph(char c) const {
                auto it = m_atlas.glyphs.find(c);
                if(it == m_atlas.glyphs.end()) {
                    return nullptr;
                }

                return &it->second;
            }
            inline Texture* getTexture() const { return m_atlas.texture; }

            // Delete copy constructor and copy assignment operator to prevent copying 
            Font(const Font&) = delete; 
            Font& operator=(const Font&) = delete;
            // Instead, allow move
            Font(Font&& other) noexcept;
            Font& operator=(Font&& other) noexcept;

            // Utils
            TextMetrics measureText(const std::string& string, float fontSize = 32.0f);

            // Builder
            static Font loadFromMemory(Buffer data, float sourceSize = 64.0f);
    };
}