#pragma once

#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/system.h>
#include <algorithm>

namespace fluxgl {
    struct AppContext;
    struct Prefab;

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
            void destroyEntity(Entity entity);

        public:
            void setContext(AppContext* appContext);
            Registry& getRegistry();
            AppContext& getContext();
            Entity createEntity();
            Entity instantiate(const std::string& path);
            Entity instantiate(Prefab& prefab);
            Entity getEntity(EntityID id);

            template<typename S, typename... Args>
            void registerSystem(Args&&... args) {
                m_systems.push_back(new S(std::forward<Args>(args)...));
            }

            template<typename S>
            bool hasSystem() {
                for(System* sys : m_systems) {
                    if(dynamic_cast<S*>(sys) != nullptr) return true;
                }
                
                return false;            
            }

            template<typename S>
            S& getSystem() {
                for(System* sys : m_systems) {
                    if(auto casted = dynamic_cast<S*>(sys))
                        return *casted;
                }

                throw std::runtime_error("System not found");
            }

            void initSystems();
            void updateSystems(float dt);
            void fixedUpdateSystems(float dt);
            void lateUpdateSystems(float dt);
            void renderSystems(float dt);

            void initBehaviors();
            void updateBehaviors(float dt);
            void fixedUpdateBehaviors(float dt);
            void lateUpdateBehaviors(float dt);
            void renderBehaviors(float dt);

            virtual ~Scene() = default;
            virtual void onLoad() {}
            virtual void onDestroy() {}
            virtual void onInit() {}
            virtual void onUpdate(float dt) {}
            virtual void onFixedUpdate(float dt) {}
            virtual void onLateUpdate(float dt) {}
            virtual void onRender(float dt) {}
    };
}