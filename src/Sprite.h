//
// Created by aamalh on 3/27/26.
//

#ifndef CHURCHILLMAGNUM_SPRITE_H
#define CHURCHILLMAGNUM_SPRITE_H
#include <string>
#include <utility>
#include <Magnum/GL/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Math/Color.h>
#include <Corrade/Containers/Optional.h>
#include <Magnum/Primitives/Circle.h>

enum SpriteType {
    Circle = 0,
    Rectangle = 1
};

class Sprite {
protected:
    Magnum::Color3 m_meshColor;
    Magnum::Vector2 m_position;
    Magnum::Vector2 m_velocity;
    Corrade::Containers::Optional<Magnum::GL::Mesh> m_mesh;
    std::string m_name;
    bool m_alive;

public:
    Sprite(
        std::string  name,
        const bool aliveOnStart,
        const Magnum::Color3& meshColor,
        const Magnum::Vector2& position,
        const Magnum::Vector2& velocity = {1.0f, 0.5f}
    ) : m_meshColor(meshColor), m_position(position), m_velocity(velocity), m_name(std::move(name)), m_alive(aliveOnStart) {}
    [[nodiscard]] std::string getName() const {return m_name;}
    [[nodiscard]] bool isAlive() const { return m_alive; }
    [[nodiscard]] bool hasMesh() const { return (bool)m_mesh; }

    void setAlive(const bool alive) { m_alive = alive; }

    virtual ~Sprite() = default;

    Magnum::GL::Mesh& getMesh() { return *m_mesh; }
    [[nodiscard]] Magnum::Vector2 getTranslation() const { return m_position; }
    float *colorData() { return m_meshColor.data(); }
    [[nodiscard]] Magnum::Color3 getColor() const { return m_meshColor; }

    virtual void step(float dt);
    [[nodiscard]] virtual Magnum::Vector2 getScale() const = 0;
    virtual void setScale(float scale) = 0;
};

class RectangleSprite : public Sprite
{
    using Position = Magnum::GL::Attribute<0, Magnum::Vector2>;
    float m_width, m_height;


public:
    RectangleSprite(
        const Magnum::Vector2& position,
        const float width,
        const float height,
        const std::string& name,
        const bool aliveOnStart,
        const Magnum::Color3& color,
        const Magnum::Vector2& velocity = {1.0f, 0.5f}
        ) : Sprite(name, aliveOnStart, color, position, velocity), m_width(width), m_height(height)
    {
        constexpr Magnum::Vector2 vertices[]
        {
            {0.5f, -0.5f}, {0.5f, 0.5f}, {-0.5f, -0.5f}, {-0.5f, 0.5f}
        };

        m_mesh = Magnum::GL::Mesh {};
        m_mesh->setPrimitive(Magnum::GL::MeshPrimitive::TriangleStrip)
                .setCount(4)
                .addVertexBuffer(Magnum::GL::Buffer {vertices}, 0, Position {});

    }

    [[nodiscard]] Magnum::Vector2 getScale() const override { return {m_width, m_height}; }
    void setScale(const float scale) override
    {
        m_width = (int)(m_width * scale);
        m_height = (int)(m_height * scale);
    }
    void step(float dt) override;
};

class CircleSprite : public Sprite {
    float m_radius;

public:
    CircleSprite(
        const Magnum::Vector2& position,
        const float radius,
        const std::string& name,
        const bool aliveOnStart,
        const Magnum::Color3& color,
        const Magnum::Vector2& velocity = {1.0f, 0.5f},
        const int segments = 64
    ) : Sprite(name, aliveOnStart, color, position, velocity), m_radius(radius) {
        m_mesh = Magnum::MeshTools::compile(Magnum::Primitives::circle2DSolid(segments));
    }

    [[nodiscard]] Magnum::Vector2 getScale() const override { return {m_radius, m_radius}; }
    void step(float dt) override;
    void setScale(const float scale) override { m_radius = (int)(m_radius * scale); }

};


#endif //CHURCHILLMAGNUM_SPRITE_H
