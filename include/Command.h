#ifndef COMMAND_H
#define COMMAND_H

#include "Constants.h"
#include <SFML/Window.hpp>
#include "Bullet.h"

class Command
{
    public:;
        void checkUserInput(sf::RenderWindow*, sf::Event*);
        bool* getCommands();

        sf::Vector2i getMouseXY();
        int getScroll();
    protected:
        bool commands[NUMCOMMANDS];
        sf::Vector2i mouseXY;
        int scroll;
    private:
};

#endif // COMMAND_H
