#include "player.h"
#include "draw.h"
#include "input.h"

void Player::start()
{
    texture = loadTexture("C:/Users/user/Downloads/Assets/player.png");

    x = 100;
    y = 100;
    width = 0;
    height = 0;
    movementSpeed = 1;

    if (texture != nullptr)
    {
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    }
}

void Player::update()
{
    // Set speed. It stays selected after releasing the key.
    if (app.keyboard[SDL_SCANCODE_LSHIFT])
    {
        movementSpeed = 5;
    }

    if (app.keyboard[SDL_SCANCODE_BACKSPACE])
    {
        movementSpeed = 1;
    }

    // Move using WASD or arrow keys.
    if (app.keyboard[SDL_SCANCODE_W] ||
        app.keyboard[SDL_SCANCODE_UP])
    {
        y -= movementSpeed;
    }

    if (app.keyboard[SDL_SCANCODE_S] ||
        app.keyboard[SDL_SCANCODE_DOWN])
    {
        y += movementSpeed;
    }

    if (app.keyboard[SDL_SCANCODE_A] ||
        app.keyboard[SDL_SCANCODE_LEFT])
    {
        x -= movementSpeed;
    }

    if (app.keyboard[SDL_SCANCODE_D] ||
        app.keyboard[SDL_SCANCODE_RIGHT])
    {
        x += movementSpeed;
    }
}

void Player::draw()
{
    if (texture != nullptr)
    {
        blit(texture, x, y);
    }
}