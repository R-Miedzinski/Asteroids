#ifndef _Functions_CPP
#define _Functions_CPP

//miscellaneous game functions

#include "functions.h"
#include "includes.h"


/*MISCELLENOUS FUNCTIONS*/

//
//loading texture for a shape
//
void load_texture(sf::Shape &shape, std::string name)
{

    int counter=0,index;
    static std::vector<sf::CircleShape> dummies;
    //std::cout<<"rozpoczynam wczytywanie \n";
    sf::Texture text;
    //std::cout<<"sprawdzam czy byla wczytana textura z pliku: "<<name<<"\n";

    //checking if the texture was used before
    for(int i=0;i<textures_files.size();i++)
    {

        if(textures_files[i]!=name)
            counter++;
        else if(textures_files[i]==name)
            index=i;
    }
    if(counter==textures_files.size())//loading new textures
        {

            //std::cout<<"nie uzywano textury z pliku: "<<name<<". Dodaj do listy plikow\n";
            //asteroids_setup(name);
            textures_files.push_back(name);
            //std::cout<<"wczytywany plik: "<<name<<"\n";
            if (!text.loadFromFile(name))
                {std::cout<<"blad";exit(0);}
            //std::cout<<"wczytano texture z pliku \n";
            //std::cout<<"dodaje do listy textur na pozycji"<<textures_text.size()<<"\n";
            textures_text.push_back(text);
            int texture = textures_text.size()-1;
            //std::cout<<"usatwiam texture z pozycji "<<texture<<"\n";
            shape.setTexture(&textures_text.at(texture));
            //load_texture(shape,name);
            //std::cout<< "wczytano\n";
        }
        else if (counter < textures_files.size())//reloading used textures
        {
            //std::cout<<"textura byla juz uzywana i posiada index "<<index<<"\n";
            shape.setTexture(&textures_text[index]);
        }
}
//
//dummies for asteroid textures
//
void asteroids_setup()
{
    static sf::CircleShape dummy[19];
    for(int i=0;i<6;i++)
    {
        const std::string numb="123456";
        std::string name="Asteroids/Asteroid_";
        name+=numb[i];
        name+=".png";
        load_texture(dummy[i],name);
    }
        load_texture(dummy[6],"Bullet_1.png");
        load_texture(dummy[7],"Spaceship.png");
        load_texture(dummy[8],"Space_Background_1.png");
        load_texture(dummy[9],"Space_Game_GUI_PNG/PNG/Main_UI/Pause_BTN.png");
        load_texture(dummy[10],"Space_Game_GUI_PNG/PNG/Main_Menu/Start_BTN.png");
        load_texture(dummy[11],"Space_Game_GUI_PNG/PNG/Main_Menu/Exit_BTN.png");
        load_texture(dummy[12],"Space_Game_GUI_PNG/PNG/Main_Menu/Header.png");
        load_texture(dummy[13],"Space_Game_GUI_PNG/PNG/Pause/Menu_BTN.png");
        load_texture(dummy[14],"Space_Game_GUI_PNG/PNG/You_Lose/Header.png");
        load_texture(dummy[15],"Space_Game_GUI_PNG/PNG/You_Win/Header.png");
        load_texture(dummy[16],"Space_Game_GUI_PNG/PNG/Level_Menu/Play_BTN.png");
        load_texture(dummy[17],"Space_Game_GUI_PNG/PNG/Pause/Header.png");
        load_texture(dummy[18],"Space_Game_GUI_PNG/PNG/Main_Menu/Info_BTN.png");
    std::cout<<"textures are set up\n";

}
//
//player movement function
//
void controled_move(sf::CircleShape &shape,sf::RenderWindow &window,sf::Sound &sound)
{

    int pop = 20;
    float x = shape.getPosition().x;
    float y = shape.getPosition().y;
    sf::Vector2f pos;
    pos=shape.getPosition();
    float rotation=shape.getRotation();


    //rotation
    shape.setPosition(0,0);
    shape.rotate(-rotl+rotp);
    shape.setPosition(x,y);



    //directional movement
    //std::cout<<p<<"\n";
    float xdir = (-t*sin(shape.getRotation()*3.14159/180)+p*sin(shape.getRotation()*3.14159/180))*shift;
    float ydir = (t*cos(shape.getRotation()*3.14159/180)-p*cos(shape.getRotation()*3.14159/180))*shift;
    shape.move(xdir,ydir);

    //window border colision
    if(shape.getPosition().x<-pop) {shape.setPosition(H+pop,shape.getPosition().y);}
    if(shape.getPosition().x>H+pop) {shape.setPosition(-pop,shape.getPosition().y);}
    if(shape.getPosition().y<-pop) {shape.setPosition(shape.getPosition().x,W+pop);}
    if(shape.getPosition().y>W+pop) {shape.setPosition(shape.getPosition().x,-pop);}



    sound.setPitch(shift);

    if ((shape.getRotation()!=rotation||shape.getPosition()!=pos))
        {sound.pause();sound.play();sound.setLoop(1);}
    else if (shape.getRotation()==rotation&&shape.getPosition()==pos)
        sound.setLoop(false);



        window.draw(shape);
}
//
//Shooting
//
void shoot (sf::CircleShape &shape, std::vector<moving> &shots,sf::Sound &sound)
{

    float xdir = sin(shape.getRotation()*3.14159/180);
    float ydir = -cos(shape.getRotation()*3.14159/180);
    sf::CircleShape shot(10.f);
    shot.setPosition(shape.getPosition());
    load_texture(shot,"Bullet_1.png");
    moving pif;
    pif.vx=xdir;
    pif.vy=ydir;
    pif.bullet=shot;
    shots.push_back(pif);

    sound.play();
}
//
//Countdown before game starts
//
void countdown(sf::RenderWindow &window, sf::Clock &clock, sf::Font &font, sf::Sound &sound1, sf::Sound &sound2)
{
    sf::Text text;
    static int tt;
    static int counter(0);

    text.setCharacterSize(400);
    text.setPosition(H/2-text.getCharacterSize()/4,W/2-text.getCharacterSize()/2);
    text.setColor(sf::Color::White);
    text.setFont(font);

    tt=3-int(clock.getElapsedTime().asSeconds());

    std::string txt;
    std::string str;
    std::stringstream ss;
    ss << tt;
    ss >> str;
    txt += str;

    text.setString(txt);

    window.draw(text);

    if((tt==3&&counter==0)||(tt==2&&counter==1)||(tt==1&&counter==2))
        {sound1.play();counter++;}
    if(tt==0&&counter>=3)
        {sound2.play();counter=0;}
}
//
//display current score
//
void PointCounter(int points, sf::Font &font, sf::RenderWindow &window)
{
    sf::Text text;
    sf::RectangleShape background;

    text.setPosition(0,0);
    text.setCharacterSize(20);

    text.setFont(font);

    std::string txt = "Score: ";
    std::string str;
    std::stringstream ss;
    ss << points;
    ss >> str;
    txt += str;

    text.setString(txt);

    background.setFillColor(sf::Color::Black);
    background.setPosition(0,0);
    background.setSize(sf::Vector2f(100,30));

    window.draw(background);
    window.draw(text);
}
//
//display current play time
//
sf::Text time(sf::Clock &clock, sf::Font &font, float ptime, sf::RenderWindow &window)
{
    sf::Text text;
    sf::RectangleShape background;
    static int tt;

    text.setPosition(0,30);
    text.setCharacterSize(20);
    text.setFont(font);

    tt=int(clock.getElapsedTime().asSeconds()*100-ptime);

    std::string txt = "Time: ";
    std::string str;
    std::stringstream ss;
    ss << tt/100.;
    ss >> str;
    txt += str;
    txt += "s";

    text.setString(txt);

    background.setFillColor(sf::Color::Black);
    background.setPosition(0,30);
    background.setSize(sf::Vector2f(100,30));

    window.draw(background);
    window.draw(text);
    return text;
}

/*ASTEROIDS FUNCTIONS*/

//
//generating asteroid with random texture
//
void asteroid_spawn(float radius, std::vector<moving> &asteroids)
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
    //std::cout<<"spawned and asteroid\n";
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
void asteroid_split(moving &asteroid, std::vector<moving> &asteroids, moving &shot)
{
    moving aster_temp1;
    moving aster_temp2;

    aster_temp1.bullet=asteroid.bullet;
    aster_temp2.bullet=asteroid.bullet;

    aster_temp1.bullet.setRadius(asteroid.bullet.getRadius()/2.);
    aster_temp2.bullet.setRadius(asteroid.bullet.getRadius()/2.);

    aster_temp1.bullet.setPosition(asteroid.bullet.getPosition().x+asteroid.bullet.getRadius(),asteroid.bullet.getPosition().y+asteroid.bullet.getRadius());
    aster_temp2.bullet.setPosition(asteroid.bullet.getPosition().x+asteroid.bullet.getRadius(),asteroid.bullet.getPosition().y+asteroid.bullet.getRadius());

    aster_temp1.vx=shot.vx+asteroid.vx;
    aster_temp1.vy=shot.vy+asteroid.vy;
    aster_temp2.vx=shot.vx-asteroid.vx;
    aster_temp2.vy=shot.vy-asteroid.vy;

    asteroids.push_back(aster_temp1);
    asteroids.push_back(aster_temp2);


}

/*GAMESTATES FUNCTIONS*/

//
//Reading window events
//
int events(sf::RenderWindow &window, sf::Clock &clock, sf::Clock &clock2, sf::Clock &pclock, std::string &Gamestate, int ps_button)
{
    //std::cout<<"Hello\n";
    sf::Event event;
    sf::Mouse mouse;
    sf::Vector2i pos=mouse.getPosition(window);

    sf::RectangleShape PauseButton;
    PauseButton.setSize(sf::Vector2f(50.f,50.f));
    PauseButton.setPosition(H-50.f,0);
    load_texture(PauseButton,"Space_Game_GUI_PNG/PNG/Main_UI/Pause_BTN.png");

    sf::Vector2f pos_p=PauseButton.getPosition();
    sf::Vector2f size_p=PauseButton.getSize();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {window.close(); game=0;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)||sf::Keyboard::isKeyPressed(sf::Keyboard::W)){p=3;} else {p=0;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)||sf::Keyboard::isKeyPressed(sf::Keyboard::S)){t=3;} else {t=0;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)||sf::Keyboard::isKeyPressed(sf::Keyboard::A)){rotl=3;} else {rotl=0;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)||sf::Keyboard::isKeyPressed(sf::Keyboard::D)){rotp=3;} else {rotp=0;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){shift=3;} else {shift=1;}
            if (ps_button==1&&(clock2.getElapsedTime().asMilliseconds()>=500)&&(sf::Keyboard::isKeyPressed(sf::Keyboard::P)||(event.mouseButton.button==sf::Mouse::Left&&
                                                                                                             (pos.x>=pos_p.x&&pos.x<=pos_p.x+size_p.x&&pos.y>=pos_p.y&&pos.y<=pos_p.y+size_p.y))))
            { Gamestate="Pause"; clock2.restart(); pclock.restart();}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)&&clock2.getElapsedTime().asMilliseconds()>=50) {clock2.restart();return 1;}
        }
        if(ps_button==1)
            window.draw(PauseButton);
        return 0;
}
//
//Main menu
//
void mainMenu(sf::RenderWindow &window, std::string &Gamestate, int &game,sf::Clock &clock)
{
    sf::Event event;
    sf::Mouse mouse;
    sf::Vector2i pos=mouse.getPosition(window);


    static sf::RectangleShape StartButton;
    load_texture(StartButton, "Space_Game_GUI_PNG/PNG/Main_Menu/Start_BTN.png");
    StartButton.setSize(sf::Vector2f(200,50));
    StartButton.setPosition(H/2-StartButton.getSize().x/2,W/2-StartButton.getSize().y/2);
    sf::Vector2f pos_start=StartButton.getPosition();
    sf::Vector2f size_start=StartButton.getSize();

    static sf::RectangleShape ExitButton;
    load_texture(ExitButton, "Space_Game_GUI_PNG/PNG/Main_Menu/Exit_BTN.png");
    ExitButton.setSize(sf::Vector2f(200,50));
    ExitButton.setPosition(H/2-ExitButton.getSize().x/2,W/2-ExitButton.getSize().y/2+100);
    sf::Vector2f pos_exit=ExitButton.getPosition();
    sf::Vector2f size_exit=ExitButton.getSize();

    static sf::RectangleShape HowToButton;
    load_texture(HowToButton, "Space_Game_GUI_PNG/PNG/Main_Menu/Info_BTN.png");
    HowToButton.setSize(sf::Vector2f(50,50));
    HowToButton.setPosition(H/2-HowToButton.getSize().x/2,W/2-HowToButton.getSize().y/2+50);
    sf::Vector2f pos_howto=HowToButton.getPosition();
    sf::Vector2f size_howto=HowToButton.getSize();

    static sf::RectangleShape Name;
    load_texture(Name, "Space_Game_GUI_PNG/PNG/Main_Menu/Header.png");
    Name.setSize(sf::Vector2f(400,100));
    Name.setPosition(H/2-Name.getSize().x/2,W/2-Name.getSize().y/2-200);


    while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {window.close(); game=0;}
            if (clock.getElapsedTime().asMilliseconds()>=500&&event.mouseButton.button==sf::Mouse::Left&&(pos.x>=pos_start.x&&pos.x<=pos_start.x+size_start.x&&pos.y>=pos_start.y&&pos.y<=pos_start.y+size_start.y))
                {Gamestate="Play";}
            if (event.mouseButton.button==sf::Mouse::Left&&(pos.x>=pos_exit.x&&pos.x<=pos_exit.x+size_exit.x&&pos.y>=pos_exit.y&&pos.y<=pos_exit.y+size_exit.y))
                {Gamestate="Exit";game=0;window.close();}
            if (event.mouseButton.button==sf::Mouse::Left&&(pos.x>=pos_howto.x&&pos.x<=pos_howto.x+size_howto.x&&pos.y>=pos_howto.y&&pos.y<=pos_howto.y+size_howto.y))
                {Gamestate="HowTo";}
        }
    window.draw(StartButton);
    window.draw(ExitButton);
    window.draw(HowToButton);
    window.draw(Name);
}
//
//loss Menu
//
void lossMenu(sf::RenderWindow &window, std::string &Gamestate, int &game,sf::Clock &clock, sf::Font &font, int points,float time)
{
    sf::Event event;
    sf::Mouse mouse;
    sf::Vector2i pos=mouse.getPosition(window);


    static sf::RectangleShape StartButton;
    load_texture(StartButton, "Space_Game_GUI_PNG/PNG/Pause/Menu_BTN.png");
    StartButton.setSize(sf::Vector2f(50,50));
    StartButton.setPosition(H/2-3*StartButton.getSize().x,W/2-StartButton.getSize().y/2);
    sf::Vector2f pos_start=StartButton.getPosition();
    sf::Vector2f size_start=StartButton.getSize();

    static sf::RectangleShape ExitButton;
    load_texture(ExitButton, "Space_Game_GUI_PNG/PNG/Main_Menu/Exit_BTN.png");
    ExitButton.setSize(sf::Vector2f(200,50));
    ExitButton.setPosition(H/2-ExitButton.getSize().x/2+50,W/2-ExitButton.getSize().y/2);
    sf::Vector2f pos_exit=ExitButton.getPosition();
    sf::Vector2f size_exit=ExitButton.getSize();

    static sf::RectangleShape Name;
    load_texture(Name, "Space_Game_GUI_PNG/PNG/You_Lose/Header.png");
    Name.setSize(sf::Vector2f(400,100));
    Name.setPosition(H/2-Name.getSize().x/2,W/2-Name.getSize().y/2-200);

    static sf::RectangleShape Score;
    Score.setSize(sf::Vector2f(200,100));
    Score.setPosition(H/2-2*Score.getSize().x/2,W/2-Score.getSize().y/2-100);
    Score.setFillColor(sf::Color::Black);

    std::string str1,pts = "You scored: ";
    std::stringstream ss1;
    ss1 << points;
    ss1 >> str1;
    pts += str1;
    pts += "\n";
    std::string str2;
    std::stringstream ss2;
    ss2 << time;
    ss2 >> str2;
    pts += "Your time: ";
    pts += str2;
    pts += " s";

    static sf::Text Score_txt;
    Score_txt.setString(pts);
    Score_txt.setFont(font);
    Score_txt.setCharacterSize(50);
    Score_txt.setPosition(Score.getPosition());

    while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {window.close(); game=0;}
            if (event.mouseButton.button==sf::Mouse::Left&&(pos.x>=pos_start.x&&pos.x<=pos_start.x+size_start.x&&pos.y>=pos_start.y&&pos.y<=pos_start.y+size_start.y))
                {Gamestate="Menu";clock.restart();}
            if (event.mouseButton.button==sf::Mouse::Left&&(pos.x>=pos_exit.x&&pos.x<=pos_exit.x+size_exit.x&&pos.y>=pos_exit.y&&pos.y<=pos_exit.y+size_exit.y))
                {Gamestate="Exit";game=0;window.close();}
        }
    window.draw(StartButton);
    window.draw(ExitButton);
    window.draw(Name);
    window.draw(Score_txt);
}
//
//Win Menu
//
void winMenu(sf::RenderWindow &window, std::string &Gamestate, int &game,sf::Clock &clock, sf::Font &font, int points,float time)
{
    sf::Event event;
    sf::Mouse mouse;
    sf::Vector2i pos=mouse.getPosition(window);


    static sf::RectangleShape StartButton;
    load_texture(StartButton, "Space_Game_GUI_PNG/PNG/Pause/Menu_BTN.png");
    StartButton.setSize(sf::Vector2f(50,50));
    StartButton.setPosition(H/2-3*StartButton.getSize().x,W/2-StartButton.getSize().y/2);
    sf::Vector2f pos_start=StartButton.getPosition();
    sf::Vector2f size_start=StartButton.getSize();

    static sf::RectangleShape ExitButton;
    load_texture(ExitButton, "Space_Game_GUI_PNG/PNG/Main_Menu/Exit_BTN.png");
    ExitButton.setSize(sf::Vector2f(200,50));
    ExitButton.setPosition(H/2-ExitButton.getSize().x/2+50,W/2-ExitButton.getSize().y/2);
    sf::Vector2f pos_exit=ExitButton.getPosition();
    sf::Vector2f size_exit=ExitButton.getSize();

    static sf::RectangleShape Name;
    load_texture(Name, "Space_Game_GUI_PNG/PNG/You_Win/Header.png");
    Name.setSize(sf::Vector2f(400,100));
    Name.setPosition(H/2-Name.getSize().x/2,W/2-Name.getSize().y/2-200);


    static sf::RectangleShape Score;
    Score.setSize(sf::Vector2f(200,100));
    Score.setPosition(H/2-2*Score.getSize().x/2,W/2-Score.getSize().y/2-100);
    Score.setFillColor(sf::Color::Black);

    std::string str1,pts = "You scored: ";
    std::stringstream ss1;
    ss1 << points;
    ss1 >> str1;
    pts += str1;
    pts += "\n";
    std::string str2;
    std::stringstream ss2;
    ss2 << time;
    ss2 >> str2;
    pts += "Your time: ";
    pts += str2;
    pts += " s";

    static sf::Text Score_txt;
    Score_txt.setString(pts);
    Score_txt.setFont(font);
    Score_txt.setCharacterSize(50);
    Score_txt.setPosition(Score.getPosition());


    while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {window.close(); game=0;}
            if (event.mouseButton.button==sf::Mouse::Left&&(pos.x>=pos_start.x&&pos.x<=pos_start.x+size_start.x&&pos.y>=pos_start.y&&pos.y<=pos_start.y+size_start.y))
                {Gamestate="Menu";clock.restart();}
            if (event.mouseButton.button==sf::Mouse::Left&&(pos.x>=pos_exit.x&&pos.x<=pos_exit.x+size_exit.x&&pos.y>=pos_exit.y&&pos.y<=pos_exit.y+size_exit.y))
                {Gamestate="Exit";game=0;window.close();}
        }
    window.draw(StartButton);
    window.draw(ExitButton);
    window.draw(Name);
    window.draw(Score_txt);
}
//
//Pause
//
void pauseMenu(sf::RenderWindow &window, std::string &Gamestate, sf::Clock &clock2)
{
    sf::Event event;
    sf::Mouse mouse;
    sf::Vector2i pos=mouse.getPosition(window);
    static int init=0;
    static sf::Vector2f pos_start;
    static sf::Vector2f size_start;
    static sf::Vector2f pos_exit;
    static sf::Vector2f size_exit;
    static sf::Vector2f pos_name;
    static sf::Vector2f size_name;
    static sf::RectangleShape StartButton;
    static sf::RectangleShape ExitButton;
    static sf::RectangleShape Name;

    if (init==0)
    {

    load_texture(StartButton, "Space_Game_GUI_PNG/PNG/Level_Menu/Play_BTN.png");
    StartButton.setSize(sf::Vector2f(50,50));
    StartButton.setPosition(H/2-StartButton.getSize().x,W/2-StartButton.getSize().y/2);
    pos_start=StartButton.getPosition();
    size_start=StartButton.getSize();


    load_texture(ExitButton, "Space_Game_GUI_PNG/PNG/Pause/Menu_BTN.png");
    ExitButton.setSize(sf::Vector2f(50,50));
    ExitButton.setPosition(H/2-ExitButton.getSize().x/2+ExitButton.getSize().x,W/2-ExitButton.getSize().y/2);
    pos_exit=ExitButton.getPosition();
    size_exit=ExitButton.getSize();


    load_texture(Name, "Space_Game_GUI_PNG/PNG/Pause/Header.png");
    Name.setSize(sf::Vector2f(400,100));
    Name.setPosition(H/2-Name.getSize().x/2,W/2-Name.getSize().y/2-200);
    pos_name=Name.getPosition();
    size_name=Name.getSize();

    init=1;
    }



    while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {window.close(); game=0; Gamestate="Play";}
            if (event.mouseButton.button==sf::Mouse::Left&&(pos.x>=pos_start.x&&pos.x<=pos_start.x+size_start.x&&pos.y>=pos_start.y&&pos.y<=pos_start.y+size_start.y))
                {Gamestate="Play";}
            if (event.mouseButton.button==sf::Mouse::Left&&(pos.x>=pos_exit.x&&pos.x<=pos_exit.x+size_exit.x&&pos.y>=pos_exit.y&&pos.y<=pos_exit.y+size_exit.y))
                {Gamestate="Menu";clock2.restart();}
        }

    window.draw(StartButton);
    window.draw(ExitButton);
    window.draw(Name);
}
//
//Instructions window
//
void HowTo (sf::RenderWindow &window, std::string &Gamestate,sf::Font &font,
            sf::CircleShape &player,sf::Clock &clock2, std::vector<moving> &shots, std::vector<moving> &asteroids,sf::Sound &sound,sf::Sound &move_sound,sf::Sound &destroy_sound)
{
    sf::Event event;
    sf::Mouse mouse;
    sf::Vector2i pos=mouse.getPosition(window);
    static std::vector<sf::Text> pages;
    static int page = 0;
    static int pagesprep = 0;

    if (pagesprep==0)
    {
        std::string page1str="Use arrows or WASD to move.";
        static sf::Text page1;
        page1.setString(page1str);
        page1.setFont(font);
        page1.setCharacterSize(30);
        page1.setPosition(H/2-300,W/2-400);

        pages.push_back(page1);

        std::string page2str="Use LShift to speed up.";
        static sf::Text page2;
        page2.setString(page2str);
        page2.setFont(font);
        page2.setCharacterSize(30);
        page2.setPosition(H/2-400,W/2-400);

        pages.push_back(page2);

        std::string page3str="Avoid the asteroids floating around.\nPress space to shoot and try to destroy them.";
        static sf::Text page3;
        page3.setString(page3str);
        page3.setFont(font);
        page3.setCharacterSize(30);
        page3.setPosition(H/2-400,W/2-400);

        pages.push_back(page3);

        std::string page4str="You're good to go!\nTo win you need to score ";
        std::stringstream ss;
        std::string str;
        ss<<win_cond;
        ss>>str;
        page4str+=str;
        page4str+=" asteroid hits.\nGood luck.\n\nClick ENTER to continue";
        static sf::Text page4;
        page4.setString(page4str);
        page4.setFont(font);
        page4.setCharacterSize(30);
        page4.setPosition(H/2-400,W/2-400);

        pages.push_back(page4);
        pages.push_back(page4);

        pagesprep = 1;
    }


    while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {Gamestate == "Exit";window.close(); game=0; }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)||sf::Keyboard::isKeyPressed(sf::Keyboard::W)){p=3;} else {p=0;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)||sf::Keyboard::isKeyPressed(sf::Keyboard::S)){t=3;} else {t=0;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)||sf::Keyboard::isKeyPressed(sf::Keyboard::A)){rotl=3;} else {rotl=0;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)||sf::Keyboard::isKeyPressed(sf::Keyboard::D)){rotp=3;} else {rotp=0;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){shift=3;} else {shift=1;}

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)&&clock2.getElapsedTime().asMilliseconds()>=50) {clock2.restart();shoot(player,shots,sound);}

            if (page == 0&&(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)||sf::Keyboard::isKeyPressed(sf::Keyboard::W)||
                            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)||sf::Keyboard::isKeyPressed(sf::Keyboard::S)||
                            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)||sf::Keyboard::isKeyPressed(sf::Keyboard::D)||
                            sf::Keyboard::isKeyPressed(sf::Keyboard::Left)||sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
                page++;
            if (page == 1&&(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)))
                page++;
            if (page == 3&&sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                page++;
        }
        if (page == 2&&asteroids.size()==0)
                page++;
    //std::cout<<pages.size()<<":"<<page<<"\n";
    window.draw(pages[page]);
        for (int i=0;i<shots.size();i++)
            {window.draw(shots[i].bullet);shots[i].bullet.move(15*shots[i].vx,15*shots[i].vy);}

     if (asteroids.size()==0)
    {
        moving aster_temp;
        sf::CircleShape bullet_temp;
        bullet_temp.setPosition(sf::Vector2f(player.getPosition()+sf::Vector2f(-400.,0.)));
        bullet_temp.setRadius(100.f);
        load_texture(bullet_temp,"Asteroids/Asteroid_1.png");
        aster_temp.bullet = bullet_temp;
        aster_temp.vx=0;
        aster_temp.vy=0;
        asteroids.push_back(aster_temp);
    }

    if(page == 2)
        {
            for (int j=0;j<asteroids.size();j++)
                window.draw(asteroids[j].bullet);

        int shotssize = shots.size();
        int astersize = asteroids.size();
            for(int i=0;i<shotssize;i++)
            {

                for(int j=0;j<astersize;j++)
                {
                    if(bullet_impact(shots[i],asteroids[j]))
                    {
                        destroy_sound.play();
                        shots.erase(shots.begin()+i);
                        shotssize += -1;
                        astersize += -1;
                        moving aster_temp=asteroids[j];
                        int asize = asteroids.size();
                        asteroids.erase(asteroids.begin()+j);
                        asteroids.resize(asize-1);

                    }
                }
            }
        }

    controled_move(player,window,move_sound);
    if (page>=4)
        {Gamestate="Menu";destroy_sound.stop();page=0;}


}
#endif // _Functions_CPP
