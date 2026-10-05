#include "GameScene.h"
#include "Spawner.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Explosion.h"

GameScene::GameScene()
{
    backgroundTexture = nullptr;

    player = new Player();
    addGameObject(player);

    addGameObject(new EnemySpawner());
}

GameScene::~GameScene()
{
}

void GameScene::start()
{
    backgroundTexture = loadTexture("gfx/background.png");

    Scene::start();
}

void GameScene::draw()
{
    SDL_Rect backgroundDestination;

    backgroundDestination.x = 0;
    backgroundDestination.y = 0;
    backgroundDestination.w = SCREEN_WIDTH;
    backgroundDestination.h = SCREEN_HEIGHT;

    SDL_RenderCopy(
        app.renderer,
        backgroundTexture,
        NULL,
        &backgroundDestination
    );

    Scene::draw();
}

void GameScene::update()
{
    Scene::update();

    std::vector<Explosion*> explosionsToAdd;

    for (GameObject* objectA : objects)
    {
        if (objectA == nullptr || objectA->isDestroyed())
        {
            continue;
        }

        Bullet* bullet = dynamic_cast<Bullet*>(objectA);

        if (bullet == nullptr)
        {
            continue;
        }

        if (bullet->IsEnemyBullet())
        {
            continue;
        }

        for (GameObject* objectB : objects)
        {
            if (objectB == nullptr || objectB->isDestroyed())
            {
                continue;
            }

            Enemy* enemy = dynamic_cast<Enemy*>(objectB);

            if (enemy == nullptr)
            {
                continue;
            }

            bool collision =
                bullet->GetX() < enemy->getX() + enemy->getWidth() &&
                bullet->GetX() + bullet->GetWidth() > enemy->getX() &&
                bullet->GetY() < enemy->getY() + enemy->getHeight() &&
                bullet->GetY() + bullet->GetHeight() > enemy->getY();

            if (collision)
            {
                int explosionX =
                    enemy->getX() + enemy->getWidth() / 2;

                int explosionY =
                    enemy->getY() + enemy->getHeight() / 2;

                explosionsToAdd.push_back(
                    new Explosion(
                        explosionX,
                        explosionY
                    )
                );

                enemy->destroy();
                bullet->destroy();

                break;
            }
        }
    }

    for (Explosion* explosion : explosionsToAdd)
    {
        addGameObject(explosion);
    }
}