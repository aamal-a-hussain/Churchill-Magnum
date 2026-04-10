#ifndef GAMECOMPONENTS_GAMECOMPONENTS_HPP_
#define GAMECOMPONENTS_GAMECOMPONENTS_HPP_
#include <tuple>
#include <memory>

#include "Corrade/Containers/Optional.h"
#include "Magnum/Timeline.h"
#include "Magnum/GL/Mesh.h"
#include "Magnum/GL/Texture.h"
#include "Magnum/Math/Color.h"
#include "Magnum/Shaders/FlatGL.h"
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Primitives/Circle.h>


struct Component {
    bool       exists = false;
};

struct CollisionCapsule : Component {
    float      radius = 0.0f;
};

struct LifeSpan : Component {
    float     lifespan  = 0;
    float     startTime = 0;

    LifeSpan() : Component() {}
    explicit LifeSpan(const float lifespan, const float startTime) : Component(true), lifespan(lifespan), startTime(startTime) {}
};

struct Input : Component {
    int        up = 0;
    int        down = 0;
    int        right = 0;
    int        left = 0;
    int        shoot = 0;
    int        warp = 0;
};

struct Score : Component {
    size_t     score = 0;
};

class Sprite : public Component {
protected:
    Magnum::Color3 m_meshColor {};
    std::unique_ptr<Magnum::GL::Texture2D> m_meshTexture;
    std::unique_ptr<Magnum::GL::Mesh> m_mesh;
    std::shared_ptr<Magnum::Shaders::FlatGL2D> m_shader;

public:
    int numSegments {};
    Sprite() : Component() {};

    explicit Sprite(
        const std::shared_ptr<Magnum::Shaders::FlatGL2D> &shader,
        const Magnum::Color3& meshColor,
        const int segments = 64
    ) : Component(true), m_meshColor(meshColor), m_shader(shader), numSegments(segments)
    {
        m_mesh = std::make_unique<Magnum::GL::Mesh>(
            Magnum::MeshTools::compile(
                Magnum::Primitives::circle2DSolid(
                    segments,
                    Magnum::Primitives::Circle2DFlag::TextureCoordinates
                )
            )
        );
    }

    explicit Sprite(
        const std::shared_ptr<Magnum::Shaders::FlatGL2D> &shader,
        const Magnum::Color3& meshColor,
        Magnum::GL::Texture2D&& meshTexture,
        const int segments = 64
    ) : Component(true), m_meshColor(meshColor), m_shader(shader)
    {
        m_mesh = std::make_unique<Magnum::GL::Mesh>(
            Magnum::MeshTools::compile(
                Magnum::Primitives::circle2DSolid(
                    segments,
                    Magnum::Primitives::Circle2DFlag::TextureCoordinates
                )
            )
        );
        m_meshTexture = std::make_unique<Magnum::GL::Texture2D>(std::move(meshTexture));
    }

    [[nodiscard]] bool hasMesh() const { return static_cast<bool>(m_mesh); }
    virtual ~Sprite() = default;
    std::shared_ptr<Magnum::Shaders::FlatGL2D> GetShader() { return m_shader; }
    Magnum::GL::Mesh* GetMeshData() { return m_mesh.get(); }
    float *GetColorData() { return m_meshColor.data(); }
    [[nodiscard]] Magnum::Color3 GetColor() const { return m_meshColor; }
    [[nodiscard]] Magnum::GL::Texture2D* GetTexture() { return m_meshTexture.get(); }

};

struct Transform :  Component {
    Magnum::Vector2 position {};
    Magnum::Vector2 scale {};
    Magnum::Vector2 velocity {};
    float           rotation = 0.0f;

    Transform() : Component() {}

    Transform(
        const Magnum::Vector2& position,
        const Magnum::Vector2& scale,
        const Magnum::Vector2& velocity,
        const float rotation
        ) :
        Component(true),
        position(position),
        scale(scale),
        velocity(velocity),
        rotation(rotation)
    {}
};

typedef std::tuple<
    Transform,
    LifeSpan,
    CollisionCapsule,
    Input,
    Score
    > ComponentTuple;

#endif
