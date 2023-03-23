#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "entity.h"

class Enemy : public Entity{

public:
    float changeDirTimer;

	Enemy(sf::Image &image, float X, float Y, int W, int H, std::string Name, float Speed = 0, Direction Dir = Direction::down);

    void interactionWithMap();

    void control(float time);

	void update(float time);
};

#endif // ENEMY_H_INCLUDED
