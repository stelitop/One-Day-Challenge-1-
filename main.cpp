#include<iostream>
#include<SFML/Graphics.hpp>
#include<vector>
#include<cstdio>
#include<windows.h>
using namespace std;
using namespace sf;

const int SizeX=1280;
const int SizeY=1024;
const int birdSize=25;
int ticks=1,secs=0;
int LastX,LastY;
RenderWindow window(VideoMode(SizeX,SizeY),"",Style::Fullscreen);
RenderTexture screen;

enum ScreenType{INGAME,MAIN,PREGAME,SETTINGS,AFTERGAME} currentScreen=MAIN;
enum Rotation{LEFT,RIGHT,UP,DOWN} birdRot;

Texture background,bird,block,surface;

void loadTextures ()
{
    background.loadFromFile("Textures/Background.jpg");
    bird.loadFromFile("Textures/Bird.jpg");
    block.loadFromFile("Textures/Block.jpg");
    surface.loadFromFile("Textures/Surface.jpg");
}

struct Coords
{
    int x;
    int y;
};

struct object
{
    Coords center;
    int radius;
    int speed;
    Rotation motion;
};

vector<object> objects;

void GenerateObject(int speed,int sz)
{
    object addNew;
    int makeX,makeY;
    int side=rand()%4+1;
    /// UP, RIGHT, DOWN, LEFT
    if (side==1)
    {
        makeY=0-sz/2-1;
        makeX=rand()%SizeX;
        addNew.motion=DOWN;
    }
    if (side==2)
    {
        makeX=SizeX+sz/2+1;
        makeY=rand()%(SizeY*85/100);
        addNew.motion=LEFT;
    }
    if (side==3)
    {
        makeY=SizeY*85/100+sz/2+1;
        makeX=rand()%SizeX;
        addNew.motion=UP;
    }
    if (side==4)
    {
        makeX=0-sz/2-1;
        makeY=rand()%(SizeY*85/100);
        addNew.motion=RIGHT;
    }
    addNew.center.x=makeX;
    addNew.center.y=makeY;
    addNew.radius=sz/2;
    addNew.speed=speed;
    objects.push_back(addNew);
}

void StartGame(int difficulty)
{
    difficulty = min (difficulty,10);
    difficulty = max (difficulty,1);
    Mouse::setPosition(Vector2i(SizeX/2,SizeY/2),window);
    LastX = SizeX/2;
    LastY = SizeY/2;
    currentScreen = INGAME;
    objects.clear();
    ticks = 1;
    secs = 0;
}

int main()
{
    loadTextures();

    srand(time(0));
    StartGame(1);
    bool Update = true;
    while (window.isOpen())
    {
        int ShownX,ShownY;
        ShownX = Mouse::getPosition().x;
        ShownY = Mouse::getPosition().y;
        Event event;

        if (Update == true)
        {
            Update = false;
            //screen.create(SizeX,SizeY);
            //screen.clear(Color::Black);
            window.clear(Color::Black);

            if (currentScreen == INGAME)
            {
                VertexArray backgr(Quads, 4);
                backgr[0].position=Vector2f(0,0);
                backgr[1].position=Vector2f(SizeX,0);
                backgr[2].position=Vector2f(SizeX,SizeY);
                backgr[3].position=Vector2f(0,SizeY);
                backgr[0].texCoords=Vector2f(0,0);
                backgr[1].texCoords=Vector2f(1280,0);
                backgr[2].texCoords=Vector2f(1280,1024);
                backgr[3].texCoords=Vector2f(0,1024);

                window.draw(backgr,&background);

                VertexArray line(Lines, 2);
                line[0].position=Vector2f(-1,SizeY*85/100);
                line[1].position=Vector2f(SizeX,SizeY*85/100);
                window.draw(line);

                VertexArray player(Quads, 4);

                ShownX=min(ShownX,SizeX-birdSize/2);
                ShownX=max(ShownX,birdSize/2);
                ShownY=min(ShownY,SizeY*85/100-birdSize/2);
                ShownY=max(ShownY,birdSize/2);

                player[0].position=Vector2f(ShownX+birdSize/2,ShownY+birdSize/2);
                player[1].position=Vector2f(ShownX+birdSize/2,ShownY-birdSize/2);
                player[2].position=Vector2f(ShownX-birdSize/2,ShownY-birdSize/2);
                player[3].position=Vector2f(ShownX-birdSize/2,ShownY+birdSize/2);

                if (LastX>Mouse::getPosition().x)
                    birdRot=LEFT;
                if (LastX<Mouse::getPosition().x)
                    birdRot=RIGHT;

                if (birdRot==LEFT)
                {
                    player[0].texCoords=Vector2f(50,50);
                    player[1].texCoords=Vector2f(50,0);
                    player[2].texCoords=Vector2f(0,0);
                    player[3].texCoords=Vector2f(0,50);
                }
                if (birdRot==RIGHT)
                {
                    player[0].texCoords=Vector2f(0,50);
                    player[1].texCoords=Vector2f(0,0);
                    player[2].texCoords=Vector2f(50,0);
                    player[3].texCoords=Vector2f(50,50);

                }
                window.draw(player,&bird);

                for (int i=0;i<objects.size();i++)
                {player[3].texCoords=Vector2f(0,50);
                    VertexArray object(Quads, 4);
                    object[0].position=Vector2f(objects[i].center.x+objects[i].radius,objects[i].center.y+objects[i].radius);
                    object[1].position=Vector2f(objects[i].center.x+objects[i].radius,objects[i].center.y-objects[i].radius);
                    object[2].position=Vector2f(objects[i].center.x-objects[i].radius,objects[i].center.y-objects[i].radius);
                    object[3].position=Vector2f(objects[i].center.x-objects[i].radius,objects[i].center.y+objects[i].radius);

                    object[0].texCoords=Vector2f(50,50);
                    object[1].texCoords=Vector2f(50,0);
                    object[2].texCoords=Vector2f(0,0);
                    object[3].texCoords=Vector2f(0,50);

                    window.draw(object,&block);
                }

                VertexArray surfacev(Quads, 4);
                surfacev[0].position=Vector2f(0,SizeY*85/100);
                surfacev[1].position=Vector2f(SizeX,SizeY*85/100);
                surfacev[2].position=Vector2f(SizeX,SizeY);
                surfacev[3].position=Vector2f(0,SizeY);

                surfacev[0].texCoords=Vector2f(0,0);
                surfacev[1].texCoords=Vector2f(1280,0);
                surfacev[2].texCoords=Vector2f(1280,153);
                surfacev[3].texCoords=Vector2f(0,153);

                window.draw(surfacev,&surface);
            }

            //screen.display();
            //Sprite prnt;
            //prnt.setTexture(screen.getTexture());
            //window.draw(prnt);
            window.display();
        }

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
            {
                if (Keyboard::isKeyPressed(Keyboard::Escape))
                    window.close();
                if (Keyboard::isKeyPressed(Keyboard::Num1))
                {
                    StartGame(1);
                }
            }
        }

        if (currentScreen == INGAME)
        {
            window.setMouseCursorVisible(false);
            Update=true;

            if (Mouse::getPosition().x>LastX && LastX<birdSize/2)
                Mouse::setPosition(Vector2i(birdSize/2,Mouse::getPosition().y));

            if (Mouse::getPosition().x<LastX && LastX>SizeX-birdSize/2)
                Mouse::setPosition(Vector2i(SizeX-birdSize/2,Mouse::getPosition().y));

            if (Mouse::getPosition().y>LastY && LastY<birdSize/2)
                Mouse::setPosition(Vector2i(Mouse::getPosition().x,birdSize/2));

            if (Mouse::getPosition().y<LastY && LastY>SizeY*85/100-birdSize/2)
                Mouse::setPosition(Vector2i(Mouse::getPosition().x,SizeY*85/100-birdSize/2));
        }
        else window.setMouseCursorVisible(true);


        LastX=Mouse::getPosition().x;
        LastY=Mouse::getPosition().y;
        Sleep(33);
        if (currentScreen == INGAME)
        {
            ticks++;
            if (ticks==31) {ticks=1; secs++;}
            int del;
            if (secs<=10) del=7;
            else if (secs<=30) del=6;
            else if (secs<=50) del=5;
            else if (secs<=80) del=4;
            else if (secs<=120) del=3;
            else if (secs<=180) del=2;
            else del=1;
            if (ticks%del==0) GenerateObject(rand()%7+8,rand()%41+80);

            cout<<objects.size()<<endl;
            for (int i=0;i<objects.size();i++)
            {
                if (ShownX+birdSize/2>=objects[i].center.x-objects[i].radius && ShownX+birdSize/2<=objects[i].center.x+objects[i].radius && ShownY+birdSize/2>=objects[i].center.y-objects[i].radius && ShownY+birdSize/2<=objects[i].center.y+objects[i].radius)
                    currentScreen = AFTERGAME;
                if (ShownX+birdSize/2>=objects[i].center.x-objects[i].radius && ShownX+birdSize/2<=objects[i].center.x+objects[i].radius && ShownY-birdSize/2>=objects[i].center.y-objects[i].radius && ShownY-birdSize/2<=objects[i].center.y+objects[i].radius)
                    currentScreen = AFTERGAME;
                if (ShownX-birdSize/2>=objects[i].center.x-objects[i].radius && ShownX-birdSize/2<=objects[i].center.x+objects[i].radius && ShownY-birdSize/2>=objects[i].center.y-objects[i].radius && ShownY-birdSize/2<=objects[i].center.y+objects[i].radius)
                    currentScreen = AFTERGAME;
                if (ShownX-birdSize/2>=objects[i].center.x-objects[i].radius && ShownX-birdSize/2<=objects[i].center.x+objects[i].radius && ShownY+birdSize/2>=objects[i].center.y-objects[i].radius && ShownY+birdSize/2<=objects[i].center.y+objects[i].radius)
                    currentScreen = AFTERGAME;


                if (objects[i].motion == DOWN) objects[i].center.y+=objects[i].speed;
                if (objects[i].motion == UP) objects[i].center.y-=objects[i].speed;
                if (objects[i].motion == LEFT) objects[i].center.x-=objects[i].speed;
                if (objects[i].motion == RIGHT) objects[i].center.x+=objects[i].speed;

                if (objects[i].center.x+objects[i].radius<0 || objects[i].center.x-objects[i].radius>SizeX || objects[i].center.y+objects[i].radius<0 || objects[i].center.y-objects[i].radius>SizeY)
                {
                    vector<object> update;
                    for (int j=0;j<objects.size();j++)
                    {
                        if (i==j) continue;
                        update.push_back(objects[j]);
                    }
                    objects=update;
                    i--;
                }
            }
        }
    }
    return 0;
}
