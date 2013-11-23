#include "bottlingplant.h"
#include "printer.h"
#include "nameserver.h"
#include "truck.h"
#include "MPRNG.h"
#include "vendingmachine.h"

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
                 unsigned int timeBetweenShipments ):prt(prt), ns(ns), closed(false),
                 numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour),
                 maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments),
                 shippment(new unsigned int[numFlavours]) {}

/**
 * Destructor
 */
BottlingPlant::~BottlingPlant() { 
	closed = true;
	delete[] shippment;
	delete truck;
	prt.print(Printer::BottlingPlant, Finished);
}

/**
 * obtain a shipment from the plant
 * @param cargo[] shipment is copied into cargo[]
 * @return true if the bottling plant is closing 
 * down and cargo is not changed, and false otherwise
 */
bool BottlingPlant::getShipment( unsigned int cargo[] ) { 
	if ( closed ) return true;
	for ( unsigned int j = 0; j < numFlavours; j++ ) {
		cargo[j] = shippment[j];
	}
	return false;
}

/**
 * Simulate production of soda
 */ 
void BottlingPlant::main() {
	truck = new Truck(prt, ns, *this, numVendingMachines, maxStockPerFlavour);
	prt.print(Printer::BottlingPlant, Starting);
	for ( ;; ) {
		prt.print(Printer::BottlingPlant, GeneratingSoda);
		for( int i = 0; i < numFlavours; i++ ) {
			shippment[i] = RNG(0, maxShippedPerFlavour);
		}	
		yield(timeBetweenShipments);

		_Accept(~BottlingPlant) {
			break;
		} or _Accept(getShipment) {
			prt.print(Printer::BottlingPlant, ShipmentPickedUp);
		}
	}
}