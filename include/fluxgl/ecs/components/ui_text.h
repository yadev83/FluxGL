#pragma once

#include <glm/glm.hpp>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>
#include <fluxgl/assets/assets.h>
#include <fluxgl/ui/font.h>

namespace fluxgl {
    struct UIText {
        static constexpr const char* TypeName = "fluxgl::UIText";
        static void Create(Registry& registry, EntityID entityID, const Json::Value& data) {
            UIText text;
            
            // Parse the json data if it is a valid object, and go through its members to compute the text component
            if(data.isObject()) {
                text.text = data.get("text", "").asString();
                text.font = data.get("font", "").asString();
                text.shader = data.get("shader", "").asString();
                text.fontSize = data.get("fontSize", 32.0f).asFloat();
                text.maxWidth = data.get("maxWidth", 0.0f).asFloat();
                text.autoScale = data.get("autoScale", false).asBool();

                Json::Value colorData = data.get("color", Json::objectValue);
                text.color = {colorData.get("r", 1.0f).asFloat(), colorData.get("g", 1.0f).asFloat(), colorData.get("b", 1.0f).asFloat(), colorData.get("a", 1.0f).asFloat()};
            }

            registry.addComponent<UIText>(entityID, text);
        }

        std::string text;
        Resource font;
        Resource shader; 

        glm::vec4 color = {1, 1, 1, 1};

        float fontSize = 32.0f;

        float maxWidth = 0.0f;
        float maxHeight = 0.0f;
        bool autoScale = false;

        inline float getComputedFontSize(ResourceManager& resourceManager) {
            TextMetrics metrics = resourceManager.getResource<Font>(font)->measureText(text, fontSize);
            float width = metrics.width;
            float height = metrics.height;

            // Before printing text, compute the fontSize that we will be using if autoScale is set to match maxWidth
            float renderFontSize = fontSize;
            if(autoScale && maxWidth) {
                if(width > maxWidth) {
                    renderFontSize *= (maxWidth / width);
                }
            }

            // After that, check on the maxHeight as well, just in case (using the already updated fontsize from width if it happened)
            if(autoScale && maxHeight) {
                if(height > maxHeight) {
                    renderFontSize *= (maxHeight / height);
                }
            }

            return renderFontSize;
        }
    };
}