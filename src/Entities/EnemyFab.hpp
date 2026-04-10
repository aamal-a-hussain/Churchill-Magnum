//
// Created by aamal on 09/04/2026.
//

#ifndef CHURCHILLMAGNUM_ENEMYFAB_H
#define CHURCHILLMAGNUM_ENEMYFAB_H
#include <Magnum/Math/Color.h>

struct BulletFab
{
    float scale = 0.025f;
    float speed = 5.0f;
    float lifetime = 1.0f;
    float shotInterval = .5f;
    Magnum::Color3 color = Magnum::Color3::red();
};

struct EnemyFab
{
    float speed;
    float scale;
    int num_segments;
    Magnum::Color3 color = Magnum::Color3::red();

    EnemyFab() = delete;

    static EnemyFab Runner()
    {
        return EnemyFab(
            1.0f,
            0.075f,
            10,
            Magnum::Color3::cyan()
        );
    }

    static EnemyFab Heavy()
    {
        return EnemyFab(
            .5f,
            0.3f,
            4,
            Magnum::Color3::red()
        );
    }

private:
    EnemyFab(
        const float speed,
        const float scale,
        const int num_segments,
        const Magnum::Color3& color
    ) :
        speed(speed),
        scale(scale),
        num_segments(num_segments) {this->color = color;}
};



#endif //CHURCHILLMAGNUM_ENEMYFAB_H