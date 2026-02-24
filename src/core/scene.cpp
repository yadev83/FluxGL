#include <fluxgl/core/scene.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/ecs/behavior.h>

namespace fluxgl {
    Entity Scene::createEntity() { 
        return m_registry.createEntity();
    }

    void Scene::destroyEntity(Entity entity) {
        m_registry.destroyEntity(entity.getID());
    }

    void Scene::setContext(AppContext* appContext) {
        context = appContext;
    }

    Registry& Scene::getRegistry() {
        return m_registry;
    }

    AppContext& Scene::getContext() {
        return *context;
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
}