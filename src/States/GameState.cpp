#include "GameState.h"
#include "Section.h"
#include "Link.h"
#include "Animation.h"

void GameState::update()
{
    if(!paused){
        this->currentMap->update();
    }
    

    if(currentMap->isFinished()){
        currentMap->setFinished(false);
        if(currentMap->getWhereTo() == OVER_WORLD){
            //Set Link's X and Y coordinates relative to the coming map section's door positon
            currentMap->getPlayer()->setX(overWorldMap->getCurrentSection()->doorXPos);
            currentMap->getPlayer()->setY(overWorldMap->getCurrentSection()->doorYPos + Handler::TILE_HEIGHT);
            //TODO: Remove Link from being part of the Map to avoid multiple Links in diff maps.
            // For now we use the setter to have the same link in both maps
            overWorldMap->setPlayer(currentMap->getPlayer());
            currentMap = overWorldMap;
        }
        else if(currentMap->getWhereTo() == OLD_MANS_CAVE){
            currentMap->getPlayer()->setX(oldManCaveMap->getCurrentSection()->doorXPos);
            currentMap->getPlayer()->setY(oldManCaveMap->getCurrentSection()->doorYPos - Handler::TILE_HEIGHT);
            oldManCaveMap->setPlayer(currentMap->getPlayer());
            currentMap = oldManCaveMap;
        }
        else if (currentMap->getWhereTo() == DUNGEON) {
            currentMap->getPlayer()->setX(newDungeon->getCurrentSection()->doorXPos);
            currentMap->getPlayer()->setY(newDungeon->getCurrentSection()->doorYPos - Handler::TILE_HEIGHT);
            newDungeon->setPlayer(currentMap->getPlayer());
            currentMap = newDungeon;
        }
        else if (currentMap->getWhereTo() == DUNGEON1) {
            currentMap->getPlayer()->setX(newDungeon1->getCurrentSection()->doorXPos);
            currentMap->getPlayer()->setY(newDungeon1->getCurrentSection()->doorYPos - Handler::TILE_HEIGHT);
            newDungeon1->setPlayer(currentMap->getPlayer());
            currentMap = newDungeon1;
        }
    }


    auto link = currentMap->getPlayer();
    if(link && link->isAttacking() && link->getHasSword()){
        ofRectangle swordHitbox = link->getSwordHitbox();

        for(auto& enemy : currentMap->getCurrentSection()->getEnemies()){
            if(enemy && !enemy->isDead() && swordHitbox.intersects(enemy->getBounds())){
                auto tektike = dynamic_pointer_cast<Tektike>(enemy);
                auto redDarkNut = dynamic_pointer_cast<RedDarkNut>(enemy);
                 auto magicTek = dynamic_pointer_cast<MagicTek>(enemy);
                if(tektike){
                    tektike->takeDamage(1);
                } else if(redDarkNut){
                    redDarkNut->takeDamage(1);
                } else if(magicTek){
                    magicTek->takeDamage(1);
                }
            }
        }
    }

    for (auto& enemy : currentMap->getCurrentSection()->getEnemies()) {
        auto redDarkNut = dynamic_pointer_cast<RedDarkNut>(enemy);
        if (redDarkNut) {
            float RedX = redDarkNut->getX();
            float RedY = redDarkNut->getY();

            if (link) {
                if (link->getX() < RedX) {
                    redDarkNut->LinkDirect = LEFT;
                } else if (link->getX() > RedX) {
                    redDarkNut->LinkDirect = RIGHT;
                } else if (link->getY() < RedY) {
                    redDarkNut->LinkDirect = UP;
                } else if (link->getY() > RedY) {
                    redDarkNut->LinkDirect = DOWN;
                }
            }
        }
    }
    

    //Update Link and other entities
    auto currentSection = currentMap->getCurrentSection();
    if(currentSection && link){
        for(auto& itemList: currentSection->getItems()){
            for(auto& item : itemList){
                if (item && !item->getIsCollected() && link->getBounds().intersects(item->getBounds())) {
                    item->onCollision(link);
                }
            }
        }
   }

    if (currentSection && link) {
        int counter = 0;
            for (auto& enemy : currentSection->getEnemies()) {
                if (enemy && !enemy->isDead() && link->getBounds().intersects(enemy->getBounds())) {
                    link->onCollision(enemy);
                }
            }
    }

    // Continue with other game updates
    if(link){
    link->update();
    }
}


void GameState::draw() {
    ofSetColor(255, 255, 255);
    currentMap->draw();

    // Change to Game Over
    if (currentMap->getPlayer()->isDead()) {
        ofImage gameOver = handler->getImageManager()->gameOverScreen;
        gameOver.draw(0, 0, ofGetWidth(), ofGetHeight());
        changeState++;
        if (changeState == 120) {
            this->setFinished(true);
            this->setNextState(GAME_OVER);
        }
    }

    if (paused) {
        // Full Screen HUD Transition Parameters
        hudHeight <= ofGetHeight() ? hudHeight += Handler::PIXEL_SCALE*2 : hudHeight;
        hudH <= 0 ? hudH += Handler::PIXEL_SCALE*2 : hudH;

    } else {
        // Small Screen HUD Transition Parameters
        hudHeight > ofGetHeight()/4 ? hudHeight -= Handler::PIXEL_SCALE*2 : hudHeight;
        hudH > -Handler::SECTION_HEIGHT ? hudH -= Handler::PIXEL_SCALE*2 : hudH;
    }
    handler->getImageManager()->HUD.draw(0, hudH, ofGetWidth(), ofGetHeight());

    // Draw hearts on the HUD
    this->currentMap->getPlayer()->drawHearts(hudHeight-10);

    // Draw Sword on the HUD
    if(currentMap->getPlayer()->hasSword){
        handler->getImageManager()->basicSword.draw(ofGetWidth()/2 - (ofGetWidth() % 10), hudHeight - 90, 40, 40);
    }

    // MiniMap
    ofDrawRectangle(this->currentMap->getPlayer()->getX()/20 + 40 * (currentMap->getCurrentSectionX()+1),  
                    hudHeight + (this->currentMap->getPlayer()->getY()/15  + 50 * (currentMap->getCurrentSectionY()+1)) - 175, 5, 5);
}
void GameState::reset() {
    this->musicFileName = "overworld.wav";
    mapBuilder = make_unique<MapBuilder>(handler);
    overWorldMap = mapBuilder->createMap(handler->getImageManager()->overWorldPixelMap);
    oldManCaveMap = mapBuilder->createMap(handler->getImageManager()->oldMansCavePixelMap);
    //If there's more than one section in a map, besides overWorld, we must set which section we want to start at
    oldManCaveMap->setCurrentSection(0,0);
    oldManCaveMap->getSection(0,0)->addItemToTile(8, 6, createSword());

    newDungeon = mapBuilder->createMap(handler->getImageManager()->dungeonPixelMap);
    newDungeon->setCurrentSection(0, 1);

    newDungeon1 = mapBuilder->createMap(handler->getImageManager()->dungeonPixelMap1);
    newDungeon1->setCurrentSection(0, 0);

    overWorldMap->getSection(2, 0)->addEnemyToTile(8, 4, createTektike());
    overWorldMap->getSection(2, 0)->addEnemyToTile(4, 4, createTektike());
    overWorldMap->getSection(3, 1)->addEnemyToTile(6, 4, createTektike());
    overWorldMap->getSection(4, 0)->addEnemyToTile(8, 8, createRedDarkNut());
    overWorldMap->getSection(1, 0)->addEnemyToTile(10, 3, createRedDarkNut());
    overWorldMap->getSection(3, 0)->addEnemyToTile(8, 4, createMagicTek());

    currentMap = overWorldMap;

    paused = false;
    hudH = -Handler::SECTION_HEIGHT;
    hudHeight = Handler::HUD_HEIGHT;
    changeState = 0;
}

void GameState::keyPressed(int key) {
    if (key == 'p'){
        paused = !paused;
    } else {
        currentMap->keyPressed(key);
    }
}
void GameState::keyReleased(int key) {
    currentMap->keyReleased(key);
}

shared_ptr<Tektike> GameState::createTektike() {
    // getTekitkeAnimations() returns a NEW shared_ptr
    // This is so that all Tektikes will not point
    // to the same animation object and "super tick" 
    // each other
    return make_shared<Tektike>(0, 0, handler, handler->getImageManager()->getTektikeAnimations());
}

//create the sword so it appears in the cave
shared_ptr<Item> GameState::createSword() {
   
    return make_shared<Sword>(0, 0, handler);  // 0, 0 position for now
}
  shared_ptr<RedDarkNut>GameState::createRedDarkNut(){
     return make_shared<RedDarkNut>(0, 0, handler, handler->getImageManager()->getRedDarkNutAnimations());
  }

  shared_ptr<MagicTek>GameState::createMagicTek(){
    return make_shared<MagicTek>(0,0, handler, handler->getImageManager()->getMagicTektikeAnimations());
  }