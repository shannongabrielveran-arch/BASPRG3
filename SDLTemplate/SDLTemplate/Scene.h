#pragma once
#include <vector>
#include "app.h"
#include <algorithm>

extern App app;
class GameObject;

class Scene
{
public:
	~Scene();

	static void setActiveScene(Scene* scene);
	static Scene* getActiveScene();

	void addGameObject(GameObject* obj);
	void removeGameObject(GameObject* obj);

	virtual void start();
	virtual void update();
	virtual void draw();

	
private:
	static Scene* activeScene;
	std::vector <GameObject*> objects;
};

