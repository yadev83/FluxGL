#include <fluxgl/graphics/texture.h>
#include <fluxgl/core/log.h>
#include <fluxgl/core/error.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include <filesystem>
#include <string>

namespace fluxgl {
    void Texture::load(const char *data, int width, int height, int channels) {
        FLUXGL_LOG_DEBUG("Loading texture: " + std::to_string(width) + "x" + std::to_string(height) + " with " + std::to_string(channels) + " channels");

        m_width = width;
        m_height = height;
        m_channels = channels;
        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);

        FLUXGL_LOG_DEBUG("Texture loaded (ID: " + std::to_string(m_ID) + ")");
    }

    Texture::~Texture() {
        if(m_ID) {
            FLUXGL_LOG_DEBUG("Deleting texture (ID: " + std::to_string(m_ID) + ")");
            glDeleteTextures(1, &m_ID);
        }
    }

    Texture::Texture(Texture&& other) noexcept {
        m_ID = other.m_ID; 
        other.m_ID = 0; // Prevent deletion of the moved-from Texture
    }

    Texture& Texture::operator=(Texture&& other) noexcept {
        if (this != &other) {
            if(m_ID) {
                glDeleteTextures(1, &m_ID);
            }
            
            m_ID = other.m_ID;
            other.m_ID = 0;
        }
        return *this;
    }

    void Texture::bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

    void Texture::unbind() const {
        // TODO : Store current slot for this texture so we can unbind from the correct slot instead of always unbinding from slot 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture Texture::loadFromFile(const char *path) {
        if(!std::filesystem::exists(path)) {
            throw Error{ErrorCode::IOError, "Texture File does not exist: " + std::string(path)};
        }

        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
        
        Texture texture = loadFromMemory(reinterpret_cast<const char*>(data), width, height, channels);
        stbi_image_free(data);

        return texture;
    }

    Texture Texture::loadFromMemory(const char *data, int width, int height, int channels) {
        Texture texture;
        texture.load(data, width, height, channels); 
        return texture;
    }
}