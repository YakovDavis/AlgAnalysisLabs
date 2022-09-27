#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab1-Hammurabi/Lab1-Hammurabi.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Lab1UnitTests
{
	TEST_CLASS(Lab1UnitTests)
	{
	public:
		TEST_METHOD_INITIALIZE(TestInit)
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
            eatingBudget = 0;
		}
        TEST_METHOD_CLEANUP(TestClean)
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
            eatingBudget = 0;
        }
		TEST_METHOD(TestPopulationDying)
		{
            std::random_device rndDevice;
            std::default_random_engine rndEngine(rndDevice());
            eatingBudget = 1000;
            turn(rndEngine);
            Assert::AreEqual(diedLastRound, 50);
		}
        TEST_METHOD(TestPopulationBalance)
        {
            std::random_device rndDevice;
            std::default_random_engine rndEngine(rndDevice());
            eatingBudget = 1000;
            wheatPerAcreLastRound = 6;
            turn(rndEngine);
            Assert::AreEqual(population, 74);
        }
        TEST_METHOD(TestAcresWheatGrowing)
        {
            std::random_device rndDevice;
            std::default_random_engine rndEngine(rndDevice());
            sowedArea = 300;
            wheatPerAcreLastRound = 6;
            eatingBudget = 600;
            turn(rndEngine);
            Assert::AreEqual(wheat + wheatDestroyedByRats, 4000);
        }
        TEST_METHOD(TestSaveLoad)
        {
            std::random_device rndDevice;
            std::default_random_engine rndEngine(rndDevice());
            std::uniform_int_distribution<int> testDist(1, 20);
            int t_year = testDist(rndEngine);
            int t_population = testDist(rndEngine);
            int t_wheat = testDist(rndEngine);
            int t_area = testDist(rndEngine);
            int t_diedLastRound = testDist(rndEngine);
            int t_arrivedLastRound = testDist(rndEngine);
            bool t_plagueLastRound = true;
            int t_wheatCollectedLastRound = testDist(rndEngine);
            int t_wheatPerAcreLastRound = testDist(rndEngine);
            int t_wheatDestroyedByRats = testDist(rndEngine);
            int t_acrePrice = testDist(rndEngine);
            int t_sowedArea = testDist(rndEngine);
            int t_diedOverall = testDist(rndEngine);
            int t_eatingBudget = testDist(rndEngine);
            year = t_year;
            population = t_population;
            wheat = t_wheat;
            area = t_area;
            diedLastRound = t_diedLastRound;
            arrivedLastRound = t_arrivedLastRound;
            plagueLastRound = t_plagueLastRound;
            wheatCollectedLastRound = t_wheatCollectedLastRound;
            wheatPerAcreLastRound = t_wheatPerAcreLastRound;
            wheatDestroyedByRats = t_wheatDestroyedByRats;
            acrePrice = t_acrePrice;
            sowedArea = t_sowedArea;
            diedOverall = t_diedOverall;
            eatingBudget = t_eatingBudget;
            save_game();
            year = 0;
            population = 0;
            wheat = 0;
            area = 0;
            diedLastRound = 0;
            arrivedLastRound = 0;
            plagueLastRound = false;
            wheatCollectedLastRound = 0;
            wheatPerAcreLastRound = 0;
            wheatDestroyedByRats = 0;
            acrePrice = 0;
            sowedArea = 0;
            diedOverall = 0;
            eatingBudget = 0;
            load_game();
            Assert::AreEqual(year, t_year);
            Assert::AreEqual(population, t_population);
            Assert::AreEqual(wheat, t_wheat);
            Assert::AreEqual(area, t_area);
            Assert::AreEqual(diedLastRound, t_diedLastRound);
            Assert::AreEqual(arrivedLastRound, t_arrivedLastRound);
            Assert::AreEqual(plagueLastRound, t_plagueLastRound);
            Assert::AreEqual(wheatCollectedLastRound, t_wheatCollectedLastRound);
            Assert::AreEqual(wheatPerAcreLastRound, t_wheatPerAcreLastRound);
            Assert::AreEqual(wheatDestroyedByRats, t_wheatDestroyedByRats);
            Assert::AreEqual(acrePrice, t_acrePrice);
            Assert::AreEqual(sowedArea, t_sowedArea);
            Assert::AreEqual(diedOverall, t_diedOverall);
            Assert::AreEqual(eatingBudget, t_eatingBudget);
        }
	};
}
