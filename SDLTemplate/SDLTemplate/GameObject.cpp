#include "GameObject.h"
#include "Scene.h"

GameObject::~GameObject()
{
	Scene::getActiveScene()->removeGameObject(this);
}

void GameObject::setScene(Scene* scene)
{
	parentScene = scene;
}

const Scene* GameObject::getScene()
{
	return parentScene;
}

void GameObject::start()
{
}

void GameObject::update()
{
}

void GameObject::draw()
{
}
