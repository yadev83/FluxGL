#pragma once

#include <fluxgl/ecs/system.h>

namespace fluxgl {
    class RenderSystem : public System {
        public:
            void onUpdate(Scene& scene, float dt) override;
    };
}