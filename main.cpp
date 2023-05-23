#include "CPPCodes/functions.h"
#include "CPPCodes/includes.h"


//
//Main Function
//
int main()
{
    int pew=0;
    int shotssize;
    int astersize;
    static sf::RenderWindow window;
    window.create(sf::VideoMode(H, W), "Game window");
    int asize=0;
    int points=0;

    float finaltime=0, pausetime;
    asteroids_setup();
    sf::Text ctime;
    sf::Clock aspawn;
    sf::Clock clock2;
    sf::Clock play_time;
    sf::Clock pause_clock;

    sf::RectangleShape tlo(sf::Vector2f(H,W));
    std::string plik_tlo="Space_Background_1.png";
    load_texture(tlo, plik_tlo);


    sf::SoundBuffer gun_buffer;
    if (!gun_buffer.loadFromFile("200SpaceSFX/Space-Robotic-Futuristic-Wav/laser.wav"))
        std::cout<<"error, no sound\n";
    sf::Sound gun_sound;
    gun_sound.setBuffer(gun_buffer);
    gun_sound.setVolume(25);
    gun_sound.setPitch(3);

    sf::SoundBuffer win_buffer;
    if (!win_buffer.loadFromFile("200SpaceSFX/Space-Robotic-Futuristic-Wav/win_loud.wav"))
        std::cout<<"error, no sound\n";
    sf::Sound win_sound;
    win_sound.setBuffer(win_buffer);
    win_sound.setVolume(25);
    win_sound.setPitch(2);

    sf::SoundBuffer loss_buffer;
    if (!loss_buffer.loadFromFile("200SpaceSFX/Space-Robotic-Futuristic-Wav/deadmyforce.wav"))
        std::cout<<"error, no sound\n";
    sf::Sound loss_sound;
    loss_sound.setBuffer(loss_buffer);
    loss_sound.setVolume(25);

    sf::SoundBuffer move_buffer;
    if (!move_buffer.loadFromFile("200SpaceSFX/Space-Robotic-Futuristic-Wav/energy_upload_loop_edit.wav"))
        std::cout<<"error, no sound,  move\n";
    sf::Sound move_sound;
    move_sound.setBuffer(move_buffer);
    move_sound.setVolume(5);

    sf::SoundBuffer destroy_buffer;
    if (!destroy_buffer.loadFromFile("200SpaceSFX/Space-Robotic-Futuristic-Wav/destroyed_stones.wav"))
        std::cout<<"error, no sound, destroy\n";
    sf::Sound destroy_sound;
    destroy_sound.setBuffer(destroy_buffer);
    destroy_sound.setVolume(25);

    sf::SoundBuffer sound1_buffer;
    if (!sound1_buffer.loadFromFile("200SpaceSFX/Space-Robotic-Futuristic-Wav/unitready3.wav"))
        std::cout<<"error, no sound\n";
    static sf::Sound sound1;
    sound1.setBuffer(sound1_buffer);
    sound1.setVolume(25);

    sf::SoundBuffer sound2_buffer;
    if (!sound2_buffer.loadFromFile("200SpaceSFX/Space-Robotic-Futuristic-Wav/unitready.wav"))
        std::cout<<"error, no sound\n";
    static sf::Sound sound2;
    sound2.setBuffer(sound2_buffer);
    sound2.setVolume(25);

    sf::Music menu_music;
    if (!menu_music.openFromFile("02_Space_Riddle.ogg"))
        std::cout<<"error, no music\n";
    menu_music.setLoop(true);
    menu_music.setVolume(10);

    sf::Music play_music;
    if (!play_music.openFromFile("The_Desolate_Planet_LOOP.ogg"))
        std::cout<<"error, no music\n";
    play_music.setLoop(true);
    play_music.setVolume(10);

    sf::CircleShape shape(100.f);

    sf::Font CenturyGothic;
    if(!CenturyGothic.loadFromFile("CenturyGothic.ttf"))
        std::cout<<"error, no font found\n";


    player.setPosition(H/2,W/2);
    load_texture(player,"Spaceship.png");
    player.setOrigin(player.getRadius(),player.getRadius());

    window.setFramerateLimit(60);


    while(game!=0)
    {
    if (Gamestate=="Menu"||Gamestate=="HowTo")
        menu_music.play();

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

    while(Gamestate=="HowTo"&&window.isOpen())
    {
        window.draw(tlo);

        HowTo(window,Gamestate,CenturyGothic, player,clock2,shots,asteroids,gun_sound,move_sound,destroy_sound);

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

    menu_music.stop();

    while(window.isOpen()&&play_time.getElapsedTime().asSeconds()<4&&Gamestate=="Play")
    {
        window.draw(tlo);
        window.draw(player);

        countdown(window,play_time,CenturyGothic,sound1,sound2);

        window.display();
    }

    play_time.restart();
    pause_clock.restart();
    clock2.restart();
    while (window.isOpen()&&Gamestate=="Play")
    {
        if (play_music.getStatus()!=sf::Music::Status::Playing)
            play_music.play();
        window.draw(tlo);
        pew=events(window,aspawn,clock2,pause_clock,Gamestate,1);



        while(Gamestate=="Pause")
        {
            play_music.pause();
            if(move_sound.getStatus()==sf::Sound::Status::Playing)
                move_sound.stop();

            window.draw(player);
            for (int i=0;i<asteroids.size();i++)
                window.draw(asteroids[i].bullet);
            for (int j=0;j<shots.size();j++)
                window.draw(shots[j].bullet);

            PointCounter(points, CenturyGothic,window);
            window.draw(ctime);
            pauseMenu(window,Gamestate,clock2);


            if (Gamestate=="Play")
            {

                pausetime+= pause_clock.getElapsedTime().asSeconds()*100;
                pause_clock.restart();
                play_music.play();
            }
            window.display();

        }

        if (aspawn.getElapsedTime().asSeconds()>=5)
            {
                asteroid_spawn(100.f,asteroids);
                aspawn.restart();
            }


        if (pew==1)
            shoot(player,shots,gun_sound);

        shotssize = shots.size();
        astersize = asteroids.size();
            for(int i=0;i<shotssize;i++)
            {
                //std::cout<<"bullet: "<<i<<"check\n";
                for(int j=0;j<astersize;j++)
                {

                    if(bullet_impact(shots[i],asteroids[j]))
                    {
                        destroy_sound.play();
                        moving bullet_temp = shots[i];
                        shots.erase(shots.begin()+i);
                        shotssize += -1;
                        astersize += -1;
                        moving aster_temp=asteroids[j];
                        asize = asteroids.size();
                        //std::cout<<"asize = "<<asize<<"\n";
                        asteroids.erase(asteroids.begin()+j);
                        asteroids.resize(asize-1);
                        //std::cout<<"current asize = "<<asteroids.size()<<"\n";
                        if (aster_temp.bullet.getRadius()>=40)
                        {
                            asteroid_split(aster_temp,asteroids,bullet_temp);
                            //std::cout<<"split\n";
                        }
                        //std::cout<<"final size = "<<asteroids.size()<<"\n";
                        points++;
                        if (points>=win_cond)
                            {Gamestate="Win";finaltime=int(play_time.getElapsedTime().asSeconds()*100-pausetime)/100.;play_music.stop();win_sound.play();}
                        //std::cout<<"Control txt\n"<<int(play_time.getElapsedTime().asSeconds()*100-pausetime)/100.<<"\n";
                    }
                    //std::cout<<" "<<j<<" ";
                }
                //std::cout<<"CoNtRoL TxT \n";
            }


            for(int i=0;i<asteroids.size();i++)
                {
                    if(asteroids[i].bullet.getTexture()!=NULL)
                    {
                        window.draw(asteroids[i].bullet);
                        asteroids[i].bullet.move(asteroids[i].vx,asteroids[i].vy);
                    }
                    if(asteroids[i].bullet.getPosition().x<-50||asteroids[i].bullet.getPosition().x>H+50||asteroids[i].bullet.getPosition().y<-50||asteroids[i].bullet.getPosition().y>W+50)
                    {
                        int asize2 = asteroids.size();
                        asteroids.erase(asteroids.begin()+i);
                        asteroids.resize(asize2-1);
                        asteroid_spawn(100.f,asteroids);
                    }
                    if(asteroid_impact(player,asteroids[i].bullet))
                        {Gamestate="Loss"; finaltime=int(play_time.getElapsedTime().asSeconds()*100-pausetime)/100.;play_music.stop();loss_sound.play();}

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



        sf::Vector2f pos;
        pos=player.getPosition();
        float rotation=player.getRotation();

        controled_move(player,window,move_sound);

        if ((player.getRotation()!=rotation||player.getPosition()!=pos)&&move_sound.getStatus()!=sf::Sound::Status::Playing)
            {move_sound.play();move_sound.setLoop(1);}
        else if (player.getRotation()==rotation&&player.getPosition()==pos)
            move_sound.setLoop(false);

        PointCounter(points, CenturyGothic,window);
        ctime=time(play_time, CenturyGothic, pausetime,window);

        window.display();

    }
    move_sound.stop();
    while(window.isOpen()&&Gamestate=="Loss")
    {
        window.draw(tlo);
        window.draw(player);
        for(int i=0;i<asteroids.size();i++)
            window.draw(asteroids[i].bullet);
        for(int i=0;i<shots.size();i++)
            window.draw(shots[i].bullet);
        lossMenu(window,Gamestate,game,clock2,CenturyGothic,points,finaltime);


        window.display();
    }
    while(window.isOpen()&&Gamestate=="Win")
    {
        window.draw(tlo);
        window.draw(player);
        for(int i=0;i<asteroids.size();i++)
            window.draw(asteroids[i].bullet);
        for(int i=0;i<shots.size();i++)
            window.draw(shots[i].bullet);
        winMenu(window,Gamestate,game,clock2,CenturyGothic,points,finaltime);


        window.display();
    }


    }

    return 0;
}
