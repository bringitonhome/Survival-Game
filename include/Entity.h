#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>

class Entity
{
    public:

        //Set Functions
        void setSprite(sf::Texture*);
        void setSpeed();
        void causeDamage(int);

        //Get Functions
        sf::Sprite getSprite();
        float getPositionX();
        float getPositionY();
        float getOrientation();
        float getSpeed();

        int getSpriteWidth();
        int getSpriteHeight();
        int getHealth();

        bool getAlive();
        int getDamage();

    protected:
        //Sprite info
        sf::Sprite sprite;
        int spriteOriginX;
        int spriteOriginY;
        int spriteWidth;
        int spriteHeight;

        //Coordinate info
        float xPos;
        float yPos;
        float orientation;

        float speed;
        bool speedIncrease;
        bool speedDecrease;

        float typeMult;

        int health;
        bool alive;
        int damage;

    private:
};

#endif // CHARACTER_H
