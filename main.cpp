#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Constants.h"
#include "Entity.h"
#include "Hero.h"
#include "Enemy.h"
#include "Weapons.h"
#include "Bullet.h"
#include "Collision.h"
#include "Command.h"
#include "Weapons.h"

#include <math.h>
#include <list>
#include <ctime>

using namespace std;

void setTexture(sf::Texture* texture, string fileName){
    sf::Image image;
    if(!image.loadFromFile(fileName)){
        cout << "Failed to load image" << endl;
    }
    image.createMaskFromColor(sf::Color::MASKCOLOUR);

    if(!(*texture).loadFromImage(image)){
        cout << "Failed to load texture" << endl;
    }
}

void displayEverything(sf::RenderWindow* window, Hero* hero, list<Weapons*>* weapons_list, list<Enemy*>* enemy_list, list<Bullet*>* bullet_list){

    (*window).clear(sf::Color::Green);

    for(list<Bullet*>::iterator bullet_it = (*bullet_list).begin(); bullet_it != (*bullet_list).end(); bullet_it++){
        (*window).draw((*(*bullet_it)).getSprite());
    }

    (*window).draw((*hero).getSprite());

    list<Weapons*>::iterator weapons_it;
    for(weapons_it = (*weapons_list).begin(); weapons_it != (*weapons_list).end() && (*(*weapons_it)).getEquipped() == false; weapons_it++){
    }
    (*window).draw((*(*weapons_it)).getSprite());



    for(list<Enemy*>::iterator enemy_it = (*enemy_list).begin(); enemy_it != (*enemy_list).end(); enemy_it++){
        (*window).draw((*(*enemy_it)).getSprite());
    }

    (*window).display();

}

void updatePositions(sf::RenderWindow* window, Hero* hero, list<Weapons*>* weapons_list, list<Enemy*>* enemy_list, list<Bullet*>* bullet_list, Command* command){
    //Move character using mouse & keyboard, followed by weapon
    (*hero).moveChar((*command).getMouseXY(),(*command).getCommands());

    for(list<Weapons*>::iterator weapon_it = (*weapons_list).begin(); weapon_it != (*weapons_list).end(); weapon_it++){
        (*(*weapon_it)).followCharacter(hero);
    }

    //Move zombie (wandering or chasing)
    for(list<Enemy*>::iterator enemy_it = (*enemy_list).begin(); enemy_it != (*enemy_list).end(); enemy_it++){
        (*(*enemy_it)).moveZombie((*hero).getPositionX(), (*hero).getPositionY());
    }
    //Move bullet
    for(list<Bullet*>::iterator bullet_it = (*bullet_list).begin(); bullet_it != (*bullet_list).end(); bullet_it++){
        (*(*bullet_it)).moveBullet();
    }
}

void shoot(Command* command, list<Bullet*>* bullet_list, sf::Texture* texture, list<Weapons*>* weapons_list){
    static time_t lastTime = 0;
    time_t currentTime = clock();
    time_t timeSinceLastBullet = currentTime - lastTime;

    static int bulletAlternate = -1;

    static list<Weapons*>::iterator weapons_it = (*weapons_list).begin();

    (*(*weapons_it)).setEquipped(false);

    if((*command).getScroll() == -1 && weapons_it != (*weapons_list).end()){
        weapons_it++;
    }
    else if((*command).getScroll() == 1 && weapons_it != (*weapons_list).begin()){
        weapons_it--;
    }
    if(weapons_it == (*weapons_list).end()){
        weapons_it--;
    }

    (*(*weapons_it)).setEquipped(true);

    float angle = (*(*weapons_it)).getOrientation();
    float xSpawn = (*(*weapons_it)).getPositionX() + (*(*weapons_it)).getSpriteWidth()*SCALE*cos(angle) + bulletAlternate*(*(*weapons_it)).getBulletSpawnDistance()*SCALE*cos(angle + M_PI/2);
    float ySpawn = (*(*weapons_it)).getPositionY() + (*(*weapons_it)).getSpriteWidth()*SCALE*sin(angle) + bulletAlternate*(*(*weapons_it)).getBulletSpawnDistance()*SCALE*sin(angle + M_PI/2);

    bool* shoot = (*command).getCommands();


    if(shoot[LCLICK] == true && timeSinceLastBullet > 1.0/(*(*weapons_it)).getFireRate()){
        (*bullet_list).push_back(new Bullet(texture, xSpawn, ySpawn, (*(*weapons_it)).getOrientation()));
        lastTime = currentTime;
        bulletAlternate *= -1;
    }
}

void checkCollision(Hero* hero, list<Enemy*>* enemy_list, list<Bullet*>* bullet_list){

    //Check for Bullet collision
    for(list<Bullet*>::iterator bullet_it = (*bullet_list).begin(); bullet_it != (*bullet_list).end();){

        bool collision = false;
        bool outOfBounds = ((*(*bullet_it)).getPositionX() < 0 || (*(*bullet_it)).getPositionX() > WW || (*(*bullet_it)).getPositionY() < 0 || (*(*bullet_it)).getPositionY() > WH);
        for(list<Enemy*>::iterator enemy_it = (*enemy_list).begin(); enemy_it != (*enemy_list).end() && collision == false;){
            collision = (*(*bullet_it)).getSprite().getGlobalBounds().intersects((*(*enemy_it)).getSprite().getGlobalBounds());
            if(collision == true){
                (*(*enemy_it)).causeDamage(20);
                if((*(*enemy_it)).getAlive() == false){
                    enemy_it = (*enemy_list).erase(enemy_it);
                }
            }
            else{
                enemy_it++;
            }
        }

        if(collision == true || outOfBounds == true){
            bullet_it = (*bullet_list).erase(bullet_it);
        }
        else{
            bullet_it++;
        }
    }
}


int main()
{

    srand(time(0));

    //Setting up the window
    sf::RenderWindow window(sf::VideoMode(WW, WH), "Justin's Survival Adventure 1993");
    window.setFramerateLimit(60);
    window.setPosition(sf::Vector2i(20, 20));

    //Setting up the master spritesheet
    sf::Texture spriteSheetTexture;
    setTexture(&spriteSheetTexture, "MasterSpriteSheet.png");

    //Initialize entities
    Hero hero(&spriteSheetTexture);

    list<Enemy*> enemy_list;
    list<Weapons*> weapons_list;
    list<Bullet*> bullet_list;


    int numEnemies = NUMENEMIES;
    for(int enemy = 0; enemy < numEnemies; enemy++){
        enemy_list.push_back(new Enemy(&spriteSheetTexture));
    }

    for(int weapon = 0; weapon < NUMWEAPONS; weapon++){
        weapons_list.push_back(new Weapons(&spriteSheetTexture, weapon));
    }

    Command command;

    while(window.isOpen()){
        sf::Event event;

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }

        }
        command.checkUserInput(&window, &event);
        updatePositions(&window, &hero, &weapons_list, &enemy_list, &bullet_list, &command);
        shoot(&command, &bullet_list, &spriteSheetTexture, &weapons_list);
        displayEverything(&window, &hero, &weapons_list, &enemy_list, &bullet_list);
        checkCollision(&hero, &enemy_list, &bullet_list);

    }
}
