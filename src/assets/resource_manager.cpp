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

    TextureHandle ResourceManager::findTexture(const Resource& texture) {
        return m_textureStorage.find(texture);
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

    Texture* ResourceManager::getTexture(const Resource& texture) {
        return getTexture(findTexture(texture));
    }

    std::vector<Texture*> ResourceManager::getTextures(const std::vector<Resource>& textures) {
        std::vector<TextureHandle> handles;
        for(Resource texture : textures) {
            handles.push_back(findTexture(texture));
        }

        return getTextures(handles);
    }

    ShaderHandle ResourceManager::addShader(const std::string& name, Shader* shader) {
        return m_shaderStorage.add(shader, name);
    }

    ShaderHandle ResourceManager::addShader(const std::string& name, Shader shader) {
        Shader* newShader = new Shader(std::move(shader));
        return m_shaderStorage.add(newShader, name);
    }

    ShaderHandle ResourceManager::findShader(const Resource& shader) {
        return m_shaderStorage.find(shader);
    }

    Shader* ResourceManager::getShader(ShaderHandle handle) {
        return m_shaderStorage.get(handle);
    }

    Shader* ResourceManager::getShader(const Resource& shader) {
        return getShader(findShader(shader));
    }

    MeshHandle ResourceManager::addMesh(const std::string& name, Mesh* mesh) {
        return m_meshStorage.add(mesh, name);
    }

    MeshHandle ResourceManager::addMesh(const std::string& name, Mesh mesh) {
        Mesh* newMesh = new Mesh(std::move(mesh));
        return m_meshStorage.add(newMesh, name);
    }

    MeshHandle ResourceManager::findMesh(const Resource& mesh) {
        return m_meshStorage.find(mesh);
    }

    Mesh* ResourceManager::getMesh(MeshHandle handle) {
        return m_meshStorage.get(handle);
    }

    Mesh* ResourceManager::getMesh(const Resource& mesh) {
        return getMesh(findMesh(mesh));
    }

    SoundHandle ResourceManager::addSound(const std::string& name, Sound* sound) {
        return m_soundStorage.add(sound, name);
    }

    SoundHandle ResourceManager::addSound(const std::string& name, Sound sound) {
        Sound* newSound = new Sound(std::move(sound));
        return m_soundStorage.add(newSound, name);
    }

    SoundHandle ResourceManager::findSound(const Resource& Sound) {
        return m_soundStorage.find(Sound);
    }

    Sound* ResourceManager::getSound(SoundHandle handle) {
        return m_soundStorage.get(handle);
    }

    Sound* ResourceManager::getSound(const Resource& Sound) {
        return getSound(findSound(Sound));
    }
}