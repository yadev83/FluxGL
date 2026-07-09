#include <fluxgl/ui/font.h>
#include <fluxgl/graphics/texture.h>
#include <fluxgl/core/error.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#include <cfloat>

#define FLUXGL_MAX_ATLAS_WIDTH 4096
#define FLUXGL_MAX_ATLAS_HEIGHT 4096

namespace fluxgl {
    Font::~Font() {
        delete m_atlas.texture;
    }

    bool Font::generatePack(
        Buffer data, 
        int atlasWidth, 
        int atlasHeight, 
        float sourceSize,
        std::vector<unsigned char>& bitmap,
        std::array<stbtt_packedchar, 96>& chars
    ) {
        bitmap.resize(atlasWidth * atlasHeight);
        stbtt_pack_context context;
        if(!stbtt_PackBegin(
            &context,
            bitmap.data(),
            atlasWidth,
            atlasHeight,
            0,
            1,
            nullptr
        )) {
            return false;
        }

        bool success = stbtt_PackFontRange(
            &context,
            data.data(),
            0,
            sourceSize,
            32,
            96,
            chars.data()
        );

        stbtt_PackEnd(&context);

        return success;
    }

    Font Font::loadFromMemory(Buffer data, float sourceSize) {
        Font font;

        int atlasWidth = 512;
        int atlasHeight = 512;
        std::vector<unsigned char> bitmap;
        std::array<stbtt_packedchar, 96> chars;

        while(!font.generatePack(
            data,
            atlasWidth,
            atlasHeight,
            sourceSize,
            bitmap,
            chars
        )) {
            if(atlasWidth <= atlasHeight) {
                atlasWidth *= 2;
            } else {
                atlasHeight *= 2;
            }

            if(atlasWidth > FLUXGL_MAX_ATLAS_WIDTH || atlasHeight > FLUXGL_MAX_ATLAS_HEIGHT) {
                throw Error{ErrorCode::Error, "Unable to fit font atlas"};
            }
        }

        // Generate glyphs
        for(int i = 0; i < 96; ++i) {
            const auto& c = chars[i];

            Glyph glyph;

            glyph.u0 = c.x0 / float(atlasWidth);
            glyph.v0 = c.y0 / float(atlasHeight);

            glyph.u1 = c.x1 / float(atlasWidth);
            glyph.v1 = c.y1 / float(atlasHeight);

            glyph.width = c.x1 - c.x0;
            glyph.height = c.y1 - c.y0;

            glyph.bearingX = c.xoff;
            glyph.bearingY = c.yoff;

            glyph.advance = c.xadvance;
            glyph.sourceSize = sourceSize;

            font.m_atlas.glyphs[char(i + 32)] = glyph;
        }

        // Generate Atlas Texture
        font.m_atlas.width = atlasWidth;
        font.m_atlas.height = atlasHeight;
        font.m_atlas.fontSize = sourceSize;
        font.m_atlas.texture = new Texture();
        font.m_atlas.texture->load(
            reinterpret_cast<const char*>(bitmap.data()),
            atlasWidth,
            atlasHeight,
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

    TextMetrics Font::measureText(const std::string& string, float fontSize) {
       TextMetrics metrics;

        float scale = fontSize / m_atlas.fontSize;
        float lineHeight = fontSize;

        float cursorX = 0.0f;
        float cursorY = 0.0f;

        float minX = FLT_MAX;
        float minY = FLT_MAX;
        float maxX = -FLT_MAX;
        float maxY = -FLT_MAX;

        int lines = 1;
        
        for(char c : string) {
            if(c == '\n') {
                cursorX = 0.0f;
                cursorY += lineHeight;
                lines++;
                continue;
            }

            const Glyph* glyph = getGlyph(c);
            if(!glyph)
                continue;

            float glyphScale = fontSize / glyph->sourceSize;

            float x0 = cursorX + glyph->bearingX * glyphScale;
            float y0 = cursorY + glyph->bearingY * glyphScale;

            float x1 = x0 + glyph->width * glyphScale;
            float y1 = y0 + glyph->height * glyphScale;

            minX = std::min(minX, x0);
            minY = std::min(minY, y0);

            maxX = std::max(maxX, x1);
            maxY = std::max(maxY, y1);

            cursorX += glyph->advance * glyphScale;
        }

        if(maxX == -FLT_MAX) {
            metrics.width = 0;
            metrics.height = 0;
            return metrics;
        }

        metrics.width = maxX - minX;
        metrics.height = lines * lineHeight;

        metrics.boundsMin = {minX, minY};
        metrics.boundsMax = {maxX, maxY};

        metrics.lines = lines;
        metrics.lineHeight = lineHeight;

        return metrics;
    }
}