#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "entity.h"

class Bullet : public Entity{
public:
	Bullet(sf::Image &image, float X, float Y, int W, int H, std::string Name, float Speed, Direction Dir);

    void control(float time);

	void update(float time);

	void interactionWithMap();
};

#endif // BULLET_H_INCLUDED
