#include <fluxgl/core/scene.h>
#include "entity.h"

namespace fluxgl {
    template<typename ComponentT>
    ComponentT& Behavior::getComponent() {
        return getEntity().getComponent<ComponentT>();
    }

    template<typename ComponentT>
    void Behavior::hasComponent() {
        return getEntity().hasComponent<ComponentT>();
    }
}