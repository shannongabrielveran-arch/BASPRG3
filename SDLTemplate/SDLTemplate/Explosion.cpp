#include "Explosion.h"

Explosion::Explosion(
    int positionX,
    int positionY
)
{
    x = positionX;
    y = positionY;

    width = 0;
    height = 0;

    lifeTime = 20;
    currentLifeTime = lifeTime;

    texture = nullptr;
    sound = nullptr;
}

void Explosion::start()
{
    SDL_Surface* surface =
        IMG_Load("gfx/explosion.png");

    if (surface != nullptr)
    {
        // Make pure black transparent.
        Uint32 blackColor =
            SDL_MapRGB(
                surface->format,
                0,
                0,
                0
            );

        SDL_SetColorKey(
            surface,
            SDL_TRUE,
            blackColor
        );

        texture =
            SDL_CreateTextureFromSurface(
                app.renderer,
                surface
            );

        width = surface->w;
        height = surface->h;

        SDL_FreeSurface(surface);
    }

    // Center explosion on destroyed enemy.
    x -= width / 2;
    y -= height / 2;

    sound = SoundManager::loadSound(
        "sound/245372__quaker540__hq-explosion.ogg"
    );

    SoundManager::playSound(sound);
}

void Explosion::update()
{
    currentLifeTime--;

    if (currentLifeTime <= 0)
    {
        destroy();
    }
}

void Explosion::draw()
{
    if (texture != nullptr)
    {
        blit(texture, x, y);
    }
}