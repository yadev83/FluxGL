#include <fluxgl/core/scene_manager.h>
#include <fluxgl/core/log.h>

namespace fluxgl {
    SceneManager& SceneManager::get() {
        static SceneManager instance;
        return instance;
    }

    void SceneManager::setContext(AppContext* appContext) { 
        m_appContext = appContext; 
    }

    void SceneManager::loadScene(std::string name) {
        auto it = m_scenes.find(name);
        if(it != m_scenes.end()) {
            m_nextScene = it->second;
        } else {
            FLUXGL_LOG_WARNING("Scene " + name + " not found");
        }
    }

    void SceneManager::update(float dt) {
        if(!m_currentScene && !m_nextScene && !m_scenes.empty()) {
            m_nextScene = m_scenes.begin()->second;
        }

        if(m_nextScene) {
            if(m_currentScene) m_currentScene->onDestroy();

            m_currentScene = m_nextScene;
            m_currentScene->setContext(m_appContext);
            m_currentScene->onLoad();
            m_initialized = false;
            m_nextScene = nullptr;
        }

        if(m_currentScene) {
            if(!m_initialized) {
                m_currentScene->onInit();
                m_currentScene->initBehaviors();
                m_currentScene->initSystems();
                m_initialized = true;
            }

            m_currentScene->onUpdate(dt);
            m_currentScene->updateBehaviors(dt);
            m_currentScene->updateSystems(dt);
        }
    }

    Scene *SceneManager::getCurrentScene() const { return m_currentScene; }
}