// Lab1-Hammurabi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <locale.h>
#include <utility>
#include <stdexcept>

#define DEBUG

constexpr int kPopulationWheatConsumption = 20;
constexpr int kMaxYears = 10;
constexpr char kSaveFileName[] = "save.dat";

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
    std::uniform_int_distribution<int> acreDist(17, 26);
    std::uniform_int_distribution<int> wheatPerAcreDist(1, 6);
    std::uniform_int_distribution<int> plagueDist(1, 20);

    if (save_exists())
    {
        std::cout << "���������� ����������� ����. ���������� ��? (y/n) ";
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
        plagueLastRound = (plagueDist(rndEngine) > 3 ? false : true);
        int populationBackup = population;
        turn(rndEngine);
#ifdef DEBUG
        std::cout << "[DEBUG] ��������� �� ����: " << populationBackup << "; ��������� �����: " << population << "; �������: " << diedLastRound << "; ���� ��������: " << 1.0f * diedLastRound / populationBackup << std::endl;
#endif // DEBUG

        if (1.0f * diedLastRound / populationBackup > 0.45f)
        {
            successFlag = false;
            break;
        }
        acrePrice = acreDist(rndEngine);
        display_info();
        save_game();
        std::cout << "���� ���������. ������ ���������? (y/n) ";
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
            std::cout << "�� ������ ������� ������� ����� �����. ���� ��������.";
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
    //acrePrice = 0;
    sowedArea = 0;
    diedOverall = 0;

    std::cout << "��� ����������, �������� �������� ���� � ��������� ��� �� ������ ������ ����������� ���������" << std::endl;
    std::cout << "\t��������� ������ ������ ���������� " << population << " �������;" << std::endl <<
        "\t������ � ��� " << wheat << " ������ ������� � �������;" << std::endl <<
        "\t����� ������ �������� " << area << " �����;" << std::endl <<
        "\t1 ��� ����� ����� ������ " << acrePrice << " ������." << std::endl;
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
    std::cout << "��� ���������, ����������?" << std::endl;
    int buyInput = 0;
    int sellInput = 0;
    int eatInput = 0;
    int sowInput = 0;
    while (true)
    {
        std::cout << "������� ����� ����� ����������� ������? ";
        buyInput = get_input_int();
        std::cout << "������� ����� ����� ����������� �������? ";
        sellInput = get_input_int();
        std::cout << "������� ������� ������� ����������� ������? ";
        eatInput = get_input_int();
        std::cout << "������� ����� ����� ����������� �������? ";
        sowInput = get_input_int();

#ifdef DEBUG
        std::cout << "[DEBUG] ���������� ������� ����� ��������:" << wheat - (buyInput - sellInput) * acrePrice - eatInput - ceil(sowInput * 0.5f) << std::endl;
#endif // DEBUG

        if (wheat - (buyInput - sellInput) * acrePrice - eatInput - ceil(sowInput * 0.5f) > 0 && sowInput <= 10 * population && area >= sowInput)
            break;
        std::cout << "�, ����������, ������ ���! � ��� ������ " << population << " �������, " << wheat << " ������� ������� � " << area << " ����� �����!" << std::endl;
    }
    area += buyInput - sellInput;
    wheat -= (buyInput - sellInput) * acrePrice + eatInput;
    sowedArea = sowInput;
    eatingBudget = eatInput;
}

void turn(std::default_random_engine rndEngine_)
{
    wheatCollectedLastRound = wheatPerAcreLastRound * sowedArea;
    wheat += wheatCollectedLastRound;
    std::uniform_int_distribution<int> ratDist(0, round(0.07f * wheat));
    wheatDestroyedByRats = ratDist(rndEngine_);
    wheat -= wheatDestroyedByRats;
    wheat -= eatingBudget;
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

    if (p > 0.33f && l < 7)
        std::cout << "��-�� ����� ���������������� � ����������, ����� ������� ����, � ������ ��� �� ������.������ �� ��������� ������� ������ ������������� � ��������" << std::endl;
    else if (p > 0.10f && l < 9)
        std::cout << "�� ������� �������� �����, ������� ������ � ����� ��������.����� �������� � �����������, � ����� ������ �� ������ ������ ��� ����������" << std::endl;
    else if (p > 0.03f && l < 10)
        std::cout << "�� ���������� ������ �������, � ���, �������, ���� ���������������, �� ������ ������ �� ������� ��� �� ����� ������ �����" << std::endl;
    else
        std::cout << "����������! ���� �������, �������� � ���������� ������ �� ���������� �� �����." << std::endl;
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
            std::cout << "������� �����." << std::endl;
        }
        catch (std::out_of_range const& ex)
        {
            std::cout << "������� ����� ����������� ���������." << std::endl;
        }
        if (output < 0)
            std::cout << "������� ��������������� �����." << std::endl;
    } while (output < 0);
    return output;
}

void display_info()
{
    std::cout << "��� ����������, �������� �������� ����" << std::endl <<
        "\t� ���� " << year << " ������ ����������� ���������" << std::endl;
    if (diedLastRound > 0)
    {
        std::cout << "\t" << diedLastRound << " ������� ������ � ������";
        if (arrivedLastRound > 0)
            std::cout << ", � ";
    }
    if (arrivedLastRound > 0)
        std::cout << "\t" << arrivedLastRound << " ������� ������� � ��� ������� �����";
    if (diedLastRound > 0 || arrivedLastRound > 0)
        std::cout << ";" << std::endl;
    if (plagueLastRound)
        std::cout << "\t���� ���������� �������� ���������;" << std::endl;
    std::cout << "\t��������� ������ ������ ���������� " << population << " �������;" << std::endl <<
        "\t�� ������� " << wheatCollectedLastRound << " ������� �������, �� " << wheatPerAcreLastRound << " ������ � ����;" << std::endl <<
        "\t����� ��������� " << wheatDestroyedByRats << " ������� �������, ������� " << wheat << " ������ � �������;" << std::endl <<
        "\t����� ������ �������� " << area << " �����;" << std::endl <<
        "\t1 ��� ����� ����� ������ " << acrePrice << " ������." << std::endl;
}