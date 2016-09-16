#include "Bullet.h"
#include <math.h>
#include "Hero.h"
#include "Entity.h"
#include "Constants.h"
#include "Enemy.h"

using namespace std;

Bullet::Bullet(sf::Texture* texture, float spawnX, float spawnY, float angle){

    //Sprite initialization
    spriteOriginX = 5;
    spriteOriginY = 35;
    spriteWidth = 2;
    spriteHeight = 1;
    sprite.setOrigin(0, 0.5);

    //Position & Orientation initialization
    xPos = spawnX;
    yPos = spawnY;
    orientation = angle;

    //Speed initialization
    typeMult = 2;
    speedIncrease = false;
    speedDecrease = false;
    setSpeed();

    setSprite(texture);
}

void Bullet::moveBullet(){

    xPos += cos(orientation)*speed;
    yPos += sin(orientation)*speed;

    sprite.setPosition(xPos, yPos);
}




