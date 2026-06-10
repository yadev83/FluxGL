#include <fluxgl/ecs/systems/lifetime_system.h>
#include <fluxgl/ecs/components/lifetime.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/core/scene.h>

#include <set>

namespace fluxgl {
    void LifetimeSystem::onLateUpdate(Scene& scene, float dt) {
        auto& registry = scene.getRegistry();

        std::set<EntityID> entitiesToDelete; // Set to avoid removing the same entity multiple times
        for(auto e : registry.query<Lifetime>()) {
            auto& lifetime = e.getComponent<Lifetime>();
            lifetime.remaining -= dt;

            if(lifetime.remaining <= 0.0f) {
                entitiesToDelete.insert(e.getID());
            }
        }

        // Don't delete from the registry while iterating over it
        for(auto toDelete : entitiesToDelete) registry.destroyEntity(toDelete);
    }
}