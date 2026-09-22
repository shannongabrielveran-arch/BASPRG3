#include "Scene.h"
#include "GameObject.h"

Scene* Scene::activeScene = nullptr;

Scene::Scene()
{
    hasStarted = false;
}

Scene::~Scene()
{
    while (!objects.empty())
    {
        GameObject* obj = objects.back();
        objects.pop_back();

        obj->setScene(nullptr);
        delete obj;
    }
}

void Scene::setActiveScene(Scene* scene)
{
    if (Scene::activeScene != nullptr)
    {
        delete Scene::activeScene;
        Scene::activeScene = nullptr;
    }

    Scene::activeScene = scene;
    scene->start();
}

Scene* Scene::getActiveScene()
{
    return Scene::activeScene;
}

void Scene::addGameObject(GameObject* obj)
{
    obj->setScene(this);
    objects.push_back(obj);

    if (hasStarted)
    {
        obj->start();
    }
}

void Scene::removeGameObject(GameObject* obj)
{
    auto itr = std::find(
        objects.begin(),
        objects.end(),
        obj
    );

    if (itr != objects.end())
    {
        objects.erase(itr);
    }
}

void Scene::start()
{
    for (size_t i = 0; i < objects.size(); i++)
    {
        objects[i]->start();
    }

    hasStarted = true;
}

void Scene::update()
{
    // Objects spawned now begin updating next frame.
    const auto frameObjects = objects;

    for (GameObject* obj : frameObjects)
    {
        if (!obj->isDestroyed())
        {
            obj->update();
        }
    }

    // Remove destroyed objects after all updates.
    for (size_t i = 0; i < objects.size();)
    {
        if (objects[i]->isDestroyed())
        {
            delete objects[i];
        }
        else
        {
            i++;
        }
    }
}

void Scene::draw()
{
    for (size_t i = 0; i < objects.size(); i++)
    {
        objects[i]->draw();
    }
}