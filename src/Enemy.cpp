#include "Entity.h"
#include "Enemy.h"
#include "Constants.h"
#include <math.h>
#include "Hero.h"


Enemy::Enemy(sf::Texture* texture){
    //Sprite initialization
    spriteOriginX = 10 + 40*(rand()%2);
    spriteOriginY = 50;
    spriteWidth = 32;
    spriteHeight = 32;
    sprite.setOrigin(spriteWidth/2, spriteHeight/2);

    //Starting location
    xPos = rand()%WW;
    yPos = rand()%WH;
    orientation = (rand()%360)*M_PI/180;

    setSprite(texture);

    speedIncrease = false;
    speedDecrease = false;
    typeMult = 0.25;

    //Gameplay
    health = 100;
    alive = true;
}

void Enemy::wander(){

    bool newDirection = (rand()%1000 <= 10);
    if(newDirection == true){
        orientation = (rand()%360)*M_PI/180;
    }
}

void Enemy::chaseHero(float heroPosX, float heroPosY, float heroDistance){

    orientation = atan2(heroPosY - yPos, heroPosX - xPos);

    if(heroDistance > 75){
        grabbing = false;
    }
    else if(heroDistance <= 75){
        grabbing = true;
    }

}

void Enemy::moveZombie(float heroPosX, float heroPosY){

    float heroDistance = sqrt(pow(heroPosY - yPos, 2) + pow(heroPosX - xPos, 2));

    if(heroDistance <= DETECTRANGE*SCALE){
        chaseHero(heroPosX, heroPosY, heroDistance);
        speedIncrease = true;
        speedDecrease = false;
    }
    else if(heroDistance > DETECTRANGE*SCALE){
        wander();
        speedIncrease = false;
        speedDecrease = true;
    }

    setSpeed();

    xPos += cos(orientation)*(1 - 2*(heroDistance < spriteWidth*SCALE))*speed + (xPos < -6*SCALE)*(WW+12*SCALE) - (xPos > WW+6*SCALE)*(WW+12*SCALE);
    yPos += sin(orientation)*(1 - 2*(heroDistance < spriteWidth*SCALE))*speed + (yPos < -6*SCALE)*(WH+12*SCALE) - (yPos > WH+6*SCALE)*(WH+12*SCALE);

    sprite.setRotation(orientation*180/M_PI);
    sprite.setPosition(xPos, yPos);


}


