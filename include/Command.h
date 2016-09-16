#ifndef COMMAND_H
#define COMMAND_H

#include "Constants.h"
#include <SFML/Window.hpp>
#include "Bullet.h"

class Command
{
    public:;
        void checkUserInput(sf::RenderWindow*);
        bool* getCommands();

        sf::Vector2i getMouseXY();

    protected:
        bool commands[NUMCOMMANDS];
        sf::Vector2i mouseXY;
    private:
};

#endif // COMMAND_H
