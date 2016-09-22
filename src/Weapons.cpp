#include <iostream>
#include "Weapons.h"
#include "Constants.h"
#include <math.h>
#include "Entity.h"

using namespace std;

Weapons::Weapons(sf::Texture* texture, int weaponType){
    if(weaponType == PISTOLS){
        spriteOriginX = 90;
        spriteOriginY = 10;
        spriteWidth = 8;
        spriteHeight = 24;
        sprite.setOrigin(-15, spriteHeight/2);
        firerate = 1.0/300.0;
        bulletSpawnDistance = 11;
        equipped = true;
        damage = 10;
    }
    else if(weaponType == SAWEDOFF){
        spriteOriginX = 100;
        spriteOriginY = 10;
        spriteWidth = 20;
        spriteHeight = 6;
        sprite.setOrigin(-15, 3);
        firerate = 1.0/1000.0;
        equipped = false;
        damage = 20;
    }
    else if(weaponType == BOW){
        spriteOriginX = 130;
        spriteOriginY = 10;
        spriteWidth = 6;
        spriteHeight = 22;
        sprite.setOrigin(-15, spriteHeight/2);
        firerate = 1.0/1000.0;
        bulletSpawnDistance = 0;
        equipped = false;
        damage = 50;
    }
    else if(weaponType == MINIGUN){
        spriteOriginX = 140;
        spriteOriginY = 10;
        spriteWidth = 38;
        spriteHeight = 20;
        sprite.setOrigin(-15, spriteHeight/2);
        firerate = 1.0/1.0;
        bulletSpawnDistance = 2.0;
        equipped = false;
        damage = 5;
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

bool Weapons::getEquipped(){
    return equipped;
}
bool Weapons::setEquipped(bool equipWeapon){
    equipped = equipWeapon;
}
