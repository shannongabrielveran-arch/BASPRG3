
#include "player.h"
#include "GameScene.h"
#include "Scene.h"

Player::~Player()
{
    // Scene handles deleting bullets.
}

void Player::start()
{
    texture = loadTexture("gfx/player.png");

    sound = SoundManager::loadSound(
        "sound/196914__dpoggioli__laser-gun.ogg"
    );

    x = 100;
    y = 100;

    width = 0;
    height = 0;

    defaultSpeed = 5;
    boostedSpeed = 10;

    currentSpeed = defaultSpeed;

    reloadTime = 4;
    currentReloadTime = 0;

    wingReloadTime = 4;
    currentWingReloadTime = 0;

    SDL_QueryTexture(
        texture,
        NULL,
        NULL,
        &width,
        &height
    );
}

void Player::update()
{
    // Update power-up timers.
    if (miniShipFrames > 0)
        --miniShipFrames;

    if (shotgunFrames > 0)
        --shotgunFrames;

    if (invulnerableFrames > 0)
        --invulnerableFrames;

    if (miniShipReload > 0)
        --miniShipReload;

    // MINI SHIP POWER-UP
    // The companion follows the player and fires automatically.
    if (miniShipFrames > 0 && miniShipReload <= 0)
    {
        getScene()->addGameObject(
            new Bullet(
                x + width + 8,
                y - 12,
                1,
                0,
                9
            )
        );

        miniShipReload = 18;
    }

    if (currentReloadTime > 0)
    {
        currentReloadTime--;
    }

    if (currentWingReloadTime > 0)
    {
        currentWingReloadTime--;
    }

    // MAIN CANNON
    if (
        app.keyboard[SDL_SCANCODE_F] &&
        currentReloadTime <= 0
        )
    {
        SoundManager::playSound(sound);

        Bullet* bullet = new Bullet(
            x + width - 5,
            y + height / 2 - 5,
            1,
            0,
            5
        );

        getScene()->addGameObject(bullet);

        // SHOTGUN POWER-UP
        // Five projectiles with different spread directions.
        if (shotgunFrames > 0)
        {
            for (int spread = -2; spread <= 2; ++spread)
            {
                if (spread != 0)
                {
                    getScene()->addGameObject(
                        new Bullet(
                            x + width - 5,
                            y + height / 2 - 5,
                            5,
                            spread,
                            2
                        )
                    );
                }
            }
        }

        currentReloadTime = reloadTime;
    }

    // WING CANNONS
    if (
        app.keyboard[SDL_SCANCODE_G] &&
        currentWingReloadTime <= 0
        )
    {
        SoundManager::playSound(sound);

        Bullet* upperBullet = new Bullet(
            x,
            y,
            1,
            0,
            5
        );

        Bullet* lowerBullet = new Bullet(
            x,
            y + height - 5,
            1,
            0,
            5
        );

        getScene()->addGameObject(upperBullet);
        getScene()->addGameObject(lowerBullet);

        // SHOTGUN POWER-UP FOR WING CANNONS
        if (shotgunFrames > 0)
        {
            for (int spread = -2; spread <= 2; ++spread)
            {
                if (spread != 0)
                {
                    getScene()->addGameObject(
                        new Bullet(
                            x,
                            y,
                            5,
                            spread,
                            2
                        )
                    );

                    getScene()->addGameObject(
                        new Bullet(
                            x,
                            y + height - 5,
                            5,
                            spread,
                            2
                        )
                    );
                }
            }
        }

        currentWingReloadTime = wingReloadTime;
    }

    // SPEED BOOST
    // Hold Left Shift to increase speed.
    if (
        app.keyboard[SDL_SCANCODE_LSHIFT]
        )
    {
        currentSpeed = boostedSpeed;
    }

    // Backspace returns speed to normal.
    if (
        app.keyboard[SDL_SCANCODE_BACKSPACE]
        )
    {
        currentSpeed = defaultSpeed;
    }

    // MOVEMENT
    if (
        app.keyboard[SDL_SCANCODE_W]
        )
    {
        y -= currentSpeed;
    }

    if (
        app.keyboard[SDL_SCANCODE_S]
        )
    {
        y += currentSpeed;
    }

    if (
        app.keyboard[SDL_SCANCODE_A]
        )
    {
        x -= currentSpeed;
    }

    if (
        app.keyboard[SDL_SCANCODE_D]
        )
    {
        x += currentSpeed;
    }

    // Keep player inside left side.
    if (x < 0)
    {
        x = 0;
    }

    // Keep player inside right side.
    if (x + width > SCREEN_WIDTH)
    {
        x = SCREEN_WIDTH - width;
    }

    // Keep player inside top.
    if (y < 0)
    {
        y = 0;
    }

    // Keep player inside bottom.
    if (y + height > SCREEN_HEIGHT)
    {
        y = SCREEN_HEIGHT - height;
    }
}

void Player::draw()
{
    // Draw player, with blinking effect after damage.
    if (
        invulnerableFrames == 0 ||
        (invulnerableFrames / 5) % 2 == 0
        )
    {
        blit(texture, x, y);
    }

    // SHIELD POWER-UP VISUAL
    // Blue protective ring surrounding the player.
    if (shieldCharges > 0)
    {
        SDL_SetRenderDrawBlendMode(
            app.renderer,
            SDL_BLENDMODE_BLEND
        );

        SDL_SetRenderDrawColor(
            app.renderer,
            30, 190, 255, 220
        );

        SDL_Rect outer = {
            x - 5,
            y - 5,
            width + 10,
            height + 10
        };

        SDL_RenderDrawRect(app.renderer, &outer);

        SDL_Rect inner = {
            x - 3,
            y - 3,
            width + 6,
            height + 6
        };

        SDL_RenderDrawRect(app.renderer, &inner);
    }

    // MINI SHIP POWER-UP VISUAL
    // Green triangular companion beside the player.
    if (miniShipFrames > 0)
    {
        SDL_SetRenderDrawColor(
            app.renderer,
            90, 255, 170, 255
        );

        SDL_Point shape[4] = {
            {x + width / 2 - 10, y - 12},
            {x + width / 2 + 10, y - 12},
            {x + width / 2, y - 26},
            {x + width / 2 - 10, y - 12}
        };

        SDL_RenderDrawLines(
            app.renderer,
            shape,
            4
        );
    }
}

int Player::getX()
{
    return x;
}

int Player::getY()
{
    return y;
}

// POWER-UP ACTIVATION
void Player::activatePowerUp(int type)
{
    // Shield: block one enemy attack.
    if (type == 0)
    {
        shieldCharges = 1;
    }

    // Mini ship: active for 30 seconds.
    else if (type == 1)
    {
        miniShipFrames = 30 * FPS;
        miniShipReload = 0;
    }

    // Shotgun: active for 15 seconds.
    else if (type == 2)
    {
        shotgunFrames = 15 * FPS;
    }
}

// SHIELD DAMAGE ABSORPTION
bool Player::consumeShield()
{
    if (shieldCharges <= 0)
    {
        return false;
    }

    --shieldCharges;
    return true;
}

// PLAYER DAMAGE HANDLING
bool Player::receiveHit()
{
    // Ignore damage during invulnerability frames.
    if (invulnerableFrames > 0)
    {
        return false;
    }

    // Shield absorbs one attack.
    if (consumeShield())
    {
        invulnerableFrames = 25;
        return true;
    }

    // Without shield, player loses health.
    --health;

    invulnerableFrames = 60;

    if (health <= 0)
    {
        destroy();
    }

    return true;
}
