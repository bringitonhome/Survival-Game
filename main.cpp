#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <math.h>
#include <ctime>
#include <list>

#define WW 1000
#define WH 600
#define SCALE 4
#define BASESPEED 5

#define DETECTRANGE 200
#define FIRERATE 0

#define WANDERRATE 5000

using namespace std;


static sf::Texture spriteSheetTxt;

void setSprite(sf::Sprite* sprite, int spriteOriginX, int spriteOriginY, int spriteWidth, int spriteHeight){
    (*sprite).setTexture(spriteSheetTxt);
    (*sprite).setTextureRect(sf::IntRect(spriteOriginX, spriteOriginY, spriteWidth, spriteHeight));
    (*sprite).setScale(SCALE, SCALE);
}

// Base class
class Character{
public:

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
    float getSpeed(){
        return speed;
    }

protected:
    sf::Sprite sprite;
    int spriteOriginX;
    int spriteOriginY;
    int spriteWidth;
    int spriteHeight;

    float xPos;
    float yPos;
    float lookAngle;

    float speed;
};

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
            xPos += speed*cos(moveAngle) + (xPos < -11*SCALE)*(WW+22*SCALE) - (xPos > WW + 11*SCALE)*(WW+22*SCALE);
            yPos += speed*sin(moveAngle) + (yPos < -11*SCALE)*(WH+22*SCALE) - (yPos > WH + 11*SCALE)*(WH+22*SCALE);
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
    spriteOriginX = 5;
    spriteOriginY = 5;
    spriteWidth = 13;
    spriteHeight = 11;
    sprite.setOrigin(5.5, 5.5);
    xPos = WW/4;
    yPos = WH/2;
    speed = BASESPEED;

    grabbed = false;

    setSprite(&sprite, spriteOriginX, spriteOriginY, spriteWidth, spriteHeight);
}

class Enemy : public Character{
public:
    Enemy();

    void killZombie(){
        alive = false;
    }

    bool getAlive(){
        return alive;
    }

    void setSpeed(){
        speed = (BASESPEED/2)*(1+0.5*chasing)/((1 + wounded)*(1 + 4*wandering)*(1 + grabbing));
    }

    void wander(){

        chasing = false;
        wandering = true;

        bool newDirection = (rand()%1000 <= 10);
        if(newDirection == true){
            lookAngle = (rand()%360)*M_PI/180;
        }
    }

    void chaseHero(float heroPosX, float heroPosY, float heroDistance){

        wandering = false;
        chasing = true;

        lookAngle = atan2(heroPosY - yPos, heroPosX - xPos);

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
            (*hero).setGrabbed(grabbing);
        }
        else if(heroDistance > DETECTRANGE){
            wander();
        }

        setSpeed();

        xPos += cos(lookAngle)*(1 - 2*(heroDistance < 50))*speed + (xPos < -6*SCALE)*(WW+12*SCALE) - (xPos > WW+6*SCALE)*(WW+12*SCALE);
        yPos += sin(lookAngle)*(1 - 2*(heroDistance < 50))*speed + (yPos < -6*SCALE)*(WH+12*SCALE) - (yPos > WH+6*SCALE)*(WH+12*SCALE);

        sprite.setRotation(lookAngle*180/M_PI);
        sprite.setPosition(xPos, yPos);

    }


private:
    bool chasing;
    bool grabbing;
    bool wounded;
    bool wandering;

    bool alive;

    float xDirection;
    float yDirection;
};

Enemy::Enemy(){
    spriteOriginX = 5;
    spriteOriginY = 20;
    spriteWidth = 11;
    spriteHeight = 11;
    sprite.setOrigin(5.5, 5.5);
    setSprite(&sprite, spriteOriginX, spriteOriginY, spriteWidth, spriteHeight);

    xPos = rand()%WW;
    yPos = rand()%WH;

    chasing = false;
    grabbing = false;
    wounded = false;
    alive = true;


}

class Bullet{
public:

    Bullet(Hero* hero);

    sf::Sprite getSprite(){
        return sprite;
    }
    void moveBullet(){
        xPos += cos(direction)*2*BASESPEED;
        yPos += sin(direction)*2*BASESPEED;

        sprite.setPosition(xPos, yPos);
    }

    void setBulletSpeed(){
        bulletSpeed = 2*BASESPEED;
    }

    bool checkForImpact(list<Enemy*>* enemy_list){
        //impact = (*enemy).getSprite().getGlobalBounds().intersects(sprite.getGlobalBounds());
        for(list<Enemy*>::iterator it = (*enemy_list).begin(); it != (*enemy_list).end(); ++it){
            impact = (sprite.getGlobalBounds().intersects((*(*it)).getSprite().getGlobalBounds()));
            if(impact == true){
                (*enemy_list).erase(it);
                return impact;
            }
        }
        return impact;
    }

    float getPositionX(){
        return xPos;
    }
    float getPositionY(){
        return yPos;

    }

    float getImpactAngle(){
        return direction;
    }

    float getBulletSpeed(){
        return bulletSpeed;
    }


private:
    sf::Sprite sprite;

    bool impact;

    int spriteOriginX;
    int spriteOriginY;
    int spriteWidth;
    int spriteHeight;

    float xPos;
    float yPos;
    float direction;
    float bulletSpeed;
};

Bullet::Bullet(Hero* hero){

    xPos = (*hero).getPositionX();
    yPos = (*hero).getPositionY();
    direction = (*hero).getLookAngle();
    sprite.setRotation(direction*180/M_PI);

    impact = false;

    spriteOriginX = 5;
    spriteOriginY = 35;
    spriteWidth = 2;
    spriteHeight = 1;

    setSprite(&sprite, spriteOriginX, spriteOriginY, spriteWidth, spriteHeight);


    setBulletSpeed();

}

class Gore{
public:
    Gore(Bullet* bullet);

    sf::Sprite getSprite(){
        return sprite;
    }

private:
    float spawnX;
    float spawnY;
    float impactAngle;
    float goreSpeed;

    sf::Sprite sprite;
};

Gore::Gore(Bullet* bullet){
    spawnX = (*bullet).getPositionX();
    spawnY = (*bullet).getPositionY();
    impactAngle = (*bullet).getImpactAngle();
    goreSpeed = (*bullet).getBulletSpeed();
}

class Blood : public Gore{
public:
    Blood(Bullet* bullet) : Gore::Gore(bullet){
    }

private:

};

class Chunks : public Gore{
public:
    Chunks(Bullet* bullet) : Gore::Gore(bullet){

    }

private:

};


int main()
{
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(WW, WH), "Survival Game 1993");
    window.setFramerateLimit(60);

    sf::Image spriteSheetImg;
    if(!spriteSheetImg.loadFromFile("MasterSpriteSheet.png")){
        cout << "Failed to load image" << endl;
    }
    spriteSheetImg.createMaskFromColor(sf::Color::Magenta);

    if(!spriteSheetTxt.loadFromImage(spriteSheetImg)){
        cout << "Failed to load texture" << endl;
    }

    Hero hero;

    list<Bullet*> bullet_list;
    list<Enemy*> enemy_list;

    time_t currentTime;
    time_t timeOfPreviousBullet = 0;

    int numEnemies = 5000;

    for(int enemy = 0; enemy < numEnemies; enemy++){
        enemy_list.push_back(new Enemy);
    }


    while(window.isOpen()){
        sf::Event event;

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        window.clear(sf::Color::Green);

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){;
            currentTime = clock();
            time_t timeSinceLastBullet = currentTime - timeOfPreviousBullet;
            if(timeSinceLastBullet > FIRERATE){
                bullet_list.push_back(new Bullet(&hero));
                timeOfPreviousBullet = currentTime;
            }
        }



        hero.moveChar(&window);

        for(list<Enemy*>::iterator it1 = enemy_list.begin(); it1 != enemy_list.end(); ++it1){
            (*(*it1)).moveZombie(&hero);
        }




        for(list<Bullet*>::iterator it=bullet_list.begin(); it != bullet_list.end(); ++it){
            bool impact = false;

            impact = (*(*it)).checkForImpact(&enemy_list);

            if((*(*it)).getPositionX() < -100 || (*(*it)).getPositionX() > WW + 100 || (*(*it)).getPositionY() < -100 || (*(*it)).getPositionY() > WH + 100 || impact == true){
                it = bullet_list.erase(it);
            }
            else{
                (*(*it)).moveBullet();
                window.draw((*(*it)).getSprite());
            }
        }



        window.draw(hero.getSprite());

        for(list<Enemy*>::iterator it=enemy_list.begin(); it != enemy_list.end(); ++it){
            window.draw((*(*it)).getSprite());
        }



        window.display();

    }
}
