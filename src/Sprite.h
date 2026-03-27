//
// Created by aamalh on 3/27/26.
//

#ifndef CHURCHILLMAGNUM_SPRITE_H
#define CHURCHILLMAGNUM_SPRITE_H
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

public:
    Sprite(
        const Magnum::Color3& meshColor,
        const Magnum::Vector2& position,
        const Magnum::Vector2& velocity = {1.0f, 0.5f}
    ) : m_meshColor(meshColor), m_position(position), m_velocity(velocity) {}
    bool hasMesh() const { return (bool)m_mesh; }
    virtual ~Sprite() {}

    Magnum::GL::Mesh& getMesh() { return *m_mesh; }
    Magnum::Vector2 getTranslation() const { return m_position; }
    float *colorData() { return m_meshColor.data(); }
    Magnum::Color3 getColor() const { return m_meshColor; }

    virtual void step(float dt);
    virtual Magnum::Vector2 getScale() const;
};

class CircleSprite : public Sprite {
    float m_radius;

public:
    CircleSprite(
        const Magnum::Vector2& position,
        const float radius,
        const Magnum::Color3& color,
        const Magnum::Vector2& velocity = {1.0f, 0.5f},
        const int segments = 64
    ) : Sprite(color, position, velocity), m_radius(radius) {
        m_mesh = Magnum::MeshTools::compile(Magnum::Primitives::circle2DSolid(segments));
    }
    Magnum::Vector2 getScale() const override { return {m_radius, m_radius}; }

    void step(float dt);
};


#endif //CHURCHILLMAGNUM_SPRITE_H
