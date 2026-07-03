#include <fluxgl/core/app.h>
#include <fluxgl/core/scene.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/ecs/behavior.h>
#include <fluxgl/assets/prefab_loader.h>

namespace fluxgl {
    Entity Scene::createEntity() { 
        return m_registry.createEntity();
    }

    Entity Scene::instantiate(Prefab& prefab) {
        return getContext().prefabLoader.instantiate(&m_registry, prefab);
    }

    void Scene::destroyEntity(Entity entity) {
        m_registry.destroyEntity(entity.getID());
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
    
    Entity Scene::getEntity(EntityID id) {
        return m_registry.getEntity(id);
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