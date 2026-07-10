#pragma once

#include <glm/glm.hpp>

#include <json/json.h>
#include <fluxgl/ecs/registry.h>
#include <fluxgl/ecs/entity.h>

namespace fluxgl {
    enum class UIAlignment {
        TopLeft,
        TopCenter,
        TopRight,

        CenterLeft,
        Center,
        CenterRight,

        BottomLeft,
        BottomCenter,
        BottomRight
    };

    inline glm::vec2 toAlignment(UIAlignment alignment) {
        switch(alignment) {
            case UIAlignment::TopLeft:      return {0.0f, 0.0f};
            case UIAlignment::TopCenter:    return {0.5f, 0.0f};
            case UIAlignment::TopRight:     return {1.0f, 0.0f};

            case UIAlignment::CenterLeft:   return {0.0f, 0.5f};
            case UIAlignment::Center:       return {0.5f, 0.5f};
            case UIAlignment::CenterRight:  return {1.0f, 0.5f};

            case UIAlignment::BottomLeft:   return {0.0f, 1.0f};
            case UIAlignment::BottomCenter: return {0.5f, 1.0f};
            case UIAlignment::BottomRight:  return {1.0f, 1.0f};
        }

        return {0.0f, 0.0f};
    }

    inline UIAlignment alignmentFromIndex(int index) {
        return static_cast<fluxgl::UIAlignment>(index % 9);
    }

    struct UITransform {
        static constexpr const char* TypeName = "fluxgl::UITransform";
        static void Create(fluxgl::Registry& registry, fluxgl::EntityID entityID, const Json::Value& data) {
            UITransform transform;
            
            // Parse the json data if it is a valid object, and go through its members to compute the transform component
            if(data.isObject()) {
                Json::Value offsetData = data.get("offset", Json::objectValue);
                transform.offset = {offsetData.get("x", 0.0f).asFloat(), offsetData.get("y", 0.0f).asFloat()};

                Json::Value scaleData = data.get("scale", Json::objectValue);
                transform.scale = {scaleData.get("x", 1.0f).asFloat(), scaleData.get("y", 1.0f).asFloat()};

                transform.layer = data.get("layer", 0).asInt();

                Json::Value anchorData = data.get("anchor", "TopLeft");
                if(anchorData.asString() == "TopLeft") transform.anchor = UIAlignment::TopLeft;
                if(anchorData.asString() == "TopCenter") transform.anchor = UIAlignment::TopCenter;
                if(anchorData.asString() == "TopRight") transform.anchor = UIAlignment::TopRight;
                if(anchorData.asString() == "CenterLeft") transform.anchor = UIAlignment::CenterLeft;
                if(anchorData.asString() == "Center") transform.anchor = UIAlignment::Center;
                if(anchorData.asString() == "CenterRight") transform.anchor = UIAlignment::CenterRight;
                if(anchorData.asString() == "BottomLeft") transform.anchor = UIAlignment::BottomLeft;
                if(anchorData.asString() == "BottomCenter") transform.anchor = UIAlignment::BottomCenter;
                if(anchorData.asString() == "BottomRight") transform.anchor = UIAlignment::BottomRight;

                Json::Value pivotData = data.get("pivot", "TopLeft");
                if(pivotData.asString() == "TopLeft") transform.pivot = UIAlignment::TopLeft;
                if(pivotData.asString() == "TopCenter") transform.pivot = UIAlignment::TopCenter;
                if(pivotData.asString() == "TopRight") transform.pivot = UIAlignment::TopRight;
                if(pivotData.asString() == "CenterLeft") transform.pivot = UIAlignment::CenterLeft;
                if(pivotData.asString() == "Center") transform.pivot = UIAlignment::Center;
                if(pivotData.asString() == "CenterRight") transform.pivot = UIAlignment::CenterRight;
                if(pivotData.asString() == "BottomLeft") transform.pivot = UIAlignment::BottomLeft;
                if(pivotData.asString() == "BottomCenter") transform.pivot = UIAlignment::BottomCenter;
                if(pivotData.asString() == "BottomRight") transform.pivot = UIAlignment::BottomRight;
            }

            registry.addComponent<UITransform>(entityID, transform);
        }

        glm::vec2 offset = {0, 0};
        glm::vec2 scale = {1, 1};
        int layer = 0;

        UIAlignment anchor = UIAlignment::TopLeft; // Anchor point in the ui window
        UIAlignment pivot = UIAlignment::TopLeft; // Pivot anchor point of the ui element (offset is an offset to this)

        inline glm::vec3 getComputedPosition(const glm::vec2& elementSize, const glm::vec2& screenSize) const {
            glm::vec2 anchorPosition = toAlignment(anchor) * screenSize;

            return glm::vec3(anchorPosition + offset - getPivotOffset(elementSize), layer);
        }

        inline glm::vec2 getPivotOffset(const glm::vec2& elementSize) const {
            return toAlignment(pivot) * elementSize;
        }
    };
}