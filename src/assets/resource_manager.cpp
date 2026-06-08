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

    MeshHandle ResourceManager::addMesh(Mesh* mesh) {
        return m_meshStorage.add(mesh);
    }

    MeshHandle ResourceManager::addMesh(Mesh mesh) {
        Mesh* newMesh = new Mesh(std::move(mesh));
        return m_meshStorage.add(newMesh);
    }

    Texture* ResourceManager::getTexture(TextureHandle handle) {
        return m_textureStorage.get(handle);
    }

    std::vector<Texture*> ResourceManager::getTextures(const std::vector<TextureHandle>& handles) {
        std::vector<Texture*> textures;
        textures.reserve(handles.size());

        for(const auto& handle : handles) {
            Texture* texture = m_textureStorage.get(handle);
            if(texture) {
                textures.push_back(texture);
            } else {
                textures.push_back(nullptr); // Or handle this case as needed
            }
        }

        return textures;
    }

    Shader* ResourceManager::getShader(ShaderHandle handle) {
        return m_shaderStorage.get(handle);
    }

    Mesh* ResourceManager::getMesh(MeshHandle handle) {
        return m_meshStorage.get(handle);
    }
}