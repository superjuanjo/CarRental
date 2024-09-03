
#include <windows.h>
#include <iostream>
#include "carRental.h"

using namespace std;

//Just a main function to test the functionality in the DLL project
int main()
{

    //init function... read text files and populates lists...
    bool k = initialize();

    if (k == true)//initialize is ok
    {
        int costOfRenting = rentACar(
            "Client name 1",//name of the client
            {
                //each client can rent many cars... for each entry, specify the car name and the ammount of days
                renting("BMW 7", 2),
                renting("KIA Sorrento", 4)
            }
        );
        if (costOfRenting == -1)
        {
            wcout << L"Error trying to rent a car !!";
        }

        //gets the cars rented by a client
        vector<string> mycars = getRentedCarsBy("Client name 1");

        //returning a car:
        //the function has 3 parameters, the third one is optional: If it not set, it will take the current date
        int extraCost = returnACar("Client name 1", "BMW 7", new Date(3, 9, 2024));
        if (extraCost == -1)
        {
            wcout << L"Error trying to return a car !!";
        }

        //gets the available cars, given its name
        int x = getAvailableCars("BMW 7");
        int z = getAvailableCars("Seat Ibiza");
        wcout << L"There are " << x << " BMW 7 and " << z << " Seat Ibiza availables...";


        mycars = getRentedCarsBy("Client name 1");

        return 0;
    }
    else
        return -1;
}