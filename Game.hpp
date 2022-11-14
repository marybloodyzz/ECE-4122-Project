/*
Author: Lizhe Zhang
Class: ECE4122 Last Date Modified: OCT 26
Description:
 function header file for lab3
*/




#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>


const int MAP_HEIGHT = 31;
const int MAP_WIDTH = 28;
void draw_map(std::array<std::string, MAP_HEIGHT> map_sketch,  sf::RenderWindow& window);
void getCoord(std::array<std::string, MAP_HEIGHT> map_sketch, sf::Sprite& sprite, int& i, int& j);
void pacMove(std::array<std::string, MAP_HEIGHT> &map_sketch, sf::Sprite& pSprite, float& PPX, float& PPY, int& coin, int& live, bool& powerUp, int& pacDir, int& desiredDir);
void setPacDir(sf::Event& event, int& desiredDir);
void ghostMove(std::array<std::string, MAP_HEIGHT> map_sketch, sf::Sprite& pSprite, float& PX, float& PY, bool& MoveLock);
void getGCoord(std::array<std::string, MAP_HEIGHT> map_sketch, sf::Sprite& sprite, int& i, int& j);
void check_gAlive(float& PPX, float& PPY, float& PGX, float& PGY, bool& gAlive, bool& powerUp, int& live);
