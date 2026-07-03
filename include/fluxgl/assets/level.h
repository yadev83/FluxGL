#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <fluxgl/assets/assets.h>
#include <fluxgl/core/app.h>

namespace fluxgl {
    using TileRow = std::vector<std::string>;
    using TileID = std::string;

    struct TileDefinition {
        Resource prefab;
    };

    struct EntityDefinition {
        Resource prefab;

        float positionX;
        float positionY;
    };

    class Level {
        private:
            int m_tileWidth;
            int m_tileHeight;

            std::vector<TileRow> m_tileMap;
            std::unordered_map<TileID, TileDefinition> m_tileDefinitions;
            std::vector<EntityDefinition> m_entities;

        public:
            Level() = default;
            Level(const std::string& jsonString);

            inline const int getTileWidth() const { return m_tileWidth; }
            inline const int getTileHeight() const { return m_tileHeight; }
            inline const std::vector<TileRow>& getTileMap() const { return m_tileMap; }
            inline const std::unordered_map<TileID, TileDefinition>& getTileDefinitions() const { return m_tileDefinitions; }
            inline const std::vector<EntityDefinition>& getEntities() const { return m_entities; }

            void loadFromJSON(const std::string& jsonString);
            std::string toString();
    };
}