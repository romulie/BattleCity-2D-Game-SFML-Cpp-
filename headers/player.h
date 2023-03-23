#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "entity.h"
#include "gamemap.h"

class Player : public Entity {
public:
    int score;
    bool isShoot;

    Player(sf::Image& image, float X, float Y, int W, int H, std::string Name, float Speed = 0, Direction Dir = Direction::up);

    int getPlayerScore();

    void interactionWithMap();

    void control(float time);

    void update(float time);
};

#endif // PLAYER_H_INCLUDED
