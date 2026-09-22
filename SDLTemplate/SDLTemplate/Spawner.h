#pragma once
#include "GameObject.h"

class EnemySpawner : public GameObject
{
public:
    void start() override;
    void update() override;

private:
    int spawnTime = 120;
    int currentSpawnTime = 0;
};