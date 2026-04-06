//
// Created by aamalh on 3/27/26.
//

#ifndef CHURCHILLMAGNUM_SPRITE_H
#define CHURCHILLMAGNUM_SPRITE_H

#include <memory>
#include <Magnum/GL/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Primitives/Circle.h>

#include "GameComponents.hpp"
#include "Magnum/Shaders/FlatGL.h"
#include "Magnum/ShaderTools/Stage.h"
#include "Magnum/Vk/Mesh.h"

enum SpriteType {
    Circle = 0,
    Rectangle = 1
};



class RectangleSprite : public Sprite
{
    using Position = Magnum::GL::Attribute<0, Magnum::Vector2>;
    float m_width, m_height;


public:
    RectangleSprite(
        std::shared_ptr<Magnum::Shaders::FlatGL2D> shader,
        const float width,
        const float height,
        const Magnum::Color3& color
        ) : Sprite(shader, color), m_width(width), m_height(height)
    {
        static constexpr Magnum::Vector2 vertices[]
        {
            {0.5f, -0.5f}, {0.5f, 0.5f}, {-0.5f, -0.5f}, {-0.5f, 0.5f}
        };

        m_mesh = std::make_shared<Magnum::GL::Mesh>();
        m_mesh->setPrimitive(Magnum::GL::MeshPrimitive::TriangleStrip)
                .setCount(4)
                .addVertexBuffer(Magnum::GL::Buffer {vertices}, 0, Position {});

    }
};

class CircleSprite : public Sprite {
    float m_radius;

public:
    CircleSprite(
        const std::shared_ptr<Magnum::Shaders::FlatGL2D> &shader,
        const float radius,
        const Magnum::Color3& color,
        const int segments = 64
    ) : Sprite(shader, color), m_radius(radius) {
        m_mesh = std::make_shared<Magnum::GL::Mesh>(Magnum::MeshTools::compile(Magnum::Primitives::circle2DSolid(segments)));
    }
};


#endif //CHURCHILLMAGNUM_SPRITE_H
