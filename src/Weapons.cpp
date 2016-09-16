#include <iostream>
#include "Weapons.h"
#include "Constants.h"
#include <math.h>
#include "Entity.h"

using namespace std;

Weapons::Weapons(sf::Texture* texture, string weaponType){
    if(weaponType == "pistols"){
        spriteOriginX = 20;
        spriteOriginY = 5;
        spriteWidth = 4;
        spriteHeight = 11;
        sprite.setOrigin(-3.5, 5.5);
        firerate = 1.0/300.0;
        bulletSpawnDistance = 4.0;

    }
    else if(weaponType == "sawed-off"){
        spriteOriginX = 25;
        spriteOriginY = 5;
        spriteWidth = 10;
        spriteHeight = 3;
        sprite.setOrigin(-5.5, 1.5);
        firerate = 1.0/1000.0;
    }
    else if(weaponType == "bow"){
        spriteOriginX = 40;
        spriteOriginY = 5;
        spriteWidth = 3;
        spriteHeight = 11;
        sprite.setOrigin(-5.5, 5.5);
        firerate = 1.0/1000.0;
        bulletSpawnDistance = 0;
    }
    else if(weaponType =="minigun"){
        spriteOriginX = 45;
        spriteOriginY = 5;
        spriteWidth = 19;
        spriteHeight = 10;
        sprite.setOrigin(-5.5, 4.5);
        firerate = 1.0/1.0;
        bulletSpawnDistance = 2.0;
    }

    setSprite(texture);
}

void Weapons::followCharacter(Hero* hero){
    xPos = (*hero).getPositionX() + cos(orientation);
    yPos = (*hero).getPositionY() + sin(orientation);
    orientation = (*hero).getOrientation();

    sprite.setRotation(orientation*180/M_PI);
    sprite.setPosition(xPos, yPos);
}

float Weapons::getFireRate(){
    return firerate;
}

float Weapons::getBulletSpawnDistance(){
    return bulletSpawnDistance;
}
