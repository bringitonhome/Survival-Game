#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Hero.h"


class Enemy : public Entity
{
public:

    Enemy(sf::Texture*);

    void wander();

    void chaseHero(float, float, float);

    void moveZombie(float, float);


private:
    bool chasing;
    bool grabbing;
    bool wounded;
    bool wandering;

    bool alive;

    float xDirection;
    float yDirection;

};

#endif // ENEMY_H
