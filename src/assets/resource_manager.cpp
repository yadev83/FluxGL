#include <fluxgl/assets/resource_manager.h>

namespace fluxgl {
    ResourceManager::~ResourceManager() {
        for (auto& handle : m_textureStorage.findAll()) m_textureStorage.remove(handle);
        for (auto& handle : m_shaderStorage.findAll()) m_shaderStorage.remove(handle);
        for (auto& handle : m_meshStorage.findAll()) m_meshStorage.remove(handle);
    }

    TextureHandle ResourceManager::addTexture(const std::string& name, Texture* texture) {
        return m_textureStorage.add(texture, name);
    }

    TextureHandle ResourceManager::addTexture(const std::string& name, Texture texture) {
        Texture* newTexture = new Texture(std::move(texture));
        return m_textureStorage.add(newTexture, name);
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
    
    TextureHandle ResourceManager::findTexture(const std::string& name) {
        return m_textureStorage.find(name);
    }

    Texture* ResourceManager::getTexture(TextureHandle handle) {
        return m_textureStorage.get(handle);
    }

    ShaderHandle ResourceManager::addShader(const std::string& name, Shader* shader) {
        return m_shaderStorage.add(shader, name);
    }

    ShaderHandle ResourceManager::addShader(const std::string& name, Shader shader) {
        Shader* newShader = new Shader(std::move(shader));
        return m_shaderStorage.add(newShader, name);
    }

    ShaderHandle ResourceManager::findShader(const std::string& name) {
        return m_shaderStorage.find(name);
    }

    Shader* ResourceManager::getShader(ShaderHandle handle) {
        return m_shaderStorage.get(handle);
    }

    MeshHandle ResourceManager::addMesh(const std::string& name, Mesh* mesh) {
        return m_meshStorage.add(mesh, name);
    }

    MeshHandle ResourceManager::addMesh(const std::string& name, Mesh mesh) {
        Mesh* newMesh = new Mesh(std::move(mesh));
        return m_meshStorage.add(newMesh, name);
    }

    MeshHandle ResourceManager::findMesh(const std::string& name) {
        return m_meshStorage.find(name);
    }

    Mesh* ResourceManager::getMesh(MeshHandle handle) {
        return m_meshStorage.get(handle);
    }
}