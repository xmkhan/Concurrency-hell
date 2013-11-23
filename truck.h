#ifndef __TRUCK_H__
#define __TRUCK_H__

#include <uC++.h>

_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;
_Task VendingMachine;
_Task Truck;

_Task Truck {
	Printer &prt;
	NameServer &ns;
	BottlingPlant &plant;
	unsigned int numVendingMachines, maxStockPerFlavour, 
	             addToVM, outOfFlavour, totalNotReplenished;
	int curCargoSize;

	VendingMachine **machineList;
	unsigned int *cargo;
	unsigned int *inventory;


	enum States { Starting = 'S', ShipmentPickedUp = 'P', 
	              Deliver = 'd', UnsuccessfulRestock = 'U', 
	              EndDelivery = 'D', Finished = 'F' };
    
    bool restockVendingMachine(unsigned int index);
    void restockFlavour(unsigned int flavour);
    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );

    ~Truck();
};

#endif