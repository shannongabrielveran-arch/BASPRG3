#include "GameObject.h"
#include "Scene.h"

GameObject::~GameObject()
{
	Scene::getActiveScene()->removeGameObject(this);
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
