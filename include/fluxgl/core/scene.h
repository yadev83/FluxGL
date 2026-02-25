#pragma once

#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/system.h>

namespace fluxgl {
    struct AppContext;

    /**
     * @brief Scene base class
     * 
     * Every scene in a project must inherit from this class
     * It should then be registered in the singleton class "SceneManager" before starting the app
     * It holds a Registry and a list of Systems for the ECS implementation, and gives acces to the AppContext (window / input)
     */
    class Scene {
        private:
            Registry m_registry;
            std::vector<System*> m_systems;

        protected:
            AppContext* context = nullptr;

            Entity createEntity();
            void destroyEntity(Entity entity);

        public:
            void setContext(AppContext* appContext);
            Registry& getRegistry();
            AppContext& getContext();
            Entity getEntity(EntityID id);

            template<typename S, typename... Args>
            void registerSystem(Args&&... args) {
                m_systems.push_back(new S(std::forward<Args>(args)...));
            }

            void initSystems();
            void updateSystems(float dt);

            void initBehaviors();
            void updateBehaviors(float dt);

            virtual ~Scene() = default;
            virtual void onLoad() {}
            virtual void onDestroy() {}
            virtual void onInit() {}
            virtual void onUpdate(float dt) {}
    };
}