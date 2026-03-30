//
// Created by aamalh on 3/27/26.
//

#ifndef CHURCHILLMAGNUM_SPRITECONFIGLOADER_H
#define CHURCHILLMAGNUM_SPRITECONFIGLOADER_H
#include <string>
#include <fstream>
#include <vector>

#include <Corrade/Containers/Pointer.h>

#include "Sprite.h"

struct SpriteConfig {
    SpriteType type;
    std::string name;
    bool aliveOnStart;
    float r, g, b;
    float pos_x, pos_y;
    float vel_x, vel_y;
    float radius, width, height;
};

class SpriteLoader {
    class SpriteIterator {
        using base_iterator = std::vector<SpriteConfig>::const_iterator;
        using SpriteT = Corrade::Containers::Pointer<Sprite>;

        base_iterator m_current;

    public:
        explicit SpriteIterator(const base_iterator current) : m_current(current) {}

        SpriteT operator*() const {
            auto cfg = *m_current;
            switch (cfg.type) {
                case (Circle):
                    return Corrade::Containers::Pointer{new CircleSprite(
                            {cfg.pos_x, cfg.pos_y},
                            cfg.radius,
                            cfg.name,
                            cfg.aliveOnStart,
                            {cfg.r, cfg.g, cfg.b},
                            {cfg.vel_x, cfg.vel_y}
                        )};
            case (Rectangle):
                return Corrade::Containers::Pointer{new RectangleSprite(
                        {cfg.pos_x, cfg.pos_y},
                        cfg.width, cfg.height,
                        cfg.name,
                        cfg.aliveOnStart,
                        {cfg.r, cfg.g, cfg.b},
                        {cfg.vel_x, cfg.vel_y}
                    )};
                default:
                    throw std::runtime_error("Unknown sprite type");
            }
        }

        SpriteIterator operator++() {
            ++m_current;
            return *this;
        }

        SpriteIterator operator++(int) {
            const auto ret = *this;
            ++m_current;
            return ret;
        }

        bool operator!=(const SpriteIterator &other) const {
            return m_current != other.m_current;
        }

        bool operator==(const SpriteIterator &other) const {
            return m_current == other.m_current;
        }
    };

    std::vector<SpriteConfig> m_sprites;

    public:
    SpriteLoader() = default;

    static SpriteLoader readFromFile(const std::string& config_path) {
        auto loader = SpriteLoader{};
        std::ifstream ifs(config_path);
        std::string line;

        int type;
        SpriteType spriteType;
        std::string name;
        bool aliveOnStart;
        float r, g, b, pos_x, pos_y, vel_x, vel_y;
        while (std::getline(ifs, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::stringstream ss(line);
            if (ss >> type)
            {
                spriteType = static_cast<SpriteType>(type);
                ss >> name >> aliveOnStart >> r >> g >> b >> pos_x >> pos_y >>  vel_x >> vel_y;
                if (spriteType == Circle) {
                    float radius;
                    ss >> radius;
                    loader.m_sprites.emplace_back(
                        SpriteConfig {spriteType, name, aliveOnStart, r, g, b, pos_x, pos_y, vel_x, vel_y, radius, 0, 0}
                    );
                } else if (spriteType == Rectangle)
                {
                    float width, height;
                    ss >> width >> height;
                    loader.m_sprites.emplace_back(
                        SpriteConfig {spriteType, name, aliveOnStart, r, g, b, pos_x, pos_y, vel_x, vel_y, 0, width, height}
                    );
                }
            }
        }
        return loader;
    }

    [[nodiscard]] size_t length() const {
        return m_sprites.size();
    }

    auto begin() { return SpriteIterator(m_sprites.begin()); }
    auto end() { return SpriteIterator(m_sprites.end()); }

};

#endif //CHURCHILLMAGNUM_SPRITECONFIGLOADER_H
