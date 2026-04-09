//
// Created by aamal on 09/04/2026.
//

#ifndef CHURCHILLMAGNUM_ENEMYFAB_H
#define CHURCHILLMAGNUM_ENEMYFAB_H
#include <Magnum/Math/Color.h>

#include "Entity.hpp"

enum EnemyType
{
    Runner,
    Heavy,
    All
};

struct EnemyFab // @TODO: Instantiate one of each in memory so that ImGui can be used to play with parameters
{
    float speed;
    float scale;
    float radius;
    int num_segments;
    Magnum::Color3 color = Magnum::Color3::red();

    EnemyFab() = delete;

    static EnemyFab Runner()
    {
        return EnemyFab(
            1.0f,
            0.05f,
            0.1f,
            10,
            Magnum::Color3::cyan()
        );
    }

    static EnemyFab Heavy()
    {
        return EnemyFab(
            .5f,
            0.3f,
            0.2f,
            4,
            Magnum::Color3::red()
        );
    }

private:
    EnemyFab(
        const float speed,
        const float scale,
        const float radius,
        const int num_segments,
        const Magnum::Color3& color
    ) :
        speed(speed),
        scale(scale),
        radius(radius),
        num_segments(num_segments),
        color(color) {}
};



#endif //CHURCHILLMAGNUM_ENEMYFAB_H