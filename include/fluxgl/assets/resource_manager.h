#pragma once

#include <string>
#include <vector>

#include <fluxgl/graphics/texture.h>
#include <fluxgl/graphics/shader.h>
#include <fluxgl/graphics/mesh.h>

namespace fluxgl {
    // Generic resource handle, can be used for any type of resource (e.g., textures, shaders)
    template<typename T>
    struct ResourceHandle {
        size_t      index = 0;
        uint32_t    generation = 0;
    };

    // Generic resource slot : holds the resource, its generation, and whether it's currently occupied
    template<typename T>
    struct ResourceSlot {
        T* resource = nullptr;
        uint32_t generation = 0;
        bool occupied = false;
    };

    template<typename T>
    class ResourceStorage {
        private:
            std::vector<ResourceSlot<T>> m_slots;

        public:
            ResourceHandle<T> add(T* resource);
            T* get(ResourceHandle<T> handle);
            void remove(ResourceHandle<T> handle);
    };

    using TextureHandle = ResourceHandle<Texture>;
    using ShaderHandle = ResourceHandle<Shader>;
    //using MeshHandle = ResourceHandle<Mesh>;

    // ResourceManager class to manage loading and getting resources
    class ResourceManager {
        private:
            ResourceStorage<Texture>    m_textureStorage;
            ResourceStorage<Shader>     m_shaderStorage;
            //ResourceStorage<Mesh>       m_meshStorage;

        public:
            TextureHandle loadTexture(const std::string& path);
            ShaderHandle loadShader(const std::string& vertexPath, const std::string& fragmentPath);
            //MeshHandle loadMesh(const std::string& path);

            Texture* getTexture(TextureHandle handle);
            Shader* getShader(ShaderHandle handle);
            //Mesh* getMesh(MeshHandle handle);
    };
}

#include "resource_manager.inl"