#pragma once
#include "Item.h"
#include "StaticEntity.h"
#include <memory>
#include "Link.h"


class Sword : public Item {
public:
    Sword(int x, int y, std::shared_ptr<Handler> handler);
    bool isSword() { return false; } 
    void applyEffect(shared_ptr<Link> link) override;
    void draw();
    ~Sword(){}

    protected:
    ofImage sprite;

};