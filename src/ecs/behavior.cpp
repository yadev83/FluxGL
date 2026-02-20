#include <fluxgl/ecs/behavior.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    Scene& Behavior::getScene() { return *m_scene; }
    AppContext& Behavior::getContext() { return m_scene->getContext(); }
    Entity Behavior::getEntity() { return m_scene->getEntity(m_entityID); }

    void Behavior::attachEntity(EntityID id) { m_entityID = id; }
    void Behavior::attachScene(Scene* scene) { m_scene = scene; }
    bool Behavior::isInitialized() { return m_entityID > 0 && m_scene != nullptr; }
}