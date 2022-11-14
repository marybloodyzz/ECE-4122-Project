/*
Author: Lizhe Zhang
Class: ECE4122 Last Date Modified: OCT 26
Description:
 main file for lab3
*/
#include <iostream>
#include "Game.hpp"
// Here is a small helper for you! Have a look.
const int SCRWIDTH = 640;
const int SCRHEIGHT = 720;
float PPX = 305;
float PPY = 490;
float PRGX = 312, PRGY = 245;
float PBGX = 312, PBGY = 245;
float POGX = 312, POGY = 245;
float PPGX = 312, PPGY = 245;
int pacDir = 3, desiredDir = 5;
bool blgAlive = true, rgAlive = true, ogAlive = true, pgAlive = true;
bool blgMoveLock = false, rgMoveLock = false, ogMoveLock = false, pgMoveLock = false;
int coin = 0;
int live = 1;
bool powerUp = false;
bool pclock = false;
auto ppp = std::chrono::steady_clock::now();
int powerupCD = 5;
bool started = false;
int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "SFML window");

    // Load the starting bg
    sf::Texture start;
    if (!start.loadFromFile("graphics/pac-man.bmp")) {
        return EXIT_FAILURE;
    }
    sf::Sprite bgSprite(start);

    // Load the pacman
    sf::Texture pacman;
    if (!pacman.loadFromFile("graphics/pacman.bmp")) {
        return EXIT_FAILURE;
    }
    sf::Sprite pSprite(pacman);
    pSprite.setPosition(PPX, PPY);
    
    // Load the red ghost
    sf::Texture rGhost;
    if (!rGhost.loadFromFile("graphics/red_ghosts.bmp")) {
        return EXIT_FAILURE;
    }
    sf::Sprite rgSprite(rGhost);
    rgSprite.setPosition(PRGX, PRGY);
    // Load the blue ghost
    sf::Texture bGhost;
    if (!bGhost.loadFromFile("graphics/blue_ghost.bmp")) {
        return EXIT_FAILURE;
    }
    sf::Sprite blgSprite(bGhost);
    blgSprite.setPosition(PBGX, PBGY);
    // Load the orange ghost
    sf::Texture oGhost;
    if (!oGhost.loadFromFile("graphics/orange_ghost.bmp")) {
        return EXIT_FAILURE;
    }
    sf::Sprite ogSprite(oGhost);
    ogSprite.setPosition(POGX, POGY);
    // Load the pink ghost
    sf::Texture pGhost;
    if (!pGhost.loadFromFile("graphics/pink_ghost.bmp")) {
        return EXIT_FAILURE;
    }
    sf::Sprite pgSprite(pGhost);
    pgSprite.setPosition(PPGX, PPGY);
    
    //initilized the array to record output map
    std::array<std::array<int, MAP_HEIGHT>, MAP_WIDTH> output_map{};
    //draw the map to rocord walls, coins and powerups.
    std::array<std::string, MAP_HEIGHT> map_sketch = {
            "############################",
            "#............##............#",
            "#.####.#####.##.#####.####.#",
            "#o####.#####.##.#####.####o#",
            "#.####.#####.##.#####.####.#",
            "#..........................#",
            "#.####.##.########.##.####.#",
            "#.####.##.########.##.####.#",
            "#......##....##....##......#",
            "######.##### ## #####.######",
            "     #.##### ## #####.#     ",
            "     #.##          ##.#     ",
            "     #.## ######## ##.#     ",
            "######.## #      # ##.######",
            "      .   #      #   .      ",
            "######.## #      # ##.######",
            "     #.## ######## ##.#     ",
            "     #.##          ##.#     ",
            "     #.## ######## ##.#     ",
            "######.## ######## ##.######",
            "#............##............#",
            "#.####.#####.##.#####.####.#",
            "#.####.#####.##.#####.####.#",
            "#o..##.......  .......##..o#",
            "###.##.##.########.##.##.###",
            "###.##.##.########.##.##.###",
            "#......##....##....##......#",
            "#.##########.##.##########.#",
            "#.##########.##.##########.#",
            "#..........................#",
            "############################",
        };
    
    // Create a start text to display
    sf::Font font;
    if (!font.loadFromFile("lab3/sansation.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text text("PRESS ENTER TO START", font, 50);
    //centering the text
    text.setFillColor(sf::Color::White);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(SCRWIDTH/2.0f,SCRHEIGHT/2.0f));
    
    //text to display the score
    sf::Text score(std::to_string(coin), font, 30);
    score.setFillColor(sf::Color::White);
    sf::FloatRect scoreRect = score.getLocalBounds();
    score.setPosition(sf::Vector2f(170,673));
    
    //text to display the remaining powerup time
    sf::Text powerUpCD("CD: " + std::to_string(powerupCD), font, 30);
    powerUpCD.setFillColor(sf::Color::White);
    sf::FloatRect powerRect = powerUpCD.getLocalBounds();
    powerUpCD.setPosition(sf::Vector2f(500,673));
    
    //text to display the remaining live
    sf::Text liveRemain(std::to_string(live), font, 30);
    liveRemain.setFillColor(sf::Color::White);
    sf::FloatRect liveRect = liveRemain.getLocalBounds();
    liveRemain.setPosition(sf::Vector2f(430,673));

    auto tbegin = std::chrono::steady_clock::now();
    auto tpbegin = std::chrono::steady_clock::now();
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
            //press enter to start
            if (!started && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
            {
                if (!start.loadFromFile("graphics/maze.bmp"))
                {
                    return EXIT_FAILURE;
                }
                started = true;
                
                text.setString("");
            }
            //if started, then user could begin to control the pacman
            if(started)
            {
                    setPacDir(event, desiredDir);
            }
            
        }
        if(started)
        {
            //series of code to record the remaining time of powerup effect
            if(powerUp && !pclock)
            {
                ppp = std::chrono::steady_clock::now();
                pclock = true;
            }
            auto ppe = std::chrono::steady_clock::now();
            int CD = std::chrono::duration_cast<std::chrono::seconds>(ppe - ppp).count();
            if(pclock && CD > 0)
            {
                powerupCD = 5;
            }
            if (pclock && CD >= 1)
            {
                powerupCD = 4;
            }
            if (pclock && CD >= 2)
            {
                powerupCD = 3;
            }
            if (pclock && CD >= 3)
            {
                powerupCD = 2;
            }
            if (pclock && CD >= 4)
            {
                powerupCD = 1;
            }
            if(pclock && CD >= 5)
            {
                powerUp = false;
                pclock = false;
            }
            
            //move the ghost in a playable speed
            ghostMove(map_sketch, rgSprite, PRGX, PRGY, rgMoveLock);
            ghostMove(map_sketch, blgSprite, PBGX, PBGY, blgMoveLock);
            ghostMove(map_sketch, ogSprite, POGX, POGY, ogMoveLock);
            ghostMove(map_sketch, pgSprite, PPGX, PPGY, pgMoveLock);
        
            //update the position
            pSprite.setPosition(PPX, PPY);
            rgSprite.setPosition(PRGX, PRGY);
            blgSprite.setPosition(PBGX, PBGY);
            ogSprite.setPosition(POGX, POGY);
            pgSprite.setPosition(PPGX, PPGY);

        }
        
        // Clear screen
        window.clear();
        // redraw the sprites
        window.draw(bgSprite);
        //update the map
        if(started)
        {
            //draw the pacman
            window.draw(pSprite);
            //draw the ghost if they are alive

            check_gAlive(PPX, PPY, PRGX, PRGY, rgAlive, powerUp, live);
            check_gAlive(PPX, PPY, PBGX, PBGY, blgAlive, powerUp, live);
            check_gAlive(PPX, PPY, POGX, POGY, ogAlive, powerUp, live);
            check_gAlive(PPX, PPY, PPGX, PPGY, pgAlive, powerUp, live);
            auto tpend = std::chrono::steady_clock::now();
            if(std::chrono::duration_cast<std::chrono::microseconds>(tpend - tpbegin).count() > 100000)
            {
                pacMove(map_sketch, pSprite, PPX, PPY, coin, live, powerUp, pacDir, desiredDir);
                std::cout << live << std::endl;
                tpbegin = std::chrono::steady_clock::now();
                if (live == 0)
                {
                    text.setString("You Lose!");
                    text.setPosition(sf::Vector2f(SCRWIDTH / 1.3f,SCRHEIGHT/2.0f));
                    window.draw(text);
                    window.display();
                    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                    window.close();
                }
            }
            auto tend = std::chrono::steady_clock::now();
            if(std::chrono::duration_cast<std::chrono::microseconds>(tend - tbegin).count() > 100000)
            {
                blgMoveLock = false;
                rgMoveLock = false;
                ogMoveLock = false;
                pgMoveLock = false;
                tbegin = std::chrono::steady_clock::now();
            }
        
            if(rgAlive)
            {
                window.draw(rgSprite);
            }
            if(blgAlive)
            {
                window.draw(blgSprite);
            }
            if(ogAlive)
            {
                window.draw(ogSprite);
            }
            if(pgAlive)
            {
                window.draw(pgSprite);
            }
            if (coin == 240)
            {
                text.setString("You Win!");
                text.setPosition(sf::Vector2f(SCRWIDTH / 1.3f,SCRHEIGHT/2.0f));
                window.draw(text);
                window.display();
                std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                window.close();
            }
            draw_map(map_sketch, window);
            score.setString(std::to_string(coin));
            window.draw(score);
            window.draw(liveRemain);
            if(powerUp)
            {
                powerUpCD.setString("CD: " + std::to_string(powerupCD));
                window.draw(powerUpCD);
            }
            
        }
        // Draw the string
        window.draw(text);
        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}


