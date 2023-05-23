#ifndef _Asteroids_CPP
#define _Asteroids_CPP

#include "functions.h"
#include "includes.h"
//
//generating asteroid with random texture
//
void asteroid_spawn(float radius)
{
    const std::string numb="123456";
    int spawn_collision = 0;

    srand(rand());
    int a = rand();
    srand(time(NULL)*a);


    std::string name="Asteroids/Asteroid_";
    name+=numb[rand()%6];
    name+=".png";

    sf::CircleShape asteroid(radius);
    load_texture(asteroid,name);

    while(spawn_collision!=1)
        {
            asteroid.setPosition(rand()%H,rand()%W);

            if ((asteroid.getPosition().x>=3./4.*H||asteroid.getPosition().x<=1./4.*H)&&(asteroid.getPosition().y>=3./4.*W||asteroid.getPosition().y<=1./4.*W))
                spawn_collision = 1;
        }


    moving temp_aster;
    if (asteroid.getPosition().x<=H/2)
        temp_aster.vx=1+rand()%3;
    else
        temp_aster.vx=-1-rand()%3;

    if  (asteroid.getPosition().y<=W/2)
        temp_aster.vy=1+rand()%3;
    else
        temp_aster.vy=-1-rand()%3;

    temp_aster.bullet = asteroid;
    asteroids.push_back(temp_aster);
    std::cout<<"spawned and asteroid\n";
}
//
//player-asteroid impact check
//
bool asteroid_impact(sf::CircleShape &shape, sf::CircleShape &asteroid)
{

    float x = shape.getPosition().x - asteroid.getPosition().x - asteroid.getRadius();
    float y = shape.getPosition().y - asteroid.getPosition().y - asteroid.getRadius();

    if  (sqrt(x*x+y*y)<asteroid.getRadius())
        return true;
    else
        return false;
}
//
//bullet-asteroid impact check
//
bool bullet_impact(moving &bullet, moving &asteroid)
{

    float x = bullet.bullet.getPosition().x - asteroid.bullet.getPosition().x - asteroid.bullet.getRadius();
    float y = bullet.bullet.getPosition().y - asteroid.bullet.getPosition().y - asteroid.bullet.getRadius();

    if  (sqrt(x*x+y*y)<asteroid.bullet.getRadius())
        return true;
    else
        return false;
}
//
//function splitting large asteroids
//
void asteroid_split(moving &asteroid)
{
    moving aster_temp1;
    moving aster_temp2;

    aster_temp1.bullet=asteroid.bullet;
    aster_temp2.bullet=asteroid.bullet;

    aster_temp1.bullet.setRadius(asteroid.bullet.getRadius()/2.);
    aster_temp2.bullet.setRadius(asteroid.bullet.getRadius()/2.);

    aster_temp1.bullet.setPosition(asteroid.bullet.getPosition().x+asteroid.bullet.getRadius(),asteroid.bullet.getPosition().y+asteroid.bullet.getRadius());
    aster_temp2.bullet.setPosition(asteroid.bullet.getPosition().x+asteroid.bullet.getRadius(),asteroid.bullet.getPosition().y+asteroid.bullet.getRadius());

    aster_temp1.vx=asteroid.vx;
    aster_temp1.vy=asteroid.vy;
    aster_temp2.vx=-asteroid.vx;
    aster_temp2.vy=-asteroid.vy;

    asteroids.push_back(aster_temp1);
    asteroids.push_back(aster_temp2);


}

#endif // _Asteroids_CPP
