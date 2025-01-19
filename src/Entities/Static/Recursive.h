#pragma once
#include "StaticEntity.h"
#include <memory>

enum class MoveDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class RecursiveMover : public StaticEntity {
public:
    RecursiveMover(int x, int y, std::shared_ptr<Handler> handler, MoveDirection direction)
        : StaticEntity(x, y, handler), direction(direction) {}

    ~RecursiveMover() {}

    void onCollision(std::shared_ptr<DynamicEntity> entity) {
        move(entity, 3);
    }

private:
    MoveDirection direction;

    void move(std::shared_ptr<DynamicEntity> entity, int steps) {
        if (steps <= 0) return;

        switch (direction) {
        case MoveDirection::UP:
            entity->setY(entity->getY() - Handler::TILE_HEIGHT);
            break;
        case MoveDirection::DOWN:
            entity->setY(entity->getY() + Handler::TILE_HEIGHT);
            break;
        case MoveDirection::LEFT:
            entity->setX(entity->getX() - Handler::TILE_WIDTH);
            break;
        case MoveDirection::RIGHT:
            entity->setX(entity->getX() + Handler::TILE_WIDTH);
            break;
        }

        move(entity, steps - 1);
    }
};
