// Lab1-Hammurabi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <locale.h>
#include <utility>
#include <stdexcept>

//#define DEBUG

constexpr int kPopulationWheatConsumption = 20;
constexpr int kMaxYears = 10;
constexpr char kSaveFileName[] = "save.dat";
constexpr float kLosingCondition = 0.45f;
constexpr int kStartingPopulation = 100;
constexpr int kStartingArea = 1000;
constexpr int kStartingWheat = 2800;
constexpr float kMaxRatDestructionPercent = 0.07f;
constexpr float kSeedsPerSowedAcre = 0.5f;
constexpr int kPlagueProbNum = 3;
constexpr int kAcresWorkedByPerson = 10;

constexpr int kAcreDistL = 17;
constexpr int kAcreDistU = 26;
constexpr int kWheatPerAcreDistL = 1;
constexpr int kWheatPerAcreDistU = 6;
constexpr int kPlagueDistL = 1;
constexpr int kPlagueDistU = 20;

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
int diedOverall;
int eatingBudget;

void new_game();
void load_game();
void save_game();
bool save_exists();
void turn(std::default_random_engine rndEngine_);
void display_info();
void get_player_input();
int get_input_int();
void display_score();

int main()
{
    setlocale(LC_ALL, "Russian");

    std::random_device rndDevice;
    std::default_random_engine rndEngine(rndDevice());
    std::uniform_int_distribution<int> acreDist(kAcreDistL, kAcreDistU);
    std::uniform_int_distribution<int> wheatPerAcreDist(kWheatPerAcreDistL, kWheatPerAcreDistU);
    std::uniform_int_distribution<int> plagueDist(kPlagueDistL, kPlagueDistU);

    if (save_exists())
    {
        std::cout << "Обнаружена сохраненная игра. Продолжить ее? (y/n) ";
        if (std::cin.get() == 'y')
        {
            load_game();
            display_info();
            get_player_input();
        }
        else
        {
            acrePrice = acreDist(rndEngine);
            new_game();
            get_player_input();
        }
    }
    else
    {
        acrePrice = acreDist(rndEngine);
        new_game();
        get_player_input();
    }

    bool successFlag = true;
    bool exitFlag = false;

    while (year < kMaxYears)
    {
        year++;
        wheatPerAcreLastRound = wheatPerAcreDist(rndEngine);
        plagueLastRound = (plagueDist(rndEngine) > kPlagueProbNum ? false : true);
        int populationBackup = population;
        turn(rndEngine);
#ifdef DEBUG
        std::cout << "[DEBUG] Население до хода: " << populationBackup << "; Население после: " << population << "; Погибло: " << diedLastRound << "; Доля погибших: " << 1.0f * diedLastRound / populationBackup << std::endl;
#endif // DEBUG

        if (1.0f * diedLastRound / populationBackup > kLosingCondition || population <= 0)
        {
            successFlag = false;
            break;
        }
        acrePrice = acreDist(rndEngine);
        display_info();
        save_game();
        std::cout << "Игра сохранена. Хотите продолжть? (y/n) ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (std::cin.get() == 'n')
        {
            exitFlag = true;
            break;
        }
        get_player_input();
    }

    if (!exitFlag)
    {
        if (successFlag)
        {
            display_score();
        }
        else
            std::cout << "Игра окончена. Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города. Теперь вы вынуждены влачить жалкое существование в изгнании.";
    }
}

void new_game()
{
    year = 0;
    population = kStartingPopulation;
    wheat = kStartingWheat;
    area = kStartingArea;
    diedLastRound = 0;
    arrivedLastRound = 0;
    plagueLastRound = false;
    wheatCollectedLastRound = 0;
    wheatPerAcreLastRound = 0;
    wheatDestroyedByRats = 0;
    //acrePrice = 0;
    sowedArea = 0;
    diedOverall = 0;
    eatingBudget = 0;

    std::cout << "Мой повелитель, соизволь поведать тебе о состоянии дел на начало твоего высочайшего правления" << std::endl;
    std::cout << "\tНаселение города сейчас составляет " << population << " человек;" << std::endl <<
        "\tСейчас у нас " << wheat << " бушеля пшеницы в амбарах;" << std::endl <<
        "\tГород сейчас занимает " << area << " акров;" << std::endl <<
        "\t1 акр земли стоит сейчас " << acrePrice << " бушель." << std::endl;
}

void load_game()
{
    std::fstream f;
    f.open(kSaveFileName, std::fstream::in);
    std::string s;
    std::getline(f, s);
    year = std::stoi(s);
    std::getline(f, s);
    population = std::stoi(s);
    std::getline(f, s);
    wheat = std::stoi(s);
    std::getline(f, s);
    area = std::stoi(s);
    std::getline(f, s);
    diedLastRound = std::stoi(s);
    std::getline(f, s);
    arrivedLastRound = std::stoi(s);
    std::getline(f, s);
    plagueLastRound = std::stoi(s);
    std::getline(f, s);
    wheatCollectedLastRound = std::stoi(s);
    std::getline(f, s);
    wheatPerAcreLastRound = std::stoi(s);
    std::getline(f, s);
    wheatDestroyedByRats = std::stoi(s);
    std::getline(f, s);
    acrePrice = std::stoi(s);
    std::getline(f, s);
    sowedArea = std::stoi(s);
    std::getline(f, s);
    diedOverall = std::stoi(s);
    std::getline(f, s);
    eatingBudget = std::stoi(s);
    f.close();
}

void save_game()
{
    std::string s = "";
    s.append(std::to_string(year));
    s.append("\n");
    s.append(std::to_string(population));
    s.append("\n");
    s.append(std::to_string(wheat));
    s.append("\n");
    s.append(std::to_string(area));
    s.append("\n");
    s.append(std::to_string(diedLastRound));
    s.append("\n");
    s.append(std::to_string(arrivedLastRound));
    s.append("\n");
    s.append(std::to_string(plagueLastRound));
    s.append("\n");
    s.append(std::to_string(wheatCollectedLastRound));
    s.append("\n");
    s.append(std::to_string(wheatPerAcreLastRound));
    s.append("\n");
    s.append(std::to_string(wheatDestroyedByRats));
    s.append("\n");
    s.append(std::to_string(acrePrice));
    s.append("\n");
    s.append(std::to_string(sowedArea));
    s.append("\n");
    s.append(std::to_string(diedOverall));
    s.append("\n");
    s.append(std::to_string(eatingBudget));
    s.append("\n");
    std::fstream f;
    f.open(kSaveFileName, std::fstream::out);
    f.write(s.c_str(), s.size());
    f.close();
}

bool save_exists()
{
    std::ifstream sFile(kSaveFileName);
    return sFile.good();
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

#ifdef DEBUG
        std::cout << "[DEBUG] Количество пшеницы после расходов:" << wheat - (buyInput - sellInput) * acrePrice - eatInput - ceil(sowInput * 0.5f) << std::endl;
#endif // DEBUG

        if (wheat - (buyInput - sellInput) * acrePrice - eatInput - ceil(sowInput * kSeedsPerSowedAcre) > 0 && sowInput <= kAcresWorkedByPerson * population && area >= sowInput)
            break;
        std::cout << "О, повелитель, пощади нас! У нас только " << population << " человек, " << wheat << " бушелей пшеницы и " << area << " акров земли!" << std::endl;
    }
    area += buyInput - sellInput;
    wheat -= (buyInput - sellInput) * acrePrice + ceil(sowInput * kSeedsPerSowedAcre);
    sowedArea = sowInput;
    eatingBudget = eatInput;
}

void turn(std::default_random_engine rndEngine_)
{
    wheatCollectedLastRound = wheatPerAcreLastRound * sowedArea;
    wheat += wheatCollectedLastRound;
    wheat -= eatingBudget;
    std::uniform_int_distribution<int> ratDist(0, round(kMaxRatDestructionPercent * wheat));
    wheatDestroyedByRats = ratDist(rndEngine_);
    if (wheatDestroyedByRats > wheat)
        wheatDestroyedByRats = wheat;
    wheat -= wheatDestroyedByRats;
    if (eatingBudget >= population * kPopulationWheatConsumption)
        diedLastRound = 0;
    else
        diedLastRound = population - eatingBudget / kPopulationWheatConsumption;
    population -= diedLastRound;
    arrivedLastRound = diedLastRound / 2 + (5 - wheatPerAcreLastRound) * wheat / 600 + 1;
    if (arrivedLastRound < 0)
        arrivedLastRound = 0;
    population += arrivedLastRound;
    if (plagueLastRound)
        population = population / 2;
}

void display_score()
{
    float p = 1.0f * diedOverall / kMaxYears;
    int l = area / population;

    std::cout << "Итак, 10 лет твоего высочайшего правления подошли к концу. Результат:" << std::endl;

    if (p > 0.33f && l < 7)
        std::cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города.Теперь вы вынуждены влачить жалкое существование в изгнании" << std::endl;
    else if (p > 0.10f && l < 9)
        std::cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному.Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем" << std::endl;
    else if (p > 0.03f && l < 10)
        std::cout << "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова" << std::endl;
    else
        std::cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше." << std::endl;
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
            std::cout << ", и " << arrivedLastRound << " человек прибыли в наш великий город";
    }
    else if (arrivedLastRound > 0)
        std::cout << "\t" << arrivedLastRound << " человек прибыли в наш великий город";
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