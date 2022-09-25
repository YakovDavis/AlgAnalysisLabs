// Lab1-Hammurabi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <locale.h>
#include <utility>
#include <stdexcept>

int year;
int population;
int wheat;
int area;
int diedLastRound;
int arrivedLastRound;
bool plagueLastRound;
int wheatCollectedLastRound;
int wheatPerAcreLastRound;
int wheatDestroyedByRats;
int acrePrice;
int sowedArea;

void new_game();
void load_game();
void save_game();
bool save_exists();
void turn(std::default_random_engine rndEngine_);
void display_info();
void get_player_input();
int get_input_int();

int main()
{
    setlocale(LC_ALL, "Russian");

    std::random_device rndDevice;
    std::default_random_engine rndEngine(rndDevice());
    std::uniform_int_distribution<int> acreDist(17, 26);
    std::uniform_int_distribution<int> wheatPerAcreDist(1, 6);
    std::uniform_int_distribution<int> plagueDist(1, 20);

    while (year < 10)
    {
        year++;
        wheatPerAcreLastRound = wheatPerAcreDist(rndEngine);
        acrePrice = acreDist(rndEngine);
        plagueLastRound = (plagueDist(rndEngine) > 3 ? false : true);
        turn(rndEngine);
        display_info();
        get_player_input();
    }
}

void new_game()
{
    year = 0;
    population = 100;
    wheat = 2800;
    area = 1000;
    diedLastRound = 0;
    arrivedLastRound = 0;
    plagueLastRound = false;
    wheatCollectedLastRound = 0;
    wheatPerAcreLastRound = 0;
    wheatDestroyedByRats = 0;
    acrePrice = 0;
}

void get_player_input()
{
    std::cout << "Что пожелаешь, повелитель?" << std::endl;
    int buyInput = 0;
    int sellInput = 0;
    int eatInput = 0;
    int sowInput = 0;
    while (true)
    {
        std::cout << "Сколько акров земли повелеваешь купить? ";
        buyInput = get_input_int();
        std::cout << "Сколько акров земли повелеваешь продать? ";
        sellInput = get_input_int();
        std::cout << "Сколько бушелей пшеницы повелеваешь съесть? ";
        eatInput = get_input_int();
        std::cout << "Сколько акров земли повелеваешь засеять? ";
        sowInput = get_input_int();
        if (wheat - (buyInput - sellInput) * acrePrice - eatInput - ceil(sowInput * 0.5) > 0 && eatInput >= population * 20 && sowInput <= 10 * population && area >= sowInput)
            break;
        std::cout << "О, повелитель, пощади нас! У нас только " << population << " человек, " << wheat << " бушелей пшеницы и " << area << " акров земли!" << std::endl;
    }
    wheat -= (buyInput - sellInput) * acrePrice + eatInput;
    sowedArea = sowInput;
}

void turn(std::default_random_engine rndEngine_)
{

}

int get_input_int()
{
    std::string input = "";
    int output = INT32_MIN;
    do
    {
        std::cin >> input;
        try
        {
            output = std::stoi(input);
        }
        catch (std::invalid_argument const& ex)
        {
            std::cout << "Введите число." << std::endl;
        }
        catch (std::out_of_range const& ex)
        {
            std::cout << "Введите число корректного диапазона." << std::endl;
        }
        //if (output > highLimit_)
        //    std::cout << "О, повелитель, пощади нас! У нас только " << population << " человек, " << wheat << " бушелей пшеницы и " << area << " акров земли!" << std::endl;
        if (output < 0)
            std::cout << "Введите неотрицательное число." << std::endl;
    } while (output < 0);
    return output;
}

void display_info()
{
    std::cout << "Мой повелитель, соизволь поведать тебе" << std::endl <<
        "\tв году " << year << " твоего высочайшего правления" << std::endl;
    if (diedLastRound > 0)
    {
        std::cout << "\t" << diedLastRound << " человек умерли с голоду";
        if (arrivedLastRound > 0)
            std::cout << ", и ";
    }
    if (arrivedLastRound > 0)
        std::cout << arrivedLastRound << " человек прибыли в наш великий город";
    if (diedLastRound > 0 || arrivedLastRound > 0)
        std::cout << ";" << std::endl;
    if (plagueLastRound)
        std::cout << "\tЧума уничтожила половину населения;" << std::endl;
    std::cout << "\tНаселение города сейчас составляет " << population << " человек;" << std::endl <<
        "\tМы собрали " << wheatCollectedLastRound << " бушелей пшеницы, по " << wheatPerAcreLastRound << " бушеля с акра;" << std::endl <<
        "\tКрысы истребили " << wheatDestroyedByRats << " бушелей пшеницы, оставив " << wheat << " бушеля в амбарах;" << std::endl <<
        "\tГород сейчас занимает " << area << " акров;" << std::endl <<
        "\t1 акр земли стоит сейчас " << acrePrice << " бушель." << std::endl;
}