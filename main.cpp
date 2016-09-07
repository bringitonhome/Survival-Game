#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <math.h>
#include <ctime>
#include <list>

#define WW 800
#define WH 600
#define SCALE 5
#define BASESPEED 0.1

#define DETECTRANGE 200
#define FIRERATE 200

#define WANDERRATE 5000

using namespace std;

// Base class
class Character{
public:

    Character();

    void setSprite(sf::Texture* texture){
        sprite.setTexture(*texture);
        sprite.setTextureRect(sf::IntRect(spriteX, spriteY, spriteWidth, spriteHeight));
        sprite.setScale(SCALE, SCALE);
        sprite.setPosition(sf::Vector2f(xPos, yPos));
    }

    sf::Sprite getSprite(){
        return sprite;
    }
    float getPositionX(){
        return xPos;
    }
    float getPositionY(){
        return yPos;
    }
    float getLookAngle(){
        return lookAngle;
    }

protected:
    sf::Sprite sprite;
    int spriteX;
    int spriteY;
    int spriteWidth;
    int spriteHeight;

    float xPos;
    float yPos;
    float lookAngle;

    float speed;
};

Character::Character(){
    spriteX = 4;
    spriteHeight = 11;
    sprite.setOrigin(5.5, 5.5);
}

class Hero : public Character{
public:
    Hero();

    void userInput(){
        wPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        aPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        sPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        dPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

        moving = wPressed + aPressed + sPressed + dPressed;

        shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    }

    void setGrabbed(bool trueOrFalse){
        grabbed = trueOrFalse;
    }

    void setSpeed(){
        speed = BASESPEED*(1 + 0.5*shiftPressed)/(1 + 2*grabbed);
    }

    void moveChar(sf::RenderWindow* window){

        userInput();

        float xDirection = (dPressed - aPressed);
        float yDirection = (sPressed - wPressed);

        float moveAngle = atan2(yDirection, xDirection);

        setSpeed();

        if(moving == true){
            xPos += speed*cos(moveAngle);
            yPos += speed*sin(moveAngle);
        }

        lookAngle = atan2(sf::Mouse::getPosition(*window).y - yPos, sf::Mouse::getPosition(*window).x - xPos);

        sprite.setRotation(lookAngle*180/M_PI);
        sprite.setPosition(xPos, yPos);
    }



private:

    bool wPressed;
    bool aPressed;
    bool sPressed;
    bool dPressed;
    bool shiftPressed;

    bool moving;
    bool grabbed;

    float speed;
};

Hero::Hero(){
    spriteY = 4;
    spriteWidth = 13;
    xPos = WW/4;
    yPos = WH/2;
    speed = BASESPEED;

    grabbed = false;
}

class Enemy : public Character{
public:
    Enemy();

    void setSpeed(){
        speed = (BASESPEED/2)*(1+0.5*chasing)/((1 + wounded)*(1 + 4*wandering)*(1 + grabbing));
    }

    void wander(){

        chasing = false;
        wandering = true;

        bool newDirection = (rand()%100000 <= 10);
        if(newDirection == true){
            lookAngle = (rand()%360)*M_PI/180;
        }

        setSpeed();

        xPos += cos(lookAngle)*speed;
        yPos += sin(lookAngle)*speed;

        sprite.setRotation(lookAngle*180/M_PI);
        sprite.setPosition(xPos, yPos);
    }

    void chaseHero(float heroPosX, float heroPosY, float heroDistance){

        wandering = false;
        chasing = true;

        setSpeed();

        lookAngle = atan2(heroPosY - yPos, heroPosX - xPos);
        if(heroDistance > 50){
            xPos += cos(lookAngle)*speed;
            yPos += sin(lookAngle)*speed;
            sprite.setRotation(lookAngle*180/M_PI);
            sprite.setPosition(xPos, yPos);
        }
        else if(heroDistance <= 50){
            xPos -= cos(lookAngle)*speed;
            yPos -= sin(lookAngle)*speed;
            sprite.setPosition(xPos, yPos);
        }
        if(heroDistance > 75){
            grabbing = false;
        }
        else if(heroDistance <= 75){
            grabbing = true;
        }
    }

    void moveZombie(Hero* hero){
        float heroPosX = (*hero).getPositionX();
        float heroPosY = (*hero).getPositionY();

        float heroDistance = sqrt(pow(heroPosY - yPos, 2) + pow(heroPosX - xPos, 2));

        if(heroDistance <= DETECTRANGE){
            chaseHero(heroPosX, heroPosY, heroDistance);
        }
        else if(heroDistance > DETECTRANGE){
            wander();
        }

        (*hero).setGrabbed(grabbing);
    }


private:
    bool chasing;
    bool grabbing;
    bool wounded;
    bool wandering;

    float xDirection;
    float yDirection;
};

Enemy::Enemy(){
    spriteY = 4 + spriteHeight;
    spriteWidth = 11;
    xPos = rand()%WW;
    yPos = rand()%WH;
    chasing = false;
    grabbing = false;
    wounded = false;
}
class Bullet{
public:

    Bullet(Hero* hero);

    void setSprite(sf::Texture* texture){
        sprite.setTexture(*texture);
        sprite.setTextureRect(sf::IntRect(5, 27, 2, 1));
        sprite.setScale(SCALE, SCALE);
        sprite.setRotation(direction*180/M_PI);
    }
    sf::Sprite getSprite(){
        return sprite;
    }
    void moveBullet(){
        xPos += cos(direction)*2*BASESPEED;
        yPos += sin(direction)*2*BASESPEED;

        sprite.setPosition(xPos, yPos);
    }
    float getPositionX(){
        return xPos;
    }
    float getPositionY(){
        return yPos;
    }


private:
    sf::Sprite sprite;
    float xPos;
    float yPos;
    float direction;
};

Bullet::Bullet(Hero* hero){
    xPos = (*hero).getPositionX();
    yPos = (*hero).getPositionY();
    direction = (*hero).getLookAngle();
}

int main()
{
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(WW, WH), "Survival Game 1993");

    sf::Image spriteSheetImg;
    if(!spriteSheetImg.loadFromFile("MasterSpriteSheet.png")){
        cout << "Failed to load image" << endl;
    }
    spriteSheetImg.createMaskFromColor(sf::Color::Magenta);
    sf::Texture spriteSheetTxt;
    if(!spriteSheetTxt.loadFromImage(spriteSheetImg)){
        cout << "Failed to load texture" << endl;
    }

    sf::Sprite roadSprite[2];
    for(int road = 0; road < 2; road++){
        roadSprite[road].setTexture(spriteSheetTxt);
        roadSprite[road].setTextureRect(sf::IntRect(4, 49, 155, 51));
        roadSprite[road].setScale(SCALE, SCALE);
        roadSprite[road].setPosition(-45*SCALE + 100*SCALE*road, WH/3);
    }


    Hero hero;
    int numEnemies = 1 + rand()%10;
    Enemy enemy[numEnemies];

    list<Bullet*> bullet_list;
    time_t currentTime;
    time_t timeOfPreviousBullet = 0;

    hero.setSprite(&spriteSheetTxt);
    for(int x = 0; x < numEnemies; x++){
        enemy[x].setSprite(&spriteSheetTxt);
    }

    while(window.isOpen()){
        sf::Event event;

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){;
            currentTime = clock();
            time_t timeSinceLastBullet = currentTime - timeOfPreviousBullet;
            if(timeSinceLastBullet > FIRERATE){
                bullet_list.push_back(new Bullet(&hero));
                (*(bullet_list.back())).setSprite(&spriteSheetTxt);
                timeOfPreviousBullet = currentTime;
            }
        }

        window.clear(sf::Color::Green);

        hero.moveChar(&window);
        for(int x = 0; x < numEnemies; x++){
            enemy[x].moveZombie(&hero);
        }

        for(int road = 0; road < 2; road++){
            window.draw(roadSprite[road]);
        }



        for(list<Bullet*>::iterator it=bullet_list.begin(); it != bullet_list.end(); ++it){

            if((*(*it)).getPositionX() < 0 || (*(*it)).getPositionX() > WW || (*(*it)).getPositionY() < 0 || (*(*it)).getPositionY() > WH){

                it = bullet_list.erase(it);
                cout << "Bullet Erased" << endl;
            }
            else{
                (*(*it)).moveBullet();
                window.draw((*(*it)).getSprite());
            }
        }

        window.draw(hero.getSprite());

        for(int x = 0; x < numEnemies; x++){
            window.draw(enemy[x].getSprite());
        }



        window.display();

    }
}
