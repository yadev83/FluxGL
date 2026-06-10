#pragma once

#include <fluxgl/ecs/system.h>

namespace fluxgl {
    class LifetimeSystem : public System {
        public:
            void onLateUpdate(Scene& scene, float dt) override; 
    };
}