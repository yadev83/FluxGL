#pragma once

namespace fluxgl {
    using EntityID = unsigned int;
    class Entity;
    class Scene;
    class AppContext;

    class Behavior {
        private:
            EntityID m_entityID = 0;
            Scene*  m_scene = nullptr;

        protected:
            Scene& getScene();
            AppContext& getContext();
            Entity getEntity();

        public:
            virtual ~Behavior() = default;
            void attachEntity(EntityID id);
            void attachScene(Scene* scene);
            bool isInitialized();

            template<typename ComponentT>
            ComponentT& getComponent();

            template<typename ComponentT>
            void hasComponent();

            virtual void onInit() {}
            virtual void onUpdate(float dt) {}
    };
}

#include "behavior.inl"