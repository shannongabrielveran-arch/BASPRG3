#pragma once
class Scene;
class GameObject
{
public:
	~GameObject();
	virtual void start();
	virtual void update();
	virtual void draw();
};

