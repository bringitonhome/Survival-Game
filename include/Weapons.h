#ifndef WEAPONS_H
#define WEAPONS_H

#include "Entity.h"
#include "Hero.h"
//#include <string.h>

using namespace std;

class Weapons : public Entity
{
    public:
        Weapons(sf::Texture*, string);
        void followCharacter(Hero*);
        float getFireRate();
        float getBulletSpawnDistance();
    protected:
        float firerate;
        float bulletSpawnDistance;
    private:
};

#endif // WEAPONS_H
