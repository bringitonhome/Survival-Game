#include <iostream>

#include "Constants.h"
#include "Entity.h"
#include <math.h>

using namespace std;



//Set Functions

void Entity::setSprite(sf::Texture* texture){
    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(spriteOriginX, spriteOriginY, spriteWidth, spriteHeight));
    sprite.setScale(SCALE, SCALE);
    sprite.setPosition(xPos, yPos);
    sprite.setRotation(orientation*180/M_PI);
}
void Entity::setSpeed(){
    speed = BASESPEED*typeMult*(1 + 0.5*speedIncrease)/(1 + 2*speedDecrease);
}

//Get Functions

int Entity::getSpriteWidth(){
    return spriteWidth;
}

int Entity::getSpriteHeight(){
    return spriteHeight;
}

sf::Sprite Entity::getSprite(){
    return sprite;
}
float Entity::getPositionX(){
    return xPos;
}
float Entity::getPositionY(){
    return yPos;
}
float Entity::getOrientation(){
    return orientation;
}
float Entity::getSpeed(){
    return speed;
}

void Entity::causeDamage(int damage){
    health -= damage;
    if(health <= 0){
        health = 0;
        alive  = false;
    }
}
int Entity::getHealth(){
    return health;
}

bool Entity::getAlive(){
    return alive;
}

int Entity::getDamage(){
    return damage;
}
