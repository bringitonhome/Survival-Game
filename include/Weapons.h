#ifndef WEAPONS_H
#define WEAPONS_H

#include "Entity.h"
#include "Hero.h"
//#include <string.h>

using namespace std;

class Weapons : public Entity
{
    public:
        Weapons(sf::Texture*, int);
        void followCharacter(Hero*);
        float getFireRate();
        float getBulletSpawnDistance();
        bool getEquipped();
        bool setEquipped(bool);
    protected:
        float firerate;
        float bulletSpawnDistance;
        bool equipped;
    private:
};

#endif // WEAPONS_H
