#include "Constants.h"
#include "Entity.h"
#include "Hero.h"
#include <math.h>
#include "Command.h"

//Constructor
Hero::Hero(sf::Texture* texture){
    //Sprite Info
    spriteOriginX = 5;
    spriteOriginY = 5;
    spriteWidth = 11;
    spriteHeight = 11;
    sprite.setOrigin(5.5, 5.5);

    //Position & Orientation initialization
    xPos = WW/4;
    yPos = WH/2;
    orientation = 0;

    //Speed initialization
    typeMult = 1;
    speedIncrease = false;
    speedDecrease = false;

    grabbed, speedIncrease, speedDecrease = false;
    setSprite(texture);
}

//Set Function
void Hero::setGrabbed(bool trueOrFalse){
    grabbed, speedDecrease = trueOrFalse;
}

void Hero::moveChar(sf::Vector2i mouseXY, bool commands[]){

    float xDirection = (commands[DKEY] - commands[AKEY]);
    float yDirection = (commands[SKEY] - commands[WKEY]);

    float moveAngle = atan2(yDirection, xDirection);
    speedIncrease = commands[LSHIFTKEY];

    setSpeed();

    if(xDirection != 0 || yDirection != 0){
        xPos += speed*cos(moveAngle) + (xPos < -11*SCALE)*(WW+22*SCALE) - (xPos > WW + 11*SCALE)*(WW+22*SCALE);
        yPos += speed*sin(moveAngle) + (yPos < -11*SCALE)*(WH+22*SCALE) - (yPos > WH + 11*SCALE)*(WH+22*SCALE);
    }

    orientation = atan2(mouseXY.y - yPos, mouseXY.x - xPos);

    sprite.setRotation(orientation*180/M_PI);
    sprite.setPosition(xPos, yPos);

}

