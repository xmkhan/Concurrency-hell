#include "truck.h"
#include "bottlingplant.h"
#include "vendingmachine.h"
#include "printer.h"
#include "nameserver.h"
#include "MPRNG.h"

/**
 * Responsible for distributing soda among
 * vending machines
 */
void Truck::main() {
	bool closing;

	prt.print(Printer::Truck, Starting);
	for( ;; ) {
	    machineList = ns.getMachineList();
	    yield(RNG(1, 10));
	    closing = plant.getShipment(cargo);

	    // if plant is closed, break out of the loop
	    if (closing) break;

	    // get the cargo size
	    curCargoSize = 0;
	    for ( int i = 0; i < numFlavours; i++ ) curCargoSize += cargo[i];
	    prt.print(Printer::Truck, (char)ShipmentPickedUp, curCargoSize);

	    // process vending machines in order
	    // break out of loop if truck runs out of soda
	    for ( unsigned int i = 0; i < numVendingMachines; i++ ) {
	        if (!restockVendingMachine(i)) break;
	    }
   	}
}

/**
 * restock the specified vending machine
 * @param id of the vending machine
 * @return false if truck is out of stock, false otherwise
 */
bool Truck::restockVendingMachine( unsigned int index ) {
	prt.print(Printer::Truck, (char)Deliver,
		machineList[index]->getId(), curCargoSize);

	inventory = machineList[index]->inventory();

	// restock each flavour
	for ( int j = 0; j < numFlavours; j++ ) {
	    restockFlavour(j);
	}

	// failed to restock all flavours
	if ( outOfFlavour > 0 ) {
		prt.print(Printer::Truck, (char)UnsuccessfulRestock,
			machineList[index]->getId(), totalNotReplenished);
	}


	prt.print(Printer::Truck, (char)EndDelivery,
		machineList[index]->getId(), curCargoSize);

    totalNotReplenished = 0;

	// truck goes back to plant if it runs out of soda
	if ( outOfFlavour == numFlavours ) {
        outOfFlavour = 0;
        return false;
    }

	machineList[index]->restocked();
	return true;
}

/**
 * restock the flavour passed in
 * @param flavour flavour to restock
 */
void Truck::restockFlavour ( unsigned int flavour ) {
	// amount of space available in the vending machine
	// for this flavour
	addToVM = maxStockPerFlavour - inventory[flavour];

	// if out of this flavour, go to the next
	if ( cargo[flavour] == 0 ) {
		totalNotReplenished += addToVM;
	    outOfFlavour++;
	    return;
	}

	// if not enough supply, restock what is left
	if (addToVM > cargo[flavour]) {
		totalNotReplenished += (addToVM - cargo[flavour]);
		addToVM = cargo[flavour];
		outOfFlavour++;
	}

	inventory[flavour] += addToVM;
	curCargoSize -= addToVM;
	cargo[flavour] -= addToVM;
}

/**
 * Constructor
 * @param prt printer
 * @param nameServer name server
 * @param numVendingMachines number of vending machines tasks
 * @param plant bottling plant responsible for producing soda
 * @param maxStockPerFlavour max capacity of vending machine for each flavour
 */
Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
       unsigned int numVendingMachines, unsigned int maxStockPerFlavour ):
       prt(prt), ns(nameServer), plant(plant), numVendingMachines(numVendingMachines),
       maxStockPerFlavour(maxStockPerFlavour), addToVM(0), outOfFlavour(0),
       totalNotReplenished(0), curCargoSize(0), cargo(new unsigned int[numFlavours]) {
       	for ( unsigned int i = 0; i < numFlavours; i++ ) cargo[i] = 0;
}

/**
 * Destructor
 */
Truck::~Truck() {
	delete[] cargo;
	prt.print(Printer::Truck, (char)Finished);

}