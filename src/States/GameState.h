#pragma once

#include "Animation.h"
#include "Link.h"
#include "Tektike.h"
#include "MapBuilder.h"
#include "State.h"
#include <memory>
#include "Sword.h"
#include "Item.h"
#include "RedDarkNut.h"
#include "MagicTectike.h"

class GameState : public State {

private:
    shared_ptr<Map> currentMap;
    shared_ptr<Map> overWorldMap;
    shared_ptr<Map> oldManCaveMap;
    shared_ptr<Map> newDungeon;
    shared_ptr<Map> newDungeon1;
    unique_ptr<MapBuilder> mapBuilder;
    shared_ptr<Sword> sword;
    shared_ptr<Link> link;
    shared_ptr<Item> item;
    shared_ptr<RedDarkNut> redNut;
   
public:
    GameState(shared_ptr<Handler> handler) : State(handler) { this->reset(); }
    void update();
    void draw();
    void reset();
    void keyPressed(int key);
    void keyReleased(int key);
    ofImage menuHUD;
    bool paused;
    int hudY;
    int hudH;
    int hudHeight;
    int changeState;

    shared_ptr<Tektike> createTektike();
    shared_ptr<Item>createSword();
    shared_ptr<RedDarkNut> createRedDarkNut();
    shared_ptr<MagicTek> createMagicTek();
};