#include <fluxgl/ui/font.h>
#include <fluxgl/graphics/texture.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

namespace fluxgl {
    Font::~Font() {
        delete m_atlas.texture;
    }

    Font Font::loadFromMemory(Buffer data) {
        Font font;

        constexpr int AtlasWidth = 512;
        constexpr int AtlasHeight = 512;
        unsigned char bitmap[AtlasWidth * AtlasHeight];

        stbtt_pack_context context;

        stbtt_PackBegin(
            &context,
            bitmap,
            AtlasWidth,
            AtlasHeight,
            0,
            1,
            nullptr
        );

        stbtt_packedchar chars[96];
        stbtt_PackFontRange(
            &context,
            data.data(),
            0,
            32.0f,
            32,
            96,
            chars
        );
        stbtt_PackEnd(&context);

        // Generate glyphs
        for(int i = 0; i < 96; ++i) {
            const auto& c = chars[i];

            Glyph glyph;

            glyph.u0 = c.x0 / float(AtlasWidth);
            glyph.v0 = c.y0 / float(AtlasHeight);

            glyph.u1 = c.x1 / float(AtlasWidth);
            glyph.v1 = c.y1 / float(AtlasHeight);

            glyph.width = c.x1 - c.x0;
            glyph.height = c.y1 - c.y0;

            glyph.bearingX = c.xoff;
            glyph.bearingY = c.yoff;

            glyph.advance = c.xadvance;

            font.m_atlas.glyphs[char(i + 32)] = glyph;
        }

        // Generate Atlas Texture
        font.m_atlas.width = AtlasWidth;
        font.m_atlas.height = AtlasHeight;
        font.m_atlas.fontSize = 32;
        font.m_atlas.texture = new Texture();
        font.m_atlas.texture->load(
            reinterpret_cast<const char*>(bitmap),
            AtlasWidth,
            AtlasHeight,
            1
        );

        return font;
    }

    Font::Font(Font&& other) noexcept {
        m_atlas.texture = other.m_atlas.texture;
        other.m_atlas.texture = nullptr;

        m_atlas.width = other.m_atlas.width;
        m_atlas.height = other.m_atlas.height;
        m_atlas.fontSize = other.m_atlas.fontSize;
        m_atlas.glyphs = std::move(other.m_atlas.glyphs);
    }


    Font& Font::operator=(Font&& other) noexcept {
        if(this != &other) {
            delete m_atlas.texture;

            m_atlas.texture = other.m_atlas.texture;
            other.m_atlas.texture = nullptr;

            m_atlas.width = other.m_atlas.width;
            m_atlas.height = other.m_atlas.height;
            m_atlas.fontSize = other.m_atlas.fontSize;
            m_atlas.glyphs = std::move(other.m_atlas.glyphs);
        }

        return *this;
    }
}