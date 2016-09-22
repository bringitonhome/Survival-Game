#include "Bullet.h"
#include <math.h>
#include "Hero.h"
#include "Entity.h"
#include "Constants.h"
#include "Enemy.h"

using namespace std;

Bullet::Bullet(sf::Texture* texture, float spawnX, float spawnY, float angle){

    //Sprite initialization
    spriteOriginX = 130;
    spriteOriginY = 10;
    spriteWidth = 4;
    spriteHeight = 2;
    sprite.setOrigin(spriteWidth/2, spriteHeight/2);

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




