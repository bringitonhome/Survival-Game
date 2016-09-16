#include "Command.h"
#include <ctime>
#include <SFML/Window.hpp>

void Command::checkUserInput(sf::RenderWindow* window){


    //Check Mouse
    int mouseX = sf::Mouse::getPosition(*window).x;
    int mouseY = sf::Mouse::getPosition(*window).y;
    mouseXY = sf::Vector2i(mouseX, mouseY);

    commands[LCLICK] = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    //Check Keyboard
    commands[WKEY] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    commands[AKEY] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    commands[SKEY] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    commands[DKEY] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    commands[LSHIFTKEY] = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

}

bool* Command::getCommands(){
    return commands;
}

sf::Vector2i Command::getMouseXY(){
    return mouseXY;
}
