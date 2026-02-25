#pragma once

#include <string>
#include <unordered_map>
#include "scene.h"

namespace fluxgl {
    struct AppContext;

    /**
     * @brief A Scene orchestrator used by the main App class
     * 
     * This class is responsible for loading, unloading and updating scenes
     * It also handles the transition between scenes
     * It is a singleton, so it can be statically accessed and only ONE instance can be created at a time in an application
     */
    class SceneManager {
        private:
            fluxgl::AppContext* m_appContext = nullptr;
            std::unordered_map<std::string, Scene*> m_scenes;

            Scene* m_currentScene = nullptr;
            Scene* m_nextScene = nullptr;
            bool m_initialized = false;

            SceneManager() = default;

        public:
            static SceneManager& get();
            void setContext(fluxgl::AppContext* appContext);

            SceneManager(const SceneManager&) = delete;
            SceneManager& operator=(const SceneManager&) = delete;

            template<typename S>
            void registerScene(std::string name) {
                m_scenes[name] = new S();
            }

            void loadScene(std::string name);
            void update(float dt);
            Scene *getCurrentScene() const;
    };
}