#include "../headers/bullet.h"

    Bullet::Bullet(sf::Image &image, float X, float Y, int W, int H, std::string Name, float Speed, Direction Dir)
	: Entity(image, X, Y, W, H, Name, Speed, Dir){
	    if (name == "Bullet1" || name == "Bullet2") {
            spriteX = 263; spriteY = 102;
            x += w/2-1; y += h/2;
            w = 2; h = 2;
        }
	    sprite.setTextureRect(sf::IntRect(spriteX, spriteY, w, h));
	}

    void Bullet::control(float time){
        if      (dir == Direction::up)      { dy = -speed;   dx = 0; }
        else if (dir == Direction::down)    { dy = speed;    dx = 0; }
        else if (dir == Direction::left)    { dx = -speed;   dy = 0; }
        else if (dir == Direction::right)   { dx = speed;    dy = 0; }
    }

	void Bullet::update(float time){
        control(time);
        x += dx * time;
        y += dy * time;
        interactionWithMap();
        sprite.setPosition(x, y);
	}

	void Bullet::interactionWithMap(){
        /// limit the movement to the borders of the map
        if (x < 0 || y < 0 || x > WIDTH_MAP*SPRITE_W-SPRITE_W/2
            || y > HEIGHT_MAP*SPRITE_H-SPRITE_H/2) {life = false; return; }
                /// collisions with map objects
                int i = y/SPRITE_H, j = x/SPRITE_W;

                if (tileMap[i][j] == '#'){
                /// concrete objects destroy bullets
                    if (power == 0){
                        if      (dy > 0) {y = i*SPRITE_H-SPRITE_H; life = false; return;}
                        else if (dy < 0) {y = i*SPRITE_H+SPRITE_H; life = false; return;}
                        else if (dx > 0) {x = j*SPRITE_W-SPRITE_W; life = false; return;}
                        else if (dx < 0) {x = j*SPRITE_W+SPRITE_W; life = false; return;}
                    }
                /// or powerful bullets destroy concrete
                    if (power > 0){
                        if      (dy > 0) {y = i*SPRITE_H-SPRITE_H; life = false; tileMap[i][j] = '0'; return;}
                        else if (dy < 0) {y = i*SPRITE_H+SPRITE_H; life = false; tileMap[i][j] = '0'; return;}
                        else if (dx > 0) {x = j*SPRITE_W-SPRITE_W; life = false; tileMap[i][j] = '0'; return;}
                        else if (dx < 0) {x = j*SPRITE_W+SPRITE_W; life = false; tileMap[i][j] = '0'; return;}
                    }
                }
                /// bullet can destroy half-a-brick:
                if (tileMap[i][j] == '*'){
                    if      (dy > 0) {y = i*SPRITE_H-SPRITE_H; life = false; tileMap[i][j] = 'U'; return;} // half-a-brick hit from top
                    else if (dy < 0) {y = i*SPRITE_H+SPRITE_H; life = false; tileMap[i][j] = 'D'; return;} // half-a-brick hit from bottom
                    else if (dx > 0) {x = j*SPRITE_W-SPRITE_W; life = false; tileMap[i][j] = 'L'; return;} // half-a-brick hit from left
                    else if (dx < 0) {x = j*SPRITE_W+SPRITE_W; life = false; tileMap[i][j] = 'R'; return;} // half-a-brick hit from right
                }/// and destroy half-a-brick completely:
                if (tileMap[i][j] == 'U' || tileMap[i][j] == 'D' || tileMap[i][j] == 'L' || tileMap[i][j] == 'R'){
                    if      (dy > 0) {y = i*SPRITE_H-SPRITE_H; life = false; tileMap[i][j] = '0'; return;}
                    else if (dy < 0) {y = i*SPRITE_H+SPRITE_H; life = false; tileMap[i][j] = '0'; return;}
                    else if (dx > 0) {x = j*SPRITE_W-SPRITE_W; life = false; tileMap[i][j] = '0'; return;}
                    else if (dx < 0) {x = j*SPRITE_W+SPRITE_W; life = false; tileMap[i][j] = '0'; return;}
                }/// when headquarters are hit the game is over
                if (tileMap[i][j] == '@') {life = false; gameOver = true; return;}
    }
