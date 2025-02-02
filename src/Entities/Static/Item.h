#pragma once

#include "StaticEntity.h"
#include <memory>

class Link;


class Item : public StaticEntity {

    protected:
        bool isCollected;

    public:
        Item(int x, int y, shared_ptr<Handler> handler, ofImage sprite) : StaticEntity(x, y, handler) {
            this->currentSprite = sprite;
            isCollected = false;
        }
        ~Item(){};
        void onCollision(shared_ptr<DynamicEntity> e);
        virtual bool isSword() { return false; } 
        virtual void applyEffect(shared_ptr<Link> link) = 0;
        
        bool getIsCollected(){return isCollected;}
};