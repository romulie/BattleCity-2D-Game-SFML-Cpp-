#include "../headers/entity.h"


    Entity::Entity(sf::Image &image, float X, float Y, int W, int H, std::string Name,
            float Speed, Direction Dir) {
        x = X; y = Y;
        w = W; h = H;
        dx = 0; dy = 0;
        dir = Dir;
        speed = Speed;
        name = Name;
        moveTimer = 0;
        power = 0; /// used for concrete destruction
        life = true;
        currentFrame = 0;
        spriteX = 0; spriteY = 0;                        /// size of the characters's sprite
        //image.createMaskFromColor(sf::Color(0, 0, 0)); /// mask for background
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(spriteX, spriteY, w, h));
    };

    void Entity::drawMap(sf::RenderWindow &window, sf::Sprite &mapSprite, const std::string &mask){
      for (int i = 0; i < HEIGHT_MAP; ++i){
        for (int j = 0; j < WIDTH_MAP; ++j){
            if (mask.find(Entity::tileMap[i][j]) != std::string::npos) continue; // do not draw masked items (e.g. bushes)
            int offsetX = 0, offsetY = 0;
            ///!?! create map with textures (map<char, sf::IntRect>)
            /// full half-a-bricks
            if      (Entity::tileMap[i][j] == '*') mapSprite.setTextureRect(sf::IntRect(256, 0, SPRITE_W, SPRITE_H)); // Bricks
            else if (Entity::tileMap[i][j] == 'U') {mapSprite.setTextureRect(sf::IntRect(256, 0, SPRITE_W, SPRITE_H/2)); offsetY = SPRITE_H/2 + 1;}
            else if (Entity::tileMap[i][j] == 'R') {mapSprite.setTextureRect(sf::IntRect(256, 0, SPRITE_W/2, SPRITE_H));}
            else if (Entity::tileMap[i][j] == 'D') {mapSprite.setTextureRect(sf::IntRect(256, 0, SPRITE_W, SPRITE_H/2));}
            else if (Entity::tileMap[i][j] == 'L') {mapSprite.setTextureRect(sf::IntRect(256, 0, SPRITE_W/2, SPRITE_H)); offsetX = SPRITE_W/2 + 1;} // half-a-brick{mapSprite.setTextureRect(sf::IntRect(320, 0, -SPRITE_W, SPRITE_H));}
            /// full and half-a-concrete-blocks
            else if (Entity::tileMap[i][j] == '#') mapSprite.setTextureRect(sf::IntRect(256, 16, SPRITE_W, SPRITE_H)); // Concrete
            else if (Entity::tileMap[i][j] == 'u') {mapSprite.setTextureRect(sf::IntRect(256, 16, SPRITE_W, SPRITE_H/2)); offsetY = SPRITE_H/2 + 1;}
            else if (Entity::tileMap[i][j] == 'r') {mapSprite.setTextureRect(sf::IntRect(256, 16, SPRITE_W/2, SPRITE_H));}
            else if (Entity::tileMap[i][j] == 'd') {mapSprite.setTextureRect(sf::IntRect(256, 16, SPRITE_W, SPRITE_H/2));}
            else if (Entity::tileMap[i][j] == 'l') {mapSprite.setTextureRect(sf::IntRect(256, 16, SPRITE_W/2, SPRITE_H)); offsetX = SPRITE_W/2 + 1;}
            /// other items
            else if (Entity::tileMap[i][j] == 'G') mapSprite.setTextureRect(sf::IntRect(272, 32, SPRITE_W, SPRITE_H)); // Green Bushes
            else if (Entity::tileMap[i][j] == 'W') mapSprite.setTextureRect(sf::IntRect(256, 32, SPRITE_W, SPRITE_H)); // Water
            else if (Entity::tileMap[i][j] == 'S') mapSprite.setTextureRect(sf::IntRect(304, 112, SPRITE_W, SPRITE_H)); // Star
            else if (Entity::tileMap[i][j] == 'B') mapSprite.setTextureRect(sf::IntRect(272, 112, SPRITE_W, SPRITE_H)); // TimeBomb
            else if (Entity::tileMap[i][j] == 'P') mapSprite.setTextureRect(sf::IntRect(352, 112, SPRITE_W, SPRITE_H)); // Gun (bullet power gain)
            else if (Entity::tileMap[i][j] == '@'){
                if (!Entity::gameOver) mapSprite.setTextureRect(sf::IntRect(304, 32, SPRITE_W, SPRITE_H)); // HeadQuarters
                else mapSprite.setTextureRect(sf::IntRect(320, 32, SPRITE_W, SPRITE_H)); // Ruined HeadQuarters
            }
            else continue; /// don't draw
            //else mapSprite.setTextureRect(sf::IntRect(336, 0, SPRITE_W, SPRITE_H)); // dtaw black/EMPTY
            mapSprite.setPosition(j*SPRITE_W+offsetX, i*SPRITE_H+offsetY);
            window.draw(mapSprite);
        }
      }
    }

    void Entity::generatePickUp(char c){
        size_t xPos = rand() % WIDTH_MAP, yPos = rand() % HEIGHT_MAP;
        while (tileMap[yPos][xPos] != '0'){ xPos = rand() % WIDTH_MAP; yPos = rand() % HEIGHT_MAP; }
        tileMap[yPos][xPos] = c;
    };

    sf::FloatRect Entity::getRect() { return sf::FloatRect(x, y, w, h); }

    float Entity::getPlayerX()      { return x;}

    float Entity::getPlayerY()      { return y;}
