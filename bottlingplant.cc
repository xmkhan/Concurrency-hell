#include "bottlingplant.h"
#include <algorithm>

/**
 * Constructor
 * @param prt printer
 * @param nameServer nameserver task
 * @param numVendingMachines number of vending machines tasks
 * @param maxShippedPerFlavour max number of soda per flavour per shippment
 * @param maxStockPerFlavour max capacity of vending machine for each flavour
 * @param timeBetweenShipments # of yields to simulate a production run
 */
BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments ) : prt(&prt), ns(&ns), 
                 numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour),
                 maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments),
                 shippment(new unsigned int[maxNumFlavours]) {

}

/**
 * Destructor
 */
BootlingPlant::~BottlingPlant() { 
	delete[] shippment;
	delete truck;
}

/**
 * obtain a shipment from the plant
 * @param cargo[] shipment is copied into cargo[]
 * @return true if the bottling plant is closing 
 * down and cargo is not changed, and false otherwise
 */
bool BottlingPlant::getShipment( unsigned int cargo[] ) { 
	// if closing down, return false
	if ( false ) return false;
	std::copy(std::begin(shippment), std::end(shippment), std::begin(cargo));
	waiting.signal();
	return true;
}

/**
 * Simulate production of soda
 */ 
void BottlingPlant::main() {
	truck = new Truck(prt, nameServer, this, numVendingMachines, maxStockPerFlavour);

	for ( ;; ) {
		for( int i = 0; i < maxNumFlavours; i++ ) {
			shippment[i] = RNG(0, maxShippedPerFlavour);
		}	
		yield(timeBetweenShipments);
		waiting.wait();
		// need to figure out whe to close to plant
	}
}