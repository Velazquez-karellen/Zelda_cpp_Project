#include "RedDarkNut.h"
#include "Map.h"
#include "GameState.h"

void RedDarkNut::update(){

    counter++;
    if (movingDir[UP] || movingDir[DOWN] || movingDir[LEFT] || movingDir[RIGHT]){
        if (counter == MOVEMENT_TIME){
            movingDir[UP] = false;
            movingDir[DOWN] = false;
            movingDir[LEFT] = false;
            movingDir[RIGHT] = false;
            counter = 0;
        }
    } 
     else{
        // Time to move in a random direction
        if (counter == WAITING_TIME){
            movingDir[UP] = false;
            movingDir[DOWN] = false;
            movingDir[LEFT] = false;
            movingDir[RIGHT] = false;
            Direction direction = static_cast<Direction>(LinkDirect);
            movingDir[direction] = true;
            counter = 0;
        }
    }


    DynamicEntity::update();
}



void RedDarkNut::damage(int damage){
    if (dead) return;
    // Play the damage sound if we're about to take damage
    if (invincibilityFrames == 0)
        handler->getSoundEffectManager()->playSoundEffect("enemy_hurt");
        
    DynamicEntity::damage(damage);
}

void RedDarkNut::onDeath(){
    handler->getSoundEffectManager()->playSoundEffect("enemy_death");
}
bool RedDarkNut::isEnemy(){
    return true;
}
void RedDarkNut::takeDamage(int amount){
    if(dead) return;

    if(invincibilityFrames == 0){
        this->health -= amount;
        this->invincibilityFrames = 15;
        handler->getSoundEffectManager()->playSoundEffect("enemy_hurt");

        if(this->health <= 0){
            this->health = 0;
            this->dead = true;
            onDeath();
        }
    }
}
void RedDarkNut::onCollision(shared_ptr<DynamicEntity> entity){
      auto link = dynamic_pointer_cast<Link>(entity);
    if (link) {
       link->damage(link->getHealth());

    }
}