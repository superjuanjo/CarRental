#pragma once
#include <fstream> 
#include <sstream>
#include <string> 
#include <list>
#include <vector>
#include "DateHelper.h"

using namespace std;

enum carTypes { PREMIUM, SUV, SMALL, NONE };

enum CAR_RENTAL_ERRORS { NoError, MissingFile };

struct rentedCars
{
    string nameCar;
    string nameClient;
    int dayNumber;
};

struct renting
{
    renting(string _name, int _days)
    {
        name = _name;
        days = _days;
    }
    string name;
    int days;
};

struct clients
{
    string name;
    int points;
};

struct cars
{
    string name;
    carTypes type;
    int ammount;
};

list <rentedCars> listOfRentedCars;
list<clients> listOfClientloyaltyPoints;
list<cars> listOfCars;
list<string> PremiumCars;
list<string> SUVCars;
list<string> SmallCars;

int PremiumPrice = 300;
int SuvPrice = 150;
int SmallPrice = 50;

CAR_RENTAL_ERRORS readFileAndPutToList(string filename, carTypes ct);
vector<string> split(string inputString, char delimiter);
carTypes getCarType(string car);
bool makeListOfCars();
int getLoyaltyPoints(carTypes ct);

#define DllExport   __declspec( dllexport )

extern "C" DllExport int rentACar(string nameClient, const initializer_list<renting>& il);
extern "C" DllExport int returnACar(string nameClient, string carName, Date* newDate = NULL);
extern "C" DllExport bool initialize();
extern "C" DllExport int getAvailableCars(string name);
extern "C" DllExport vector<string> getRentedCarsBy(string name);

