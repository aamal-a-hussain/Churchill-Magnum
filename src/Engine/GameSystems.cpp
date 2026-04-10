//
// Created by aamal on 09/04/2026.
//

#include <random>
#include <Magnum/GL/Renderer.h>

#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Resource.h>
#include <MagnumPlugins/AnyImageImporter/AnyImageImporter.h>
#include <Magnum/Trade/ImageData.h>
#include "GameEngine.h"
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/ImageView.h>
#include <Magnum/Text/AbstractShaper.h>


float GameEngine::rand()
{
    static std::random_device rnd;
    static std::uniform_real_distribution distribution(0.0f, 1.0f);
    return distribution(rnd);
}

std::shared_ptr<Entity> GameEngine::player()
{
    if (const auto player = m_playerRef.lock(); player && player->IsAlive())
        return player;
    Corrade::Utility::Fatal{} << "Player does not exist!";
}


void GameEngine::spawnPlayer()
{
    if (m_entityManager.playerExists())
    {
        Corrade::Utility::Fatal{} << "Player already exists!";
        return;
    }
    const std::shared_ptr<Entity> player = m_entityManager.addEntity(EntityType::Player);
    player->AddComponent<Transform>(
        Magnum::Vector2{0.0f, 0.0f},
        Magnum::Vector2{0.1f, 0.1f},
        Magnum::Vector2{0.0f, 0.0f},
        0.0f
    );


    Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter> manager;
    Corrade::Containers::Pointer<Magnum::Trade::AbstractImporter> importer =
        manager.loadAndInstantiate("AnyImageImporter");

    const Corrade::Utility::Resource rs{"player-data"};
    if (!importer || !importer->openData(rs.getRaw("player_direction.png")))
        Corrade::Utility::Fatal {} << "Could not load player texture";

    Corrade::Containers::Optional<Magnum::Trade::ImageData2D> image = importer->image2D(0);
    CORRADE_INTERNAL_ASSERT(image);
    Magnum::GL::Texture2D texture;
    texture.setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge)
           .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear)
           .setMinificationFilter(Magnum::GL::SamplerFilter::Linear)
           .setStorage(1, Magnum::GL::textureFormat(image->format()), image->size())
           .setSubImage(0, {}, *image);

    // @TODO: Create a player shader so that we don't need to do two draw calls for it, and can do more interesting things
    player->AddSprite(m_texturedShader, Magnum::Color3::green(), std::move(texture), 8);
    player->AddComponent<Input>();

    m_playerRef = player;
}

void GameEngine::spawnEnemy(EnemyFab& enemyFab)
{
    const std::shared_ptr<Entity> enemy = m_entityManager.addEntity(EntityType::Enemy);

    Magnum::Vector2 velocity{rand() * 2 - 1, rand() * 2 - 1};
    velocity /= velocity.length() + 1E-8f;
    velocity *= enemyFab.speed;
    enemy->AddComponent<Transform>(
        Magnum::Vector2{rand() * 1.5f - 1, rand() * 1.5f - 1},
        Magnum::Vector2{enemyFab.scale, enemyFab.scale},
        velocity,
        0.0f
    );
    enemy->AddSprite(m_flatShader, enemyFab.color, enemyFab.num_segments);
}

void GameEngine::spawnSmall(const std::shared_ptr<Entity>& enemy)
{
    const int numSmall = enemy->GetSprite()->numSegments;
    const auto& color = enemy->GetSprite()->GetColor();
    const auto& t = enemy->Get<Transform>();
    for (int i = 0; i < numSmall; ++i)
    {
        const auto theta = 2.0f * (float)M_PI / numSmall * i;
        const std::shared_ptr<Entity> s = m_entityManager.addEntity(EntityType::Small);
        Magnum::Vector2 vel = {cos(theta), sin(theta)};
        vel /= vel.length() + 1E-8f;
        vel *= t.velocity.length();
        const auto scale = t.scale / numSmall;
        s->AddComponent<Transform>(
            t.position,
            scale,
            vel,
            theta
        );
        s->AddSprite(m_flatShader, color, numSmall);
        s->AddComponent<LifeSpan>(1.0f, m_timeline.currentFrameTime());
    }
}

void GameEngine::spawnBullet(const Magnum::Vector2& position, const Magnum::Vector2& direction)
{
    const std::shared_ptr<Entity> bullet = m_entityManager.addEntity(EntityType::Bullet);
    Magnum::Vector2 velocity = direction / (direction.length() + 1E-8f) * m_bulletFab.speed;
    bullet->AddComponent<Transform>(
        position,
        Magnum::Vector2{m_bulletFab.scale, m_bulletFab.scale},
        velocity,
        0.0f
    );
    bullet->AddComponent<LifeSpan>(m_bulletFab.lifetime, m_timeline.currentFrameTime());
    bullet->AddSprite(m_flatShader, m_bulletFab.color);
}

void GameEngine::sEnemySpawner()
{
    const float cTime = m_timeline.currentFrameTime();
    if (cTime - m_lastSpawnTime > m_spawnInterval)
    {
        m_lastSpawnTime = cTime;
        if (rand() < 0.2f)
        {
            spawnEnemy(m_heavyFab);
        }
        else
        {
            spawnEnemy(m_runnerFab);
        }
    }
}

void GameEngine::sCollision()
{
    static const auto squaredDistanceCheck = [](const Magnum::Vector2& v, const float minDist) {
        return v.x() * v.x() + v.y() * v.y() < minDist * minDist;
    };

    const auto& pTransform = player()->Get<Transform>();
    const auto& enemies = m_entityManager.GetEntityById(EntityType::Enemy);
    const auto& bullets = m_entityManager.GetEntityById(EntityType::Bullet);

    for (const auto& e : enemies)
    {
        const auto &eTransform = e->Get<Transform>();
        bool eCollided = false;
        for (const auto& b : bullets) {
            if (!e->IsAlive()) break;
            if (!b->IsAlive()) continue;
            const auto& bTransform = b->Get<Transform>();
            eCollided = squaredDistanceCheck(bTransform.position - eTransform.position, (eTransform.scale.x() + bTransform.scale.x()) / 2 + m_collisionCapsuleScale);
            if (eCollided)
            {
                b->Destroy();
                e->Destroy();
                spawnSmall(e);
                incrementScore();
            }
        }
        if (eCollided) continue;

        eCollided = squaredDistanceCheck(pTransform.position - eTransform.position, (eTransform.scale.x() + pTransform.scale.x()) / 2 + m_collisionCapsuleScale);
        if (eCollided)
        {
            e->Destroy();
            spawnSmall(e);
            decrementHealth();
        }
    }
}


void GameEngine::clearEntity(EntityType type)
{
    const auto& entities = m_entityManager.GetEntityById(type);
    for (auto& e : entities) e->Destroy();
}


void GameEngine::integratePlayerPosition(Transform& t)
{
    const float dt = m_timeline.currentFrameDuration();
    Magnum::Vector2 newPosition{t.position.x(), t.position.y()};

    newPosition.x() += t.velocity.x() * dt;
    newPosition.y() += t.velocity.y() * dt;

    if (newPosition.x() - t.scale.x() / 2 < -windowDimensions().x() / 2 || newPosition.x() + t.scale.x() / 2 >
        windowDimensions().x() / 2)
        newPosition.x() = t.position.x();
    if (newPosition.y() - t.scale.y() / 2 < -windowDimensions().y() / 2 || newPosition.y() + t.scale.y() / 2 >
        windowDimensions().y() / 2)
        newPosition.y() = t.position.y();

    t.position = newPosition;
}

void GameEngine::integratePosition(Transform& t)
{
    const float dt = m_timeline.currentFrameDuration();
    auto tmp_position = t.position + t.velocity * dt;
    if (tmp_position.x() - t.scale.x() / 2 < -windowDimensions().x() / 2 || tmp_position.x() + t.scale.x() / 2 >
        windowDimensions().x() / 2)
    {
        t.velocity = {-t.velocity.x(), t.velocity.y()};
    }
    if (tmp_position.y() - t.scale.y() / 2 < -windowDimensions().y() / 2 || tmp_position.y() + t.scale.y() / 2 >
        windowDimensions().y() / 2)
    {
        t.velocity = {t.velocity.x(), -t.velocity.y()};
    }
    t.position += t.velocity * dt;
}

void GameEngine::sMovement()
{
    const auto& entities = m_entityManager.GetEntities();
    for (const auto& e : entities)
    {
        if (e->GetEntityType() == EntityType::Player) continue;
        integratePosition(e->Get<Transform>());
    }

    auto& t = player()->Get<Transform>();
    auto& i = player()->Get<Input>();

    Magnum::Vector2 newVelocity{};

    if (i.up) newVelocity.y() += 1.0f;
    if (i.down) newVelocity.y() -= 1.0f;
    if (i.left) newVelocity.x() -= 1.0f;
    if (i.right) newVelocity.x() += 1.0f;

    newVelocity /= newVelocity.length() + 1E-8f;

    const auto newSpeed = i.warp ? m_playerControls.warpSpeed : m_playerControls.movementSpeed;
    newVelocity *= newSpeed;

    t.velocity = newVelocity;
    integratePlayerPosition(t);
}

void GameEngine::sMovementInput(const KeyEvent& event, int pressOrRelease)
{
    auto& i = player()->Get<Input>();
    if (event.key() == Key::W) i.up = pressOrRelease;
    if (event.key() == Key::S) i.down = pressOrRelease;
    if (event.key() == Key::A) i.left = pressOrRelease;
    if (event.key() == Key::D) i.right = pressOrRelease;
    if (event.key() == Key::Space) i.warp = pressOrRelease;
}

void GameEngine::sShoot()
{
    const auto& screenToViewport = [this](Magnum::Vector2i& pos)
    {
        float posx = (float)pos.x(),
        posy = (float)pos.y(),
        wSizex = (float)windowSize().x(),
        wSizey = (float) windowSize().y();

        const Magnum::Vector2 normalized = {posx / wSizex * 2.0f - 1.0f, (wSizey - posy) / wSizey * 2.0f - 1.0f};
        return normalized * windowDimensions() / 2.0f;
    };

    if (!player()->Get<Input>().shoot) return;
    if (const auto cTime = m_timeline.currentFrameTime(); cTime - m_lastShotTime > m_bulletFab.shotInterval)
    {
        const auto& pointerLoc = screenToViewport(m_pointerLoc);
        const auto& pos = player()->Get<Transform>().position;
        const auto& direction = pointerLoc - pos;

        m_lastShotTime = cTime;
        spawnBullet(pos, direction);
    }
}

void GameEngine::sShootInput(const int pressOrRelease)
{
    player()->Get<Input>().shoot = pressOrRelease;
}

void GameEngine::incrementScore()
{
    m_score_renderer.clear();
    m_score_renderer.render(*m_fontShaper, m_fontSize, std::format("Score {}", m_score++));
}

void GameEngine::decrementHealth()
{
    m_health_renderer.clear();
    m_health_renderer.render(*m_fontShaper, m_fontSize, std::format("Health {}", m_health--));
}

void GameEngine::sLifespan()
{
    const auto cTime = m_timeline.currentFrameTime();

    for (const auto& e : m_entityManager.GetEntities())
    {
        if (!e->Has<LifeSpan>()) continue;;
        const auto& l = e->Get<LifeSpan>();
        const auto remaining = l.lifespan + l.startTime - cTime;
        if (e->GetEntityType() == EntityType::Small)
        {
            e->SetAlpha(1.0f - expf(-remaining));
        } else if (e->GetEntityType() == EntityType::Bullet)
        {
            e->SetAlpha(remaining / l.lifespan);
        }

        if (remaining <= 0.0f) {
            e->Destroy();
        }
    }
}
