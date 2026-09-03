#pragma once

#include <fluxgl/ecs/entity.h>
#include <glm/glm.hpp>

namespace fluxgl {
    class Follow2DSystem;
    
    struct Follow2D {
        friend Follow2DSystem;

        EntityID targetID = 0;
        glm::vec2 deadzone = {0.0f, 0.0f};
        glm::vec2 offset = {0.0f, 0.0f};

        float smoothing = 0.0f;
        float lookAheadDistance = 0.0f;
        float lookAheadSmoothing = 0.0f;

        Follow2D(EntityID id) : targetID(id) {}

        private:
            glm::vec2 m_previousTargetPos = {0.0f, 0.0f};
            glm::vec2 m_lookAhead = {0.0f, 0.0f};
    };
}