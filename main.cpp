#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <list>
#include "windows.h"
#include "headers/direction.h"
#include "headers/gamemap.h"
#include "headers/entity.h"
#include "headers/player.h"
#include "headers/enemy.h"
#include "headers/bullet.h"
#include "headers/view.h"

void start_game();
void game_over();
void deleteDeadEntities(std::list<Entity*> &entities, const float time);
int playersInteractWithBullets(std::list<Entity*> &entities, Player &myTank, Player &myTank2);
void respawnEnemies(std::list<Entity*> &entities, sf::Image &allImages);
int enemiesShoot(std::list<Entity*> &entities, sf::Image &allImages, int bulletsCnt);
void playerShoot(std::list<Entity*> &entities, sf::Image &allImages, Player &myTank);

const int WINDOW_SIZE = 600;
const std::string windowGameName            {"Battle City for LestaGames 2023"};
const std::string introPictFileName         {"intro.png"};
const std::string spritesFileName           {"generalSprites.png"};
const std::string fontFileName              {"arial.ttf"};
const std::string startThemeMusicFileName   {"start_theme.ogg"};
const std::string shootSoundFileName        {"shoot.ogg"};
const std::string overThemeMusicFileName    {"over_theme.ogg"};

///load level map and Entity::gameOver (static member should be initialized outside the class)
std::vector<std::string> Entity::tileMap = level_maps[1];
bool Entity::gameOver = false;

int main(){
  srand(time(NULL)) ;

  sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), windowGameName);
  view.reset(sf::FloatRect(0,0,WIDTH_MAP*SPRITE_W,HEIGHT_MAP*SPRITE_H+25));
  /// Shooting sound
  sf::SoundBuffer shootBuf;
  bool isLoadOK = shootBuf.loadFromFile(shootSoundFileName);
  if(!isLoadOK){std::cout<<"Unable to load media/" + shootSoundFileName<<std::endl;}
  sf::Sound shoot; shoot.setBuffer(shootBuf);
  /// Start screen
  start_game();
  /// Sprites
  sf::Image allImages;
  isLoadOK = allImages.loadFromFile(spritesFileName);
  if(!isLoadOK){std::cout<<"Unable to load media/" + spritesFileName<<std::endl;}
  sf::Texture mapTexture; mapTexture.loadFromImage(allImages);
  sf::Sprite mapSprite; mapSprite.setTexture(mapTexture);
  /// Text
  sf::Font font;
  isLoadOK = font.loadFromFile(fontFileName);
  if(!isLoadOK){std::cout<<"Unable to load media/" + fontFileName<<std::endl;}
  sf::Text text("", font, 10); text.setFillColor(sf::Color::Green);//setColor
  /// Create 3 enemies and 2 players
  std::list<Entity*>  entities;
  entities.push_back(new Enemy(allImages, 0, 0, SPRITE_W, SPRITE_H, "Enemy0", 0.1, Direction::down));
  entities.push_back(new Enemy(allImages, 200, 0, SPRITE_W, SPRITE_H, "Enemy1", 0.07, Direction::down));
  entities.push_back(new Enemy(allImages, 400, 0, SPRITE_W, SPRITE_H, "Enemy2", 0.2, Direction::down));
  Player myTank(allImages, SPRITE_W*10, 400, SPRITE_W, SPRITE_H, "Player1", 0.1, Direction::up);
  Player myTank2(allImages, SPRITE_W*14, 400, SPRITE_W, SPRITE_H, "Player2", 0.1, Direction::up);/// mask the line for 1-player mode
  /// Game clock
  sf::Clock clock, gameTimeClock;
  int gameTime = 0;
  static int bulletsCnt = 0;

  while(window.isOpen()){
    if (Entity::gameOver) break;
    /// count player's time while he is alive
    float time = clock.getElapsedTime().asMicroseconds();
    if (myTank.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();
    else break;  /// proceed to game_over();
    clock.restart();
    time /= 2000;

    sf::Event event;
    while(window.pollEvent(event)){
        if(event.type == sf::Event::Closed){ window.close(); }/// Close window button clicked.
    }

    /// Players shoot
    if (myTank.life && myTank.isShoot) {
        myTank.isShoot = false;
        if (bulletsCnt < 1){
            shoot.play();
            playerShoot(entities, allImages, myTank);
            playerShoot(entities, allImages, myTank2);
        }
    }
    /// Enemies shoot
    if (entities.size() < 5)
        bulletsCnt = enemiesShoot(entities, allImages, bulletsCnt);
    /// update entities (Enemies and Bullets) and erase dead
    deleteDeadEntities(entities, time);
    /// Interaction with bullets
    bulletsCnt = playersInteractWithBullets(entities, myTank, myTank2);
    /// Respawn new enemy with random position, speed and appearance
    if (entities.size() < 3)
        respawnEnemies(entities, allImages);
    /// Update Players
    myTank.update(time);
    myTank2.update(time);
    /// camera control (zoom: Z/X, rotation R)
    changeView(); window.setView(view);
    window.clear(sf::Color(0,0,0,255));
    /// Draw the map with NO BUSHES!
    Entity::drawMap(window, mapSprite, "G");
    /// Draw Text (Time and Score)
    std::string infoStr = "SCORE: "+std::to_string(myTank.getPlayerScore())+"/"+ std::to_string(myTank2.getPlayerScore())+
    "\nTIME: "+std::to_string(gameTime);
    text.setString(infoStr); text.setPosition(160, 377); window.draw(text);
    /// Draw entities (Enemies and Bullets)
    for (auto it = std::begin(entities); it != std::end(entities); ++it)
        { window.draw((*it)->sprite);}
    /// Draw Players
    window.draw(myTank.sprite); window.draw(myTank2.sprite);/// mask the line for 1-player mode
    /// Draw the the BUSHES on the map
    Entity::drawMap(window, mapSprite, "*@URDUSBW#");
    /// Display
    window.display();
  }

  game_over();

  return 0;
}
///=======================================================
void start_game(){
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), windowGameName);
    window.clear(sf::Color(0,0,0,255));
    sf::Image introPict;
    bool isLoadOK = introPict.loadFromFile(introPictFileName);
    if(!isLoadOK){std::cout<<"Unable to load media/" + introPictFileName<<std::endl;}
    sf::Texture introTexture; introTexture.loadFromImage(introPict);
    sf::Sprite introSprite; introSprite.setTexture(introTexture);
    introSprite.setPosition(200,150);
    window.draw(introSprite);

    sf::Font font; font.loadFromFile(fontFileName);
    sf::Text text("", font, 20); text.setFillColor(sf::Color::Green);
    text.setString("Use keyboard arrows to control pair of tanks.\n"
                   "     Use space to shoot.  Good luck!\n");
    text.setPosition(120, 360);

    window.draw(text);
    window.draw(introSprite);
    window.display();

    sf::Music music; if (!music.openFromFile(startThemeMusicFileName))
        std::cout<<"Cannot open " + startThemeMusicFileName << "\n";
    music.play();
    Sleep(4500);
    music.stop();
};
///=======================================================
void game_over(){
    sf::RenderWindow window(sf::VideoMode(300, 100), windowGameName);
    sf::Font font; font.loadFromFile(fontFileName);
    sf::Text text("", font, 20); text.setFillColor(sf::Color::Red);
    //std::string infoStr = ;
    text.setString("GAME OVER!\n"); text.setPosition(70, 35);
    sf::Music music; if (!music.openFromFile(overThemeMusicFileName))
        std::cout<<"Cannot open " + overThemeMusicFileName << "\n";
    music.play();

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                music.stop();
                window.close(); }/// Close window button clicked.
        }
        window.clear(sf::Color(0,0,0,255));
        window.draw(text);
        window.display();
    }
};
///=======================================================
void deleteDeadEntities(std::list<Entity*> &entities, const float time){
    /// update entities (Enemies and Bullets) and erase dead
    for (auto it = entities.begin(); it != entities.end(); ){
        Entity* tmp = *it;
        tmp -> update(time);
        if (!(tmp -> life)) { it = entities.erase(it); delete tmp; }
        else ++it;
    }
  };
///=======================================================
int playersInteractWithBullets(std::list<Entity*> &entities, Player &myTank, Player &myTank2){

    int bulletsCnt = 0;
    for (auto it = entities.begin(); it != entities.end(); it++){
        /// player's bullets kill enemies
        if ((*it)->name == "Bullet1"){
            ++bulletsCnt;
            for (auto it2 = entities.begin(); it2 != entities.end(); it2++){
                if (it == it2 || (*it2)->name == "Bullet1") continue;
                else if ((*it)->getRect().intersects((*it2)->getRect())){
                    (*it2)->life = false;
                    (*it)->life = false;
                    myTank.score += 100;
                    myTank2.score += 100;
                }
            }
        }
        /// enemy's bullets kill myTank and myTank2
        if ((*it)->name == "Bullet2"){
            if ((*it)->getRect().intersects(myTank.getRect()))  myTank.score -= 10;
            if ((*it)->getRect().intersects(myTank2.getRect())) myTank2.score -= 10;
        }
    }
    return bulletsCnt;
};
///=======================================================
void respawnEnemies(std::list<Entity*> &entities, sf::Image &allImages){
    entities.push_back(new Enemy(allImages, rand()%(WIDTH_MAP*SPRITE_W), 0, SPRITE_W, SPRITE_H,
                                     "Enemy"+std::to_string(rand()%3),
                                     0.1+0.1*(rand()%3), Direction::down));
};
///=======================================================
int enemiesShoot(std::list<Entity*> &entities, sf::Image &allImages, int bulletsCnt){
    for (auto it = entities.begin(); it != entities.end(); it++){
        if ((*it)->name[0] != 'B'){
            ++bulletsCnt;
            entities.push_back(new Bullet(allImages, (*it)->getPlayerX(), (*it)->getPlayerY(), SPRITE_W, SPRITE_H, "Bullet2", 0.3, (*it)->dir));
            if ((*it)->power > 0) entities.back()->power = 100; /// generate concrete-destroying bullet
            if (entities.size() > 10) break;
        }
    }
    return bulletsCnt;
};
///=======================================================
void playerShoot(std::list<Entity*> &entities, sf::Image &allImages, Player &myTank){
        entities.push_back(new Bullet(allImages, myTank.getPlayerX(), myTank.getPlayerY(), SPRITE_W, SPRITE_H, "Bullet1", 0.3, myTank.dir));
        if (myTank.power > 0) entities.back()->power = 100; /// generate concrete-destroying bullet
};
///=======================================================

