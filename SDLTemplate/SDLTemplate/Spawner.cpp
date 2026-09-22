#include "Spawner.h"
#include "Enemy.h"
#include "Scene.h"

void EnemySpawner::start()
{
    currentSpawnTime = 0;
}

void EnemySpawner::update()
{
    if (currentSpawnTime > 0)
    {
        currentSpawnTime--;
        return;
    }

    // Spawn at the right edge with a random height.
    Enemy* enemy = new Enemy(
        SCREEN_WIDTH,
        rand() % SCREEN_HEIGHT
    );

    getScene()->addGameObject(enemy);

    currentSpawnTime = spawnTime - 1;
}