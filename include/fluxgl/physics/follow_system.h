#pragma once

#include <fluxgl/ecs/system.h>

namespace fluxgl {
    class Follow2DSystem : public System {
        void onUpdate(Scene& scene, float dt) override;
    };
}