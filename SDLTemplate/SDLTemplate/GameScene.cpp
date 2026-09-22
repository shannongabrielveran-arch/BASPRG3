#include "GameScene.h"
#include "Spawner.h"

GameScene::GameScene()
{
    player = new Player();
    addGameObject(player);

    addGameObject(new EnemySpawner());
}

GameScene::~GameScene()
{
    // Scene handles deleting its objects.
}

void GameScene::start()
{
    Scene::start();
}

void GameScene::draw()
{
    Scene::draw();
}

void GameScene::update()
{
    Scene::update();
}