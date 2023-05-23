#ifndef _Functions_H
#define _Functions_H

#include "includes.h"




//Miscellenous functions
void load_texture(sf::Shape &shape, std::string name);
void asteroids_setup();
void controled_move(sf::CircleShape &shape, sf::RenderWindow &window,sf::Sound &sound);
void shoot (sf::CircleShape &shape, std::vector<moving> &shots,sf::Sound &sound);
void countdown(sf::RenderWindow &window, sf::Clock &clock, sf::Font &font, sf::Sound &sound1, sf::Sound &sound2);
void PointCounter(int points, sf::Font &font, sf::RenderWindow &window);
sf::Text time(sf::Clock &clock, sf::Font &font, float ptime, sf::RenderWindow &window);

//Asteroids functions
bool asteroid_impact(sf::CircleShape &shape, sf::CircleShape &asteroid);
void asteroid_spawn(float radius, std::vector<moving> &asteroids);
bool bullet_impact(moving &bullet, moving &asteroid);
void asteroid_split(moving &asteroid, std::vector<moving> &asteroids,moving &shot);

//Gamestates
int events(sf::RenderWindow &window, sf::Clock &clock, sf::Clock &clock2, sf::Clock &pclock, std::string &Gamestate, int ps_button);
void mainMenu(sf::RenderWindow &window, std::string &Gamestate, int &game,sf::Clock &clock);
void lossMenu(sf::RenderWindow &window, std::string &Gamestate, int &game,sf::Clock &clock, sf::Font &font, int points,float time);
void winMenu(sf::RenderWindow &window, std::string &Gamestate, int &game,sf::Clock &clock, sf::Font &font, int points,float time);
void pauseMenu(sf::RenderWindow &window, std::string &Gamestate, sf::Clock &clock2);
void HowTo (sf::RenderWindow &window, std::string &Gamestate,sf::Font &font,
            sf::CircleShape &player,sf::Clock &clock2, std::vector<moving> &shots, std::vector<moving> &asteroids,
            sf::Sound &sound,sf::Sound &move_sound,sf::Sound &destroy_sound);

#endif // _Functions_H
