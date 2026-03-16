#pragma once

namespace fluxgl {
    class Scene;

    class System {
        public:
            virtual ~System() = default;
            virtual void onInit(Scene& scene) {}
            virtual void onUpdate(Scene& scene, float dt) {}
            virtual void onFixedUpdate(Scene& scene, float dt) {}
            virtual void onLateUpdate(Scene& scene, float dt) {}
            virtual void onRender(Scene& scene, float dt) {}
    };
}