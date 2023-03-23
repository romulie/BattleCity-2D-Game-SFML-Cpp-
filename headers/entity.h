#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "gamemap.h"
#include "direction.h"

extern const int SPRITE_W;
extern const int SPRITE_H;
extern const int HEIGHT_MAP;
extern const int WIDTH_MAP;

class Entity{

public:
    float x, y, dx, dy, speed, moveTimer;   /// coordinates x/y, accelerations dx/dy, speed, timer for animation
    int w, h, power;                       ///dimensions w/h, power
    static std::vector<std::string> tileMap;/// map of the game
    static bool gameOver;
    bool life;
    std::string name;
    float spriteX, spriteY;                 /// basic coordinates for a sprite of an entity
    float currentFrame;                     /// for animation
    Direction dir;
    sf::Texture texture;
    sf::Sprite sprite;

    Entity(sf::Image &image, float X, float Y, int W, int H, std::string Name, float Speed = 0, Direction Dir = Direction::up);

    virtual ~Entity() {};//= default;

    sf::FloatRect getRect();

    float getPlayerX();

    float getPlayerY();

    void generatePickUp(char c);

    static void drawMap(sf::RenderWindow &window, sf::Sprite &mapSprite, const std::string &mask = "");

    virtual void interactionWithMap() = 0;

    virtual void update(float time) = 0;
};


#endif // ENTITY_H_INCLUDED
