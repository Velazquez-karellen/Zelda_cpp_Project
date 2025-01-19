#pragma once

#include "DynamicEntity.h"
#include "Link.h"

class RedDarkNut : public DynamicEntity{
    private:
        int counter;
        const int WAITING_TIME = 1;
        const int MOVEMENT_TIME = 1;


    public: 
        RedDarkNut(int x, int y, shared_ptr<Handler> handler, shared_ptr<DynamicEntityAnimations> animations) : DynamicEntity(x, y, handler, animations), counter(0) {
            this->x = x;
            this->y = y;
            this->MAX_HEALTH = 1;
            this->health = MAX_HEALTH;
            this->counter = 0;
            
        };
        Direction LinkDirection = RIGHT;
        bool isEnemy();
        void update();
        void damage(int damage);
        void onDeath();
        void onCollision(shared_ptr<DynamicEntity> entity);
        void takeDamage(int amount);
        Direction LinkDirect = RIGHT;
};