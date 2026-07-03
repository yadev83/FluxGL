#include <fluxgl/core/app.h>
#include <fluxgl/core/log.h>
#include <fluxgl/core/scene.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/ecs/components/transform.h>
#include <fluxgl/ecs/components/sprite.h>
#include <fluxgl/ecs/components/box_collider.h>
#include <fluxgl/ecs/components/rigidbody.h>
#include <fluxgl/ecs/behavior.h>
#include <fluxgl/assets/prefab_loader.h>
#include <fluxgl/assets/level.h>

namespace fluxgl {
    Entity Scene::createEntity() { 
        return m_registry.createEntity();
    }

    Entity Scene::instantiate(Prefab& prefab) {
        return getContext().prefabLoader.instantiate(&m_registry, prefab);
    }
   
    Entity Scene::getEntity(EntityID id) {
        return m_registry.getEntity(id);
    }

    void Scene::destroyEntity(Entity entity) {
        m_registry.destroyEntity(entity.getID());
    }

    void Scene::buildLevel(const Level& levelData) {
        glm::vec3 origin = {0.0f, 0.0f, 0.0f}; // Starting position for the level

        for (size_t y = levelData.getTileMap().size(); y-- > 0;) {
            const TileRow& row = levelData.getTileMap()[y];
            float worldY = origin.y + static_cast<float>((levelData.getTileMap().size() - 1 - y) * levelData.getTileHeight());

            for (size_t x = 0; x < row.size(); ++x) {
                const TileID& tileID = row[x];
                float worldX = origin.x + static_cast<float>(x * levelData.getTileWidth());

                auto it = levelData.getTileDefinitions().find(tileID);
                if (it != levelData.getTileDefinitions().end()) {
                    const TileDefinition& definition = it->second;
                    Entity tileEntity = getContext().prefabLoader.instantiate(&m_registry, *(getContext().resourceManager.getResource<Prefab>(definition.prefab)));
                    tileEntity.getComponent<Transform>().position = origin + glm::vec3({worldX, worldY, 0.0f});
                    tileEntity.getComponent<Sprite>().size = {levelData.getTileWidth(), levelData.getTileHeight()};
                    tileEntity.getComponent<BoxCollider2D>().halfSize = tileEntity.getComponent<Sprite>().size * 0.5f;
                    tileEntity.getComponent<RigidBody2D>().type = RigidBodyType::Static;

                    FLUXGL_LOG_DEBUG("Instantiated tile entity with ID: " + std::to_string(tileEntity.getID()) + " at position: (" + std::to_string(tileEntity.getComponent<fluxgl::Transform>().position.x) + ", " + std::to_string(tileEntity.getComponent<fluxgl::Transform>().position.y) + ")");
                } else if(tileID != "0" && tileID != " ") {
                    // Ignore empty tiles
                    FLUXGL_LOG_WARNING("Tile ID '" + tileID + "' not found in tile definitions.");
                }
            }
        }

        for(const auto& entityDef : levelData.getEntities()) {
            fluxgl::Entity entity = instantiate(*(getContext().resourceManager.getResource<fluxgl::Prefab>(entityDef.prefab)));

            // Update transform position based on tile coordinates and offsets (if provided)
            auto& transform = entity.getComponent<Transform>();
            transform.position.x = entityDef.positionX != 0.0f ? entityDef.positionX : transform.position.x;
            transform.position.y = entityDef.positionY != 0.0f ? entityDef.positionY : transform.position.y;

            FLUXGL_LOG_DEBUG("Instantiated entity with ID: " + std::to_string(entity.getID()));
            FLUXGL_LOG_TRACE("Entity Transform: " + transform.toString());
        }
    }

    void Scene::setContext(AppContext* appContext) {
        m_context = appContext;
    }

    Registry& Scene::getRegistry() {
        return m_registry;
    }

    AppContext& Scene::getContext() {
        return *m_context;
    }

    void Scene::initBehaviors() {
        for(auto& behavior : m_registry.getAllBehaviors()) {
            if(!behavior->isInitialized()) {
                behavior->attachScene(this);
                behavior->onInit();
            }
        }
    }

    void Scene::updateBehaviors(float dt) {
        for(auto& behavior : m_registry.getAllBehaviors()) {
            if(!behavior->isInitialized()) {
                behavior->attachScene(this);
                behavior->onInit();
            }

            behavior->onUpdate(dt);
        }
    }

    void Scene::fixedUpdateBehaviors(float dt) {
        for(auto& behavior : m_registry.getAllBehaviors()) {
            behavior->onFixedUpdate(dt);
        }
    }

    void Scene::lateUpdateBehaviors(float dt) {
        for(auto& behavior : m_registry.getAllBehaviors()) {
            behavior->onLateUpdate(dt);
        }
    }

    void Scene::renderBehaviors(float dt) {
        for(auto& behavior : m_registry.getAllBehaviors()) {
            behavior->onRender(dt);
        }
    }

    void Scene::initSystems() {
        for(auto& system : m_systems) {
            system->onInit(*this);
        }
    }

    void Scene::updateSystems(float dt) {
        for(auto& system : m_systems) {
            system->onUpdate(*this, dt);
        }
    }

    void Scene::lateUpdateSystems(float dt) {
        for(auto& system : m_systems) {
            system->onLateUpdate(*this, dt);
        }
    }

    void Scene::fixedUpdateSystems(float dt) {
        for(auto& system : m_systems) {
            system->onFixedUpdate(*this, dt);
        }
    }

    void Scene::renderSystems(float dt) {
        for(auto& system : m_systems) {
            system->onRender(*this, dt);
        }
    }

    void Scene::clearSystems() {
        for(auto& system : m_systems) {
            delete system;
        }
        m_systems.clear();
    }
}