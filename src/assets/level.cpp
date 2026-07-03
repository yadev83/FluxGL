#include <fluxgl/assets/level.h>
#include <fluxgl/core/log.h>
#include <fluxgl/core/error.h>

#include <sstream>

namespace fluxgl {
    Level::Level(const std::string& jsonString) {
        loadFromJSON(jsonString);
    }

    void Level::loadFromJSON(const std::string& jsonString) {
        Json::CharReaderBuilder readerBuilder;
        Json::Value root;
        std::string errs;

        std::istringstream s(jsonString);
        if (!Json::parseFromStream(readerBuilder, s, &root, &errs)) {
            FLUXGL_LOG_ERROR("Failed to parse level JSON: " + errs);
            throw fluxgl::Error{fluxgl::ErrorCode::IOError, "Failed to parse level JSON: " + errs};
        }

        // Parse tile width and height
        m_tileMap.clear();
        m_tileDefinitions.clear();
        m_entities.clear();
        m_tileWidth = root.get("tileWidth", 1).asInt();
        m_tileHeight = root.get("tileHeight", 1).asInt();

        // Parse tile definitions
        if (!root.isMember("tiles")) {
            FLUXGL_LOG_WARNING("No tile definitions found in level JSON.");
            return;
        }

        const Json::Value& tileDefs = root["tiles"];
        for (const auto& tileDef : tileDefs) {
            std::string tileID = tileDef.get("id", "").asString();
            if (tileID.empty()) {
                FLUXGL_LOG_WARNING("Tile definition missing 'id' field.");
                continue;
            }

            TileDefinition definition;
            definition.prefab = tileDef.get("prefab", "").asString();
            m_tileDefinitions[tileID] = definition;
        }

        // Parse entities
        if(root.isMember("entities")) {
            const Json::Value& entitiesJson = root["entities"];
            for (const auto& entityDef : entitiesJson) {
                EntityDefinition entity;
                entity.prefab = entityDef.get("prefab", "").asString();

                float tileX = entityDef.get("tileX", 0).asFloat();
                float tileY = entityDef.get("tileY", 0).asFloat();
                float offsetX = entityDef.get("offsetX", 0.0f).asFloat();
                float offsetY = entityDef.get("offsetY", 0.0f).asFloat();

                entity.positionX = (tileX * m_tileWidth) + offsetX;
                entity.positionY = (tileY * m_tileHeight) + offsetY;

                m_entities.push_back(entity);
            }
        }

        // Parse tile map
        if(!root.isMember("tilemap")) {
            FLUXGL_LOG_WARNING("No tile map found in level JSON.");
            return;
        }
        const Json::Value& tileMapJson = root["tilemap"];
        for (const auto& row : tileMapJson) {
            TileRow tileRow;
            for (const auto& tileID : row) {
                tileRow.push_back(tileID.asString());
            }
            m_tileMap.push_back(tileRow);
        }
    }

    std::string Level::toString() {
        std::ostringstream oss;
        oss << "LevelData: " << std::endl;
        oss << "Tile Width: " << m_tileWidth << ", Tile Height: " << m_tileHeight << std::endl;
        oss << "Tile Definitions: " << std::endl;
        for (const auto& [tileID, definition] : m_tileDefinitions) {
            oss << "  Tile ID: " << tileID << ", Prefab: " << definition.prefab << std::endl;
        }
        oss << "Entities list: " << std::endl;
        for (const auto& entity : m_entities) {
            oss << " Entity with prefab " << entity.prefab << std::endl;
        }
        oss << "Tile Map: " << std::endl;
        for (const auto& row : m_tileMap) {
            for (const auto& tileID : row) {
                oss << tileID << " ";
            }
            oss << std::endl;
        }
        return oss.str();
    }
}