// Lab1-Hammurabi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <locale.h>
#include <utility>
#include <stdexcept>

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

    new_game();
    save_game();
    load_game();
    bool successFlag = true;

    while (year < kMaxYears)
    {
        year++;
        wheatPerAcreLastRound = wheatPerAcreDist(rndEngine);
        acrePrice = acreDist(rndEngine);
        plagueLastRound = (plagueDist(rndEngine) > 3 ? false : true);
        int populationBackup = population;
        turn(rndEngine);
        if (1.0f * diedLastRound / populationBackup > 0.45f)
        {
            successFlag = false;
            break;
        }
        display_info();
        get_player_input();
    }
    if (successFlag)
    {
        display_score();
    }
    else
        std::cout << "�� ���������!";
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
    sowedArea = 0;
    diedOverall = 0;
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
        if (wheat - (buyInput - sellInput) * acrePrice - eatInput - ceil(sowInput * 0.5f) > 0 && eatInput >= population * kPopulationWheatConsumption && sowInput <= 10 * population && area >= sowInput)
            break;
        std::cout << "�, ����������, ������ ���! � ��� ������ " << population << " �������, " << wheat << " ������� ������� � " << area << " ����� �����!" << std::endl;
    }
    wheat -= (buyInput - sellInput) * acrePrice + eatInput;
    sowedArea = sowInput;
}

void turn(std::default_random_engine rndEngine_)
{
    wheat += wheatPerAcreLastRound * sowedArea;
    std::uniform_int_distribution<int> ratDist(0, round(0.07f * wheat));
    wheatDestroyedByRats = ratDist(rndEngine_);
    wheat -= wheatDestroyedByRats;
    if (wheat >= population * kPopulationWheatConsumption)
    {
        diedLastRound = 0;
        wheat -= population * kPopulationWheatConsumption;
    }
    else
    {
        diedLastRound = population - wheat / (population * kPopulationWheatConsumption);
        population -= diedLastRound;
        wheat = 0;
    }
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
        //if (output > highLimit_)
        //    std::cout << "�, ����������, ������ ���! � ��� ������ " << population << " �������, " << wheat << " ������� ������� � " << area << " ����� �����!" << std::endl;
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
        std::cout << arrivedLastRound << " ������� ������� � ��� ������� �����";
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