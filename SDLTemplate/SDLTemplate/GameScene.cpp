
#include "GameScene.h"
#include "Spawner.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Explosion.h"
#include "text.h"

// Colored, automatically collected power-up tokens.
class PowerUpPickup : public GameObject
{
public:
    PowerUpPickup(int px, int py, int kind) : x(px), y(py), type(kind) {}

    int x, y, type;
    int lifetime = 18 * FPS;

    void update() override
    {
        x -= 2;
        if (--lifetime <= 0 || x < -30)
            destroy();
    }

    void draw() override
    {
        SDL_Rect r = { x, y, 28, 28 };

        if (type == 0)
            SDL_SetRenderDrawColor(app.renderer, 25, 170, 255, 255);

        if (type == 1)
            SDL_SetRenderDrawColor(app.renderer, 30, 240, 115, 255);

        if (type == 2)
            SDL_SetRenderDrawColor(app.renderer, 255, 155, 35, 255);

        SDL_RenderFillRect(app.renderer, &r);

        SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(app.renderer, &r);

        drawText(
            x + 5,
            y,
            255, 255, 255,
            TEXT_LEFT,
            "%c",
            type == 0 ? 'S' : type == 1 ? 'M' : 'G'
        );
    }
};

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

    if (player && !player->isDestroyed())
    {
        drawText(
            12, 12,
            255, 255, 255,
            TEXT_LEFT,
            "HP %d",
            player->getHealth()
        );

        if (player->hasShield())
        {
            drawText(
                12, 46,
                60, 200, 255,
                TEXT_LEFT,
                "SHIELD READY"
            );
        }

        if (player->getMiniShipSeconds() > 0)
        {
            drawText(
                12, 80,
                90, 255, 130,
                TEXT_LEFT,
                "MINI SHIP %dS",
                player->getMiniShipSeconds()
            );
        }

        if (player->getShotgunSeconds() > 0)
        {
            drawText(
                12, 114,
                255, 175, 70,
                TEXT_LEFT,
                "SHOTGUN %dS",
                player->getShotgunSeconds()
            );
        }
    }
    else
    {
        drawText(
            SCREEN_WIDTH / 2,
            SCREEN_HEIGHT / 2,
            255, 90, 90,
            TEXT_CENTER,
            "GAME OVER"
        );
    }
}

void GameScene::update()
{
    Scene::update();

    // Spawn power-ups, cycling through all three types.
    if (--pickupSpawnFrames <= 0)
    {
        static int nextType = 0;

        addGameObject(
            new PowerUpPickup(
                SCREEN_WIDTH - 35,
                95 + rand() % (SCREEN_HEIGHT - 190),
                nextType
            )
        );

        nextType = (nextType + 1) % 3;
        pickupSpawnFrames = 8 * FPS;
    }

    // Player collision detection.
    if (player && !player->isDestroyed())
    {
        for (GameObject* object : objects)
        {
            if (!object || object->isDestroyed() || object == player)
                continue;

            PowerUpPickup* pickup =
                dynamic_cast<PowerUpPickup*>(object);

            Bullet* hostile =
                dynamic_cast<Bullet*>(object);

            Enemy* enemyShip =
                dynamic_cast<Enemy*>(object);

            int ox = 0;
            int oy = 0;
            int ow = 0;
            int oh = 0;

            if (pickup)
            {
                ox = pickup->x;
                oy = pickup->y;
                ow = oh = 28;
            }
            else if (hostile && hostile->IsEnemyBullet())
            {
                ox = hostile->GetX();
                oy = hostile->GetY();
                ow = hostile->GetWidth();
                oh = hostile->GetHeight();
            }
            else if (enemyShip)
            {
                ox = enemyShip->getX();
                oy = enemyShip->getY();
                ow = enemyShip->getWidth();
                oh = enemyShip->getHeight();
            }
            else
            {
                continue;
            }

            bool touches =
                player->getX() < ox + ow &&
                player->getX() + player->getWidth() > ox &&
                player->getY() < oy + oh &&
                player->getY() + player->getHeight() > oy;

            if (!touches)
                continue;

            if (pickup)
            {
                player->activatePowerUp(pickup->type);
                pickup->destroy();
            }
            else
            {
                player->receiveHit();
                object->destroy();

                if (player->isDestroyed())
                {
                    player = nullptr;
                    break;
                }
            }
        }
    }

    std::vector<Explosion*> explosionsToAdd;

    // Detect player bullets hitting enemies.
    for (GameObject* objectA : objects)
    {
        if (objectA == nullptr || objectA->isDestroyed())
            continue;

        Bullet* bullet = dynamic_cast<Bullet*>(objectA);

        if (bullet == nullptr)
            continue;

        if (bullet->IsEnemyBullet())
            continue;

        for (GameObject* objectB : objects)
        {
            if (objectB == nullptr || objectB->isDestroyed())
                continue;

            Enemy* enemy = dynamic_cast<Enemy*>(objectB);

            if (enemy == nullptr)
                continue;

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
