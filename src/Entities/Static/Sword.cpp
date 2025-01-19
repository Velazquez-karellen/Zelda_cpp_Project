#include "Sword.h"
#include "Link.h"


Sword::Sword(int x, int y, std::shared_ptr<Handler> handler)
    : Item(x, y, handler, handler->getImageManager()->getSpriteFromEntitySheetAt(2, 4)) {

}

void Sword::applyEffect(std::shared_ptr<Link> link) {
    if(!getIsCollected()){
    link->pickup();
    link->setHasSword(true);
    handler->getSoundEffectManager()->playSoundEffect("item_get");
    isCollected = true;
    }

}

void Sword::draw(){
    if(!getIsCollected()){
      Entity::draw();
    }
}