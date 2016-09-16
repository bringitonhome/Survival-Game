#include "Collision.h"

#include <list>
#include "Bullet.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp>

Collision::Collision()
{
    //ctor
}

void Collision::checkBulletImpact(list<Bullet*>* bullet_list, list<Enemy*>* enemy_list){
    for(list<Bullet*>::iterator bullet_it = (*bullet_list).begin(); bullet_it != (*bullet_list).end(); bullet_it){
        for(list<Enemy*>::iterator enemy_it = (*enemy_list).begin(); enemy_it != (*enemy_list).end(); enemy_it++){
            bool impact = (*(*bullet_it)).getSprite().getGlobalBounds().intersects((*(*enemy_it)).getSprite().getGlobalBounds());
            if(impact == true){
                enemy_it = (*enemy_list).erase(enemy_it);
                bullet_it = (*bullet_list).erase(bullet_it);
            }
        }
    }
}
