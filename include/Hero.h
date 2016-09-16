#ifndef HERO_H
#define HERO_H

#include <SFML/Window.hpp>

#include "Entity.h"
#include "Command.h"


class Hero : public Entity
{
    public:
        Hero(sf::Texture*);

        //Set variable
        void setGrabbed(bool);

        //Do things
        void moveChar(sf::Vector2i, bool*);

    protected:
        bool grabbed;
        bool running;


    private:
};

#endif // HERO_H
