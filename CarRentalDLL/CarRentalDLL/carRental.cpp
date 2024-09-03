#include "pch.h" 
#include "carRental.h"

CAR_RENTAL_ERRORS readFileAndPutToList(string filename, carTypes ct)
{
    list<string> theList;
    theList.clear();

    ifstream file(filename);
    string line;

    if (file.is_open())
    {
        while (getline(file, line))
            theList.push_back(line);

        file.close();

        switch (ct)
        {
        case carTypes::PREMIUM:
            PremiumCars = theList;
            break;
        case carTypes::SUV:
            SUVCars = theList;
            break;
        case carTypes::SMALL:
            SmallCars = theList;
            break;
        }
        return CAR_RENTAL_ERRORS::NoError;
    }
    else
        return CAR_RENTAL_ERRORS::MissingFile;
}

vector<string> split(string inputString, char delimiter)
{
    stringstream ss(inputString);
    string token;
    vector<string> tokens;

    while (getline(ss, token, delimiter))
        tokens.push_back(token);

    return tokens;
}

carTypes getCarType(string car)
{
    if (find(PremiumCars.begin(), PremiumCars.end(), car) != PremiumCars.end())
        return carTypes::PREMIUM;
    else if (find(SUVCars.begin(), SUVCars.end(), car) != SUVCars.end())
        return carTypes::SUV;
    else if (find(SmallCars.begin(), SmallCars.end(), car) != SmallCars.end())
        return carTypes::SMALL;
    else
        return carTypes::NONE;
}

bool makeListOfCars()
{
    listOfCars.clear();

    ifstream file("cars.txt");
    string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            vector<string> str = split(line, ',');
            if (str.size() != 2)
                return false;
            else
            {
                cars* newCar = new cars();
                newCar->name = str[0];
                newCar->ammount = stoi(str[1]);
                newCar->type = getCarType(str[0]);

                listOfCars.push_back(*newCar);
            }
        }
    }
    file.close();
    return true;
}

int getLoyaltyPoints(carTypes ct)
{
    if (ct == carTypes::PREMIUM)
        return 5;
    if (ct == carTypes::SUV)
        return 3;
    else
        return 1;
}

int rentACar(string nameClient, const initializer_list<renting>& il)
{
    string ClientName = nameClient;
    int cost = 0;
    for (auto x : il)
    {
        string name = x.name;
        int days = x.days;

        carTypes ct = getCarType(name);
        if (ct == carTypes::PREMIUM)
            cost += PremiumPrice * days;
        else if (ct == carTypes::SUV)
        {
            int firstRange, secondRange, thirdRange;
            firstRange = 0;
            secondRange = 0;
            thirdRange = 0;

            if (days <= 7)
                firstRange = days;
            else if (days <= 30)
            {
                secondRange = days - 7;
                firstRange = 7;
            }
            else
            {
                thirdRange = days - 30;
                secondRange = 22;
                firstRange = 7;
            }
            cost +=
                (int)((float)SuvPrice * 0.6f) * thirdRange +
                (int)((float)SuvPrice * 0.8f) * secondRange +
                (int)((float)SuvPrice) * firstRange
                ;
        }
        else if (ct == carTypes::SMALL)
        {
            int firstRange, secondRange;
            firstRange = 0;
            secondRange = 0;

            if (days <= 7)
                firstRange = days;
            else
            {
                secondRange = days - 7;
                firstRange = 7;
            }
            cost +=
                (int)((float)SmallPrice * 0.6f) * secondRange +
                (int)((float)SmallPrice) * firstRange
                ;
        }

        //cost has been calculated, now update the list of cars!
        bool found = false;
        int removeIndex = -1;
        list<cars>::iterator it = listOfCars.begin();
        for (int i = 0; i < listOfCars.size(); i++)
        {
            cars car = *it;
            if (car.name == name)
            {
                found = true;
                car.ammount--;
                if (car.ammount == 0)
                    removeIndex = i;
                *it = car;

                break;
            }
            ++it;
        }

        if (removeIndex != -1)
            listOfCars.erase(std::next(listOfCars.begin(), removeIndex));

        //update list of rented cars................
        if (found)
        {
            rentedCars* rc = new rentedCars();
            rc->nameCar = name;
            rc->nameClient = ClientName;

            Date* date1 = new Date(1, 1, 2000);//Safe assumption: No cars rented after this date :)

            SYSTEMTIME st;
            GetSystemTime(&st);

            Date* date2 = new Date(st.wDay, st.wMonth, st.wYear);

            rc->dayNumber = getDifference(*date1, *date2);

            listOfRentedCars.push_back(*rc);
        }
        else
            return -1;

        //update list of loyalty points
        found = false;

        list<clients>::iterator it2 = listOfClientloyaltyPoints.begin();
        for (int i = 0; i < listOfClientloyaltyPoints.size(); i++)
        {
            clients client = *it2;

            if (client.name == ClientName)
            {
                found = true;
                client.points += getLoyaltyPoints(ct);
                *it2 = client;
                break;
            }
            ++it2;
        }
        if (found == false)
        {
            clients* newClient = new clients();
            newClient->name = ClientName;
            newClient->points = getLoyaltyPoints(ct);

            listOfClientloyaltyPoints.push_back(*newClient);
        }
    }
    return cost;
}

int returnACar(string nameClient, string carName, Date* newDate)
{
    int extraDays = 0;
    bool found = false;
    list<rentedCars>::iterator it = listOfRentedCars.begin();
    for (int i = 0; i < listOfRentedCars.size(); i++)
    {
        rentedCars rentedCar = *it;
        if (rentedCar.nameCar == carName && rentedCar.nameClient == nameClient)
        {
            found = true;
            Date* date1 = new Date(1, 1, 2000);
            SYSTEMTIME st;
            GetSystemTime(&st);
            Date* date2 = new Date(st.wDay, st.wMonth, st.wYear);

            if (newDate != NULL)
                date2 = newDate;

            int diff = getDifference(*date1, *date2);
            extraDays = diff - rentedCar.dayNumber;
            break;
        }
        ++it;
    }

    if (found == false)
        return -1;

    //The car exists, now I have to update both lists of rentedCars (listOfRentedCars) 
    //and the total list of cars (listOfCars)

    //1) First, update the list of cars (listOfCars)
    found = false;
    list<cars>::iterator it2 = listOfCars.begin();
    for (int i = 0; i < listOfCars.size(); i++)
    {
        cars car = *it2;
        if (car.name == carName)
        {
            found = true;
            car.ammount++;
            *it2 = car;
            break;
        }
        ++it2;
    }
    if (found == false)
    {
        cars* newCar = new cars();
        newCar->ammount = 1;
        newCar->name = carName;
        newCar->type = getCarType(carName);
        listOfCars.push_back(*newCar);
    }
    
    //2) Second, update the list of RentedCars (listOfRentedCars)
    found = false;
    int removeIndex = -1;
    list<rentedCars>::iterator it3 = listOfRentedCars.begin();
    for (int i = 0; i < listOfRentedCars.size(); i++)
    {
        rentedCars car = *it3;
        if (car.nameCar == carName && car.nameClient == nameClient) 
        {
            found = true;
            removeIndex = i;
            break;
        }
        ++it;
    }

    if (found == true)
        listOfRentedCars.erase(std::next(listOfRentedCars.begin(), removeIndex));

    carTypes ct = getCarType(carName);
    float fact;
    if (ct == carTypes::PREMIUM)
        fact = PremiumPrice + (float)PremiumPrice * 0.2f;
    else if (ct == carTypes::SUV)
        fact = SuvPrice + (float)SuvPrice * 0.6f;
    else if (ct == carTypes::SMALL)
        fact = SmallPrice + (float)SmallPrice * 0.3f;

    return (int)((float)extraDays * fact);
}

bool initialize()
{
    CAR_RENTAL_ERRORS premiumCarsFile = readFileAndPutToList("PremiunCars.txt", carTypes::PREMIUM);
    CAR_RENTAL_ERRORS SuvCarsFile = readFileAndPutToList("SUVCars.txt", carTypes::SUV);
    CAR_RENTAL_ERRORS SmallCarsFile = readFileAndPutToList("SmallCars.txt", carTypes::SMALL);
    bool k = makeListOfCars();

    if (premiumCarsFile == CAR_RENTAL_ERRORS::NoError && SuvCarsFile == CAR_RENTAL_ERRORS::NoError && SmallCarsFile == CAR_RENTAL_ERRORS::NoError && k == true)
        return true;
    else
        return false;
}

int getAvailableCars(string name)
{
    int ret = 0;
    list<cars>::iterator it = listOfCars.begin();
    for (int i = 0; i < listOfCars.size(); i++)
    {
        cars car = *it;
        if (car.name == name)
            ret += car.ammount;

        ++it;
    }
    return ret;
}

vector<string> getRentedCarsBy(string name)
{
    vector<string> returnVector;

    list<rentedCars>::iterator it = listOfRentedCars.begin();
    for (int i = 0; i < listOfRentedCars.size(); i++)
    {
        rentedCars rentedCar = *it;
        if (rentedCar.nameClient == name)
            returnVector.push_back(rentedCar.nameCar);

        ++it;
    }

    return returnVector;
}