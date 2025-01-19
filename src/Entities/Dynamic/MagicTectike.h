#pragma once

#include "DynamicEntity.h"

class MagicTek : public DynamicEntity {
    private:
        int counter;
        const int WAITING_TIME = 60;
        const int MOVEMENT_TIME = 30;
        int shootCoolDown = 0;
        int shootInterval = 60;

    public: 
        MagicTek(int x, int y, shared_ptr<Handler> handler, shared_ptr<DynamicEntityAnimations> animations) : DynamicEntity(x, y, handler, animations) {
            this->MAX_HEALTH = 4;
            this->health = MAX_HEALTH;
            this->counter = 0;
        };
        bool isEnemy();
        void update();
        void damage(int damage);
        void onDeath();
        void onCollision(shared_ptr<DynamicEntity> entity){};
        void takeDamage(int amount);
        void ShootThatBoy();
};