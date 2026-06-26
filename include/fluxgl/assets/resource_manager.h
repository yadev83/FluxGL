#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <fluxgl/graphics/texture.h>
#include <fluxgl/graphics/shader.h>
#include <fluxgl/graphics/mesh.h>
#include <fluxgl/assets/assets.h>

namespace fluxgl { 
    /** 
     * Generic resource handle, can be used for any type of resource (e.g., textures, shaders)
     * 
     * The Handle holds the index in the resource storage and a generation count to ensure validity.
     * If a slot is used by another resource after deletion, the generation count will differ, making the handle invalid.
     * We manipulate handles instead of pointers / data directly to ensure safety and movability of resources in memory.
     * When needed we use the resourceManager (stored in the app context) to get the actual resource from its handle.
    */
    template<typename T>
    struct ResourceHandle {
        static constexpr size_t INVALID_INDEX = std::numeric_limits<size_t>::max();

        size_t      index = 0;
        uint32_t    generation = 0;

        bool isValid() const {
            return index != INVALID_INDEX;
        }
    };

    /** 
     * Generic resource slot : holds the resource, its generation, and whether it's currently occupied
     * 
     * A slot is a container for a resource in the ResourceStorage. It keeps track of whether it's occupied and its generation count.
     * When a resource is added, it fills an available slot. 
     * When a resource is removed, the slot is marked as unoccupied and its generation is incremented to invalidate existing handles.
     * This system allows for efficient reuse of slots while ensuring that handles remain valid only as long as the resource they point to exists.
     * 
     * The resource manager uses these slots to manage resources like textures, shaders, and meshes
    */
    template<typename T>
    struct ResourceSlot {
        T* resource = nullptr;
        uint32_t generation = 0;
        bool occupied = false;
    };

    /**
     * ResourceStorage class template to manage a specific type of resource (e.g., textures, shaders)
     * 
     * This class manages a collection of resources of a specific type. It provides methods to add, get, find, and remove resources.
     * add => Adds a resource to the storage, returning a handle to it. If a name is provided, it maps the name to the resource for easy retrieval.
     * get => Retrieves a resource based on its handle. It checks the validity of the handle by comparing the generation count and occupancy status.
     * find => Finds a resource by its name, returning its handle. If the resource is not found, it throws an exception.
     * findAll => Returns a list of handles for all occupied resources in the storage.
     * remove => Removes a resource based on its handle. It marks the slot as unoccupied and increments the generation count to invalidate existing handles.
     */
    template<typename T>
    class ResourceStorage {
        private:
            std::vector<ResourceSlot<T>> m_slots;
            std::unordered_map<Resource, size_t> m_nameToIndex;

        public:
            T* get(ResourceHandle<T> handle);
            ResourceHandle<T> find(const Resource& name);
            std::vector<ResourceHandle<T>> findAll();

            ResourceHandle<T> add(T* resource, const Resource& name = "");
            void remove(ResourceHandle<T> handle);
    };

    using TextureHandle = ResourceHandle<Texture>;
    using ShaderHandle = ResourceHandle<Shader>;
    using MeshHandle = ResourceHandle<Mesh>;

    /**
     * @brief The ResourceManager class is responsible for managing all resources in the application, including textures, shaders, and meshes. It provides methods to add, retrieve, find, and remove resources using handles.
     * 
     * The ResourceManager uses ResourceStorage to manage each type of resource. It ensures that resources are properly cleaned up when the manager is destroyed.
     * It provides a centralized interface for resource management, allowing for efficient loading, retrieval, and cleanup of resources throughout the application. 
     * Resources can be accessed by their handles, which are returned when adding resources to the manager.
     * 
     * The common usage pattern is to load resources in the onLoad() method of a scene, and then retrieve the actual resources when needed
     * Either by their handle if they were stored or by finding the resource handle by its name first, then calling the getter.
     */
    class ResourceManager {
        private:
            ResourceStorage<Texture>    m_textureStorage;
            ResourceStorage<Shader>     m_shaderStorage;
            ResourceStorage<Mesh>       m_meshStorage;

        public:
            ~ResourceManager();

            TextureHandle addTexture(const std::string& name, Texture* texture);
            TextureHandle addTexture(const std::string& name, Texture texture);
            TextureHandle findTexture(const Resource& texture);
            Texture* getTexture(TextureHandle handle);
            std::vector<Texture*> getTextures(const std::vector<TextureHandle>& handles);
            Texture* getTexture(const Resource& texture);
            std::vector<Texture*> getTextures(const std::vector<Resource>& textures);

            ShaderHandle addShader(const std::string& name, Shader* shader);
            ShaderHandle addShader(const std::string& name, Shader shader);
            ShaderHandle findShader(const Resource& shader);
            Shader* getShader(ShaderHandle handle);
            Shader* getShader(const Resource& shader);

            MeshHandle addMesh(const std::string& name, Mesh* mesh);
            MeshHandle addMesh(const std::string& name, Mesh mesh);
            MeshHandle findMesh(const Resource& mesh);
            Mesh* getMesh(MeshHandle handle);
            Mesh* getMesh(const Resource& mesh);
    };
}

#include "resource_manager.inl"