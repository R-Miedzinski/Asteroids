#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <sstream>

//win_cond lat you control how much points you need to achieve to win. Assign desired value.
static int win_cond = 200;
//Height and Width of the game window:
static int H=1600,W=900;

static int game=1;
static int p=0,t=0,rotl=0,rotp=0,shift=1,ti=1;
static sf::RenderWindow window(sf::VideoMode(H, W), "Game window");
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
static std::vector<moving> shots;
static std::vector<moving> asteroids;

/*Game Functions*/

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
        else//reloading used textures
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
    static sf::CircleShape dummy[18];
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
bool safe_zone(sf::CircleShape &shape, sf::CircleShape &asteroid)
{
    float x = shape.getPosition().x - asteroid.getPosition().x - asteroid.getRadius();
    float y = shape.getPosition().y - asteroid.getPosition().y - asteroid.getRadius();

    if  (sqrt(x*x+y*y)<asteroid.getRadius()+shape.getRadius())
        return true;
    else
        return false;
}
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
}
//
//Shooting
//
void shoot (sf::CircleShape &shape)
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
}
//
//Reading window events
//
void events(sf::RenderWindow &window, sf::Clock &clock, sf::Clock &clock2, sf::Clock &pclock)
{
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
            if (clock2.getElapsedTime().asMilliseconds()>=500&&(sf::Keyboard::isKeyPressed(sf::Keyboard::P)||(event.mouseButton.button==sf::Mouse::Left&&
                                                                                                             (pos.x>=pos_p.x&&pos.x<=pos_p.x+size_p.x&&pos.y>=pos_p.y&&pos.y<=pos_p.y+size_p.y))))
            { Gamestate="Pause"; clock2.restart(); pclock.restart();}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)&&clock2.getElapsedTime().asMilliseconds()>=50) {shoot(player);clock2.restart();}
        }
        window.draw(PauseButton);
}
//
//player movement function
//
void controled_move(sf::CircleShape &shape)
{

    int pop = 20;
    float x = shape.getPosition().x;
    float y = shape.getPosition().y;


    //rotation
    shape.setPosition(0,0);
    shape.rotate(-rotl+rotp);
    shape.setPosition(x,y);



    //directional movement
    float xdir = (-t*sin(shape.getRotation()*3.14159/180)+p*sin(shape.getRotation()*3.14159/180))*shift;
    float ydir = (t*cos(shape.getRotation()*3.14159/180)-p*cos(shape.getRotation()*3.14159/180))*shift;
    shape.move(xdir,ydir);

    //window border colision
    if(shape.getPosition().x<-pop) {shape.setPosition(H+pop,shape.getPosition().y);}
    if(shape.getPosition().x>H+pop) {shape.setPosition(-pop,shape.getPosition().y);}
    if(shape.getPosition().y<-pop) {shape.setPosition(shape.getPosition().x,W+pop);}
    if(shape.getPosition().y>W+pop) {shape.setPosition(shape.getPosition().x,-pop);}

    window.draw(shape);
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
//display current score
//
void PointCounter(int points, sf::Font &font)
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
//
//display current play time
//
sf::Text time(sf::Clock &clock, sf::Font &font, float ptime)
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
        }
    window.draw(StartButton);
    window.draw(ExitButton);
    window.draw(Name);
}
//
//Countdown before game starts
//
void countdown(sf::RenderWindow &window, sf::Clock &clock, sf::Font &font)
{
    sf::Text text;
    static int tt;


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
    sf::Vector2f pos_name=Name.getPosition();
    sf::Vector2f size_name=Name.getSize();

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
                {window.close(); game=0;}
            if (event.type == sf::Event::Closed||sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {Gamestate="Play";}
            if (event.mouseButton.button==sf::Mouse::Left&&(pos.x>=pos_start.x&&pos.x<=pos_start.x+size_start.x&&pos.y>=pos_start.y&&pos.y<=pos_start.y+size_start.y))
                {Gamestate="Play";}
            if (event.mouseButton.button==sf::Mouse::Left&&(pos.x>=pos_exit.x&&pos.x<=pos_exit.x+size_exit.x&&pos.y>=pos_exit.y&&pos.y<=pos_exit.y+size_exit.y))
                {Gamestate="Menu";clock2.restart();}
        }

    window.draw(StartButton);
    window.draw(ExitButton);
    window.draw(Name);
}
/*Game Functions*/

//
//Main Function
//
int main()
{
    int asize=0;
    int points=0;

    float finaltime, pausetime;
    asteroids_setup();
    sf::Text ctime;
    sf::Clock aspawn;
    sf::Clock clock2;
    sf::Clock play_time;
    sf::Clock pause_clock;

    sf::RectangleShape tlo(sf::Vector2f(H,W));
    std::string plik_tlo="Space_Background_1.png";
    load_texture(tlo, plik_tlo);





    sf::CircleShape shape(100.f);

    sf::Font CenturyGothic;
    if(!CenturyGothic.loadFromFile("CenturyGothic.ttf"))
        std::cout<<"error";


    player.setPosition(H/2,W/2);
    load_texture(player,"Spaceship.png");
    player.setOrigin(player.getRadius(),player.getRadius());

    /*sf::CircleShape protected_sphere;
    sf::Vector2f pos = player.getPosition();
    float s_radius = player.getRadius();
    protected_sphere.setPosition(pos.x-2*s_radius,pos.y-2*s_radius);
    protected_sphere.setRadius(3*s_radius);*/


    window.setFramerateLimit(60);


    while(game)
    {

    while(Gamestate=="Menu"&&window.isOpen())
    {

        window.draw(tlo);

        mainMenu(window,Gamestate,game,clock2);

        window.display();
    }

    play_time.restart();
    aspawn.restart();
    player.setPosition(H/2,W/2);
    player.setRotation(0);
    asteroids.clear();
    shots.clear();
    points = 0;
    pausetime = 0;

    while(window.isOpen()&&play_time.getElapsedTime().asSeconds()<4)
    {
        window.draw(tlo);
        window.draw(player);

        countdown(window,play_time,CenturyGothic);

        window.display();
    }

    play_time.restart();
    pause_clock.restart();
    clock2.restart();
    while (window.isOpen()&&Gamestate=="Play")
    {

        /*pos = player.getPosition();
        s_radius = player.getRadius();
        protected_sphere.setPosition(pos.x-3*s_radius,pos.y-3*s_radius);
        protected_sphere.setRadius(3*s_radius);*/


        window.draw(tlo);
        events(window,aspawn,clock2,pause_clock);

        while(Gamestate=="Pause")
        {
            window.draw(player);
            for (int i=0;i<asteroids.size();i++)
                window.draw(asteroids[i].bullet);
            for (int j=0;j<shots.size();j++)
                window.draw(shots[j].bullet);

            PointCounter(points, CenturyGothic);
            window.draw(ctime);
            pauseMenu(window,Gamestate,clock2);


            if (Gamestate=="Play")
            {

                pausetime+= pause_clock.getElapsedTime().asSeconds()*100;
                pause_clock.restart();
                std::cout<<"Pause_time count\n";
            }
            window.display();

        }

        if (aspawn.getElapsedTime().asSeconds()>=5)
            {
                asteroid_spawn(100.f);
                aspawn.restart();
            }



        //window render
        //window.draw(protected_sphere);
            for(int i=0;i<shots.size();i++)
            {
                for(int j=0;j<asteroids.size();j++)
                {
                    if(bullet_impact(shots[i],asteroids[j]))
                    {
                        shots.erase(shots.begin()+i);
                        moving aster_temp=asteroids[j];
                        asize = asteroids.size();
                        std::cout<<"asize = "<<asize<<"\n";
                        asteroids.erase(asteroids.begin()+j);
                        asteroids.resize(asize-1);
                        std::cout<<"current asize = "<<asteroids.size()<<"\n";
                        if (aster_temp.bullet.getRadius()>=40)
                        {
                            asteroid_split(aster_temp);
                            std::cout<<"split\n";
                        }
                        std::cout<<"final size = "<<asteroids.size()<<"\n";
                        points++;
                        if (points>=win_cond)
                            {Gamestate="Win";finaltime=int(play_time.getElapsedTime().asSeconds()*100-pausetime)/100.;}
                        std::cout<<"Control txt\n"<<int(play_time.getElapsedTime().asSeconds()*100-pausetime)/100.<<"\n";
                    }
                }
            }
            //std::cout<<"ctrl text\n";
            //if (asize<asteroids.size()) std::cout<<"control txt no2\n";
            //asize=asteroids.size();

            for(int i=0;i<asteroids.size();i++)
                {
                    if(asteroids[i].bullet.getTexture()!=NULL)
                    {
                        window.draw(asteroids[i].bullet);
                        asteroids[i].bullet.move(ti*asteroids[i].vx,ti*asteroids[i].vy);
                    }
                    if(asteroids[i].bullet.getPosition().x<-50||asteroids[i].bullet.getPosition().x>H+50||asteroids[i].bullet.getPosition().y<-50||asteroids[i].bullet.getPosition().y>W+50)
                    {
                        int asize2 = asteroids.size();
                        asteroids.erase(asteroids.begin()+i);
                        asteroids.resize(asize2-1);
                        asteroid_spawn(100.f);
                    }
                    if(asteroid_impact(player,asteroids[i].bullet))
                        {Gamestate="Loss"; finaltime=int(play_time.getElapsedTime().asSeconds()*100-pausetime)/100.;}

                }
            for(int i=0;i<shots.size();i++)
            {
                    shots[i].bullet.move(15*shots[i].vx,15*shots[i].vy);
                    window.draw(shots[i].bullet);
                    if(shots[i].bullet.getPosition().x<-50||shots[i].bullet.getPosition().x>H+50||shots[i].bullet.getPosition().y<-50||shots[i].bullet.getPosition().y>W+50)
                    {
                        shots.erase(shots.begin()+i);
                    }
            }



        controled_move(player);

        PointCounter(points, CenturyGothic);
        ctime=time(play_time, CenturyGothic, pausetime);

        window.display();

    }

    while(window.isOpen()&&Gamestate=="Loss")
    {
        window.draw(tlo);
        window.draw(player);
        for(int i=0;i<asteroids.size();i++)
            window.draw(asteroids[i].bullet);
        lossMenu(window,Gamestate,game,clock2,CenturyGothic,points,finaltime);


        window.display();
    }
    while(window.isOpen()&&Gamestate=="Win")
    {
        window.draw(tlo);
        window.draw(player);
        for(int i=0;i<asteroids.size();i++)
            window.draw(asteroids[i].bullet);
        winMenu(window,Gamestate,game,clock2,CenturyGothic,points,finaltime);


        window.display();
    }


    }

    return 0;
}
