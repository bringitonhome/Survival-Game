#ifndef BULLET_H
#define BULLET_H

#include <list>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Hero.h"
#include "Enemy.h"

using namespace std;

class Bullet : public Entity
{
    public:
        Bullet(sf::Texture*, float, float, float);

        void moveBullet();

    protected:

    private:
};

#endif // BULLET_H
