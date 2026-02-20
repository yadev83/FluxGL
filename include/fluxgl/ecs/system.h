#pragma once

namespace fluxgl {
    class Scene;

    class System {
        public:
            virtual ~System() = default;
            virtual void onInit(Scene& scene) {}
            virtual void onUpdate(Scene& scene, float dt) {}
    };
}