#pragma once

#include <fluxgl/ecs/system.h>

namespace fluxgl {
    class AudioSystem : public System {
        public:
            void onUpdate(Scene& scene, float dt) override;
    };
}