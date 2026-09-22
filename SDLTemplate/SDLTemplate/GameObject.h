#pragma once

class Scene;

class GameObject
{
public:
    virtual ~GameObject();

    void setScene(Scene* scene);
    Scene* getScene();

    virtual void start();
    virtual void update();
    virtual void draw();

    void destroy()
    {
        destroyed = true;
    }

    bool isDestroyed() const
    {
        return destroyed;
    }

private:
    Scene* parentScene = nullptr;
    bool destroyed = false;
};