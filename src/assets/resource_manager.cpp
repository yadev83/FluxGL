#include <fluxgl/assets/resource_manager.h>

namespace fluxgl {
    TextureHandle ResourceManager::loadTexture(const std::string& path) {
        Texture* texture = new Texture();
        texture->loadFromFile(path.c_str());

        return m_textureStorage.add(texture);
    }

    TextureHandle ResourceManager::addTexture(Texture* texture) {
        return m_textureStorage.add(texture);
    }

    TextureHandle ResourceManager::addTexture(Texture texture) {
        Texture* newTexture = new Texture(std::move(texture));
        return m_textureStorage.add(newTexture);
    }

    ShaderHandle ResourceManager::loadShader(const std::string& vertexPath, const std::string& fragmentPath) {
        Shader* shader = new Shader();
        shader->loadFromFiles(vertexPath.c_str(), fragmentPath.c_str());

        return m_shaderStorage.add(shader);
    }

    ShaderHandle ResourceManager::addShader(Shader* shader) {
        return m_shaderStorage.add(shader);
    }

    ShaderHandle ResourceManager::addShader(Shader shader) {
        Shader* newShader = new Shader(std::move(shader));
        return m_shaderStorage.add(newShader);
    }

    Texture* ResourceManager::getTexture(TextureHandle handle) {
        return m_textureStorage.get(handle);
    }

    Shader* ResourceManager::getShader(ShaderHandle handle) {
        return m_shaderStorage.get(handle);
    }
}