El ejercicio está dividio en dos partes:
- CarRentalDLL:
En este proyecto está toda la funcionalidad... Básicamente, crea un fichero llamado "CarRentalDLL.dll", que es el que luego es accedido por otro proyecto (.exe)

- CarRental:
Este es el proyecto que genera un fichero llamado CarRental.exe, que usa el fichero DLL anterior, y que testéa la funcionalidad


Localización de los dos proyectos:
Estos dos proyectos han de estar en el mismo nivel:
En mi caso, la estructura es la siguiente:

c:/Projects
	c:/Projects/CarRentalDLL
	c:/Projects/CarRental

Al compilarse el proyecto DLL, el fichero DLL correspondiente (CarRentalDLL.dll) se copiará automáticamente en el lugar apropiado del proyecto .EXE

- En la misma carpeta donde se situa el fichero CarRental.exe, debe haber 4 ficheros de texto:
* cars.txt:
Este fichero contiene una lista de todos los coches de que se disponen, asi como de su cantidad
BMW 7, 4
KIA Sorrento, 5
Seat Ibiza, 10
Volkswagen Passat, 2
Volvo EX30, 1
Renault Clio, 6
Cada linea define un coche y la cantidad que hay, separados por comas

* PremiunCars.txt:
Este fichero contiene una lista de todos los coches tipo "Premium", por ejemplo:
BMW 7
Volkswagen Passat
Nissan Maxima

* SUVCars.txt:
Lo mismo para coches tipo "SUV"

* SmallCars.txt:
Lo mismo para coches tipo "Small"



////////////////////////////////////////////////////////////////////////////////////////////

Una vez compilado el proyecto .DLL, se puede ejecutar el proyecto de prueba "CarRental"
Básicamente, hay 5 funciones útiles:
- initialize();
Como su nombre indica, inicializa todo, y devuelve un valor bool indicando si se ha inicializado bien o no

- rentACar();
Alquila un coche, y devuelve el coste
Un usuario puede alquilar uno o varios coches:
int costOfRenting = rentACar(
					            "Pepe",//name of the client
            					{
                					renting("BMW 7", 2),
            					}
        					);
En el ejemplo de arriba, el cliente "Pepe" alquila un "BMW 7" por 2 días        	

int costOfRenting = rentACar(
					            "Juan",//name of the client
            					{
                					renting("BMW 7", 2),
                					renting("KIA Sorrento", 4)
            					}
        					);				
En este caso, el cliente "Juan" alquila dos coches: Un BMW 7 por dos días, y un KIA Sorrento por 4 días


- returnACar();
Devuelve un coche:
Esta función tiene dos parámetros obligatorios, y uno opcional

int extraCost = returnACar("Pepe", "BMW 7");
En este caso, "Pepe" devuelve un coche "BMW 7" en la fecha actual

int extraCost = returnACar("Juan", "KIA Sorrento", new Date(3, 9, 2024));
En este caso, "Juan" devolvería un "KIA Sorrento" la fecha del 3 de Septiembre de 2024

En ambos casos, si el valor de retorno es -1, implica un error: Generalmente que el usuario o el coche no existan en la lista de coches alquilados


- getAvailableCars();
Devuelve el numero de coches disponibles de un determinado modelo, por ejemplo:
int x = getAvailableCars("BMW 7");
x tendrá la cantidad de coches "BMW 7" disponibles


- getRentedCarsBy();
Devuelve un vector de strings con todos los coches alquilados por un cliente, por ejemplo:

rentACar("Pepe",//name of the client
            {
                //each client can rent many cars... for each entry, specify the car name and the ammount of days
                renting("BMW 7", 2),
                renting("KIA Sorrento", 4)
            }
        );

Despues de ejecutar la función de arriba, al ejecutar la siguiente:
vector<string> mycars = getRentedCarsBy("Pepe");
el vector "mycars" contendrá dos valores: "BMW 7" y "KIA Sorrento"

y si hacemos
returnACar("Pepe", "BMW 7");

Al hacer de nuevo
mycars = getRentedCarsBy("Pepe");
Esta vez, my cars contendrá solamente un valor: "KIA Sorrento"
