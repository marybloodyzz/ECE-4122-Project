/*
Author: Lizhe Zhang
Class: ECE4122 Last Date Modified: OCT 26
Description:
 function cpp file for lab3
*/

#include "Game.hpp"
#include <random>

//function to draw the map
void draw_map(std::array<std::string, MAP_HEIGHT> map_sketch,  sf::RenderWindow& window)
{
    //create coins and powerup shapes
    sf::CircleShape powerUp(5);
    powerUp.setFillColor(sf::Color::White);
    sf::CircleShape coin(2);
    coin.setFillColor(sf::Color::White);
    //draw the map based on map_sketch
    for (int i = 0; i < MAP_WIDTH; i++)
        {
            for (int j = 0; j < MAP_HEIGHT; j++)
            {
                switch (map_sketch[j][i])
                {
                        
                    case '.':
                    {
                        coin.setPosition(36 + 20.76 * i - coin.getRadius(), 32 + j * 20.76 - coin.getRadius());
                        window.draw(coin);
                        break;
                    }
                        
                    case 'o':
                    {
                        powerUp.setPosition(36 + 20.76 * i - powerUp.getRadius(), 32 + j * 20.76 - powerUp.getRadius());
                        window.draw(powerUp);
                        break;
                    }
                }
            }
        }
}

//fucntion to get current coordinate of the pacman
void getCoord(std::array<std::string, MAP_HEIGHT> map_sketch, sf::Sprite& sprite, int& i, int& j)
{
    sf::Vector2f position(sprite.getPosition());
    i = static_cast<int>(round((position.x - 36.0 + 15) / 20.7));
    j = static_cast<int>(round((position.y - 32.0 + 15) / 20.7));
//    std::cout << i << " " << position.y << std::endl;
}

//fucntion to get current coordinate of the ghost
void getGCoord(std::array<std::string, MAP_HEIGHT> map_sketch, sf::Sprite& sprite, int& i, int& j)
{
    sf::Vector2f position(sprite.getPosition());
    i = static_cast<int>(round((position.x - 36.0 + 13) / 20.7));
    j = static_cast<int>(round((position.y - 32.0 + 10) / 20.7));
}

void setPacDir(sf::Event& event, int& desiredDir)
{
//    if(pacDir != prevDir)
//    {
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
        {
            desiredDir = 0;
        }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
    {
        desiredDir = 1;
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
    {
        desiredDir = 2;
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
    {
        desiredDir = 3;
    }
//    }
}

//fucntion to move the pacman based on user input
void pacMove(std::array<std::string, MAP_HEIGHT> &map_sketch, sf::Sprite& pSprite, float& PPX, float& PPY, int& coin, int& live, bool& powerUp, int& pacDir, int& desiredDir)
{
    int a, b;
    if(desiredDir == 0)
    {
        PPY -= 20.76;
        pSprite.setPosition(PPX, PPY);
        getCoord(map_sketch, pSprite, a, b);
        if(map_sketch[b][a] == '#')
        {
            PPY += 20.76;
            desiredDir = pacDir;
        }
        else if(map_sketch[b][a] == '.')
        {
            coin++;
            map_sketch[b][a] = ' ';
            pacDir = desiredDir;
        }
        else if(map_sketch[b][a] == 'o')
        {
            map_sketch[b][a] = ' ';
            powerUp = true;
            pacDir = desiredDir;
        }
        else
        {
            pacDir = desiredDir;
        }
    }

    if(desiredDir == 1)
    {
        PPY += 20.76;
        pSprite.setPosition(PPX, PPY);
        getCoord(map_sketch, pSprite, a, b);
        if(map_sketch[b][a] == '#')
        {
            PPY-= 20.76;
            desiredDir = pacDir;
        }
        else if(map_sketch[b][a] == '.')
        {
            coin++;
            map_sketch[b][a] = ' ';
            pacDir = desiredDir;
        }
        else if(map_sketch[b][a] == 'o')
        {
            map_sketch[b][a] = ' ';
            powerUp = true;
            pacDir = desiredDir;
        }
        else
        {
            pacDir = desiredDir;
        }
    }

    if(desiredDir == 2)
    {
        PPX -= 20.76;
        pSprite.setPosition(PPX, PPY);
        getCoord(map_sketch, pSprite, a, b);
        if(a <= 0 && b == 14)
        {
            PPX += 20.76 * 27;
        }
        if(map_sketch[b][a] == '#')
        {
            PPX += 20.76;
            desiredDir = pacDir;
        }
        else if(map_sketch[b][a] == '.')
        {
            coin++;
            map_sketch[b][a] = ' ';
            pacDir = desiredDir;
        }
        else if(map_sketch[b][a] == 'o')
        {
            map_sketch[b][a] = ' ';
            powerUp = true;
            pacDir = desiredDir;
        }
        else
        {
            pacDir = desiredDir;
        }
    }

    if(desiredDir == 3)
    {
        PPX += 20.76;
        pSprite.setPosition(PPX, PPY);
        getCoord(map_sketch, pSprite, a, b);
        if(a >= 28 && b == 14)
        {
            PPX -= 20.76 * 28;
        }
        if(map_sketch[b][a] == '#')
        {
            PPX -= 20.76;
            desiredDir = pacDir;
        }
        else if(map_sketch[b][a] == '.')
        {
            coin++;
            pacDir = desiredDir;
            map_sketch[b][a] = ' ';
        }
        else if(map_sketch[b][a] == 'o')
        {
            map_sketch[b][a] = ' ';
            pacDir = desiredDir;
            powerUp = true;
        }
        else
        {
            pacDir = desiredDir;
        }
    }
}

//function to randomly move the ghost
void ghostMove(std::array<std::string, MAP_HEIGHT> map_sketch, sf::Sprite& pSprite, float& PX, float& PY, bool& MoveLock)
{
    //movelock to limit the move frequency of the ghost
    if(!MoveLock)
    {
        int a, b;
        int dir = rand() % 4;
        //0 = down, 1 = up, 2 = left, and 3 = right
        if(dir == 0)
        {
            PY += 20.76;
            pSprite.setPosition(PX, PY);
            getGCoord(map_sketch, pSprite, a, b);
            if(map_sketch[b][a] == '#')
            {
                PY -= 20.76;
            }
            else
            {
                MoveLock = true;
            }
        }
        else if(dir == 1)
        {
            PY -= 20.76;
            pSprite.setPosition(PX, PY);
            getGCoord(map_sketch, pSprite, a, b);
            if(map_sketch[b][a] == '#')
            {
                PY += 20.76;
            }
            else
            {
                MoveLock = true;
            }
        }
        else if(dir == 2)
        {
            PX -= 20.76;
            pSprite.setPosition(PX, PY);
            getGCoord(map_sketch, pSprite, a, b);
            if(a == -1 && b == 14)
            {
                PX += 20.76 * 27;
            }
            if(map_sketch[b][a] == '#')
            {
                PX += 20.76;
            }
            else
            {
                MoveLock = true;
            }
        }
        else
        {
            PX += 20.76;
            pSprite.setPosition(PX, PY);
            getGCoord(map_sketch, pSprite, a, b);
            if(a >= 28 && b == 14)
            {
                PX -= 20.76 * 28;
            }
            if(map_sketch[b][a] == '#')
            {
                PX -= 20.76;
            }
            else
            {
                MoveLock = true;
            }
        }
    }
}

//function to check if the ghost is still alive
void check_gAlive(float& PPX, float& PPY, float& PGX, float& PGY, bool& gAlive, bool& powerUp, int& live)
{
    if ((pow((PPX - PGX), 2) + pow((PPY - PGY), 2)) < 100)
    {
        if(powerUp)
        {
            PGX = 0;
            PGY = 0;
            gAlive = false;
        }
        else
        {
            live = 0;
        }
    }
}
