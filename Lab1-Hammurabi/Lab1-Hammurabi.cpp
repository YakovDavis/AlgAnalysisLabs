// Lab1-Hammurabi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <random>
#include <iostream>
#include <fstream>

int population;
int wheat;
int area;
int round;
int diedLastRound;
int arrivedLastRound;
bool plagueLastRound;
int wheatCollectedLastRound;
int wheatPerAcreLastRound;
int wheatDestroyedByRats;
int acrePrice;

void new_game(int acrePrice_);
void load_game();
void save_game();
bool save_exists();

int main()
{
    std::random_device rndDevice;
    std::default_random_engine rndEngine(rndDevice());
    std::uniform_int_distribution<int> acreDist(17, 26);
    std::uniform_int_distribution<int> wheatPerAcreDist(1, 6);
    std::uniform_int_distribution<int> wheatPerAcreDist(1, 6);


}

void new_game(int acrePrice_)
{
    population = 100;
    wheat = 2800;
    area = 1000;
    round = 1;
    diedLastRound = 0;
    arrivedLastRound = 0;
    plagueLastRound = false;
    wheatCollectedLastRound = 0;
    wheatPerAcreLastRound = 0;
    wheatDestroyedByRats = 0;
    acrePrice = acrePrice_;
}