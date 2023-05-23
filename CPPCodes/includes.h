#ifndef _Includes_H
#define _Includes_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <sstream>

//win_cond lat you control how much points you need to achieve to win. Assign desired value.
const int win_cond = 50;
//Height and Width of the game window:
const int H=1600;
const int W=900;



static int game=1;
static int p=0,t=0,rotl=0,rotp=0,shift=1;
static sf::CircleShape player(100.f);
static std::string Gamestate = "Menu";

static std::vector<std::string> textures_files;
static std::vector<sf::Texture> textures_text;

class moving
{
public:
    float vx;
    float vy;
    sf::CircleShape bullet;

};
static std::vector<moving> asteroids;
static std::vector<moving> shots;

#endif // _Includes_H
