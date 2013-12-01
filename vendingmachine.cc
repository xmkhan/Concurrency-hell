#include "vendingmachine.h"
#include "printer.h"
#include "nameserver.h"

/**
 * simulate vending machine purchases and
 * restocking
 */
void VendingMachine::main() {
	prt.print(Printer::Vending, id, (char)Starting, sodaCost);
	for ( ;; ) {
		_Accept(~VendingMachine) {
			break;
		} or _Accept(inventory) {
            prt.print(Printer::Vending, id, (char)Restocking);
        } or _Accept(restocked) {
            prt.print(Printer::Vending, id, (char)RestockingComplete);
        }or _When(stocked) _Accept(buy) {
        }
	}
}

/**
 * Constructor
 * @param prt printer
 * @param nameServer name server task
 * @param id of the vending machine
 * @param sodaCost cost of each can of soda
 * @param maxStockPerFlavour max space available for each soda type
 */
VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
    unsigned int maxStockPerFlavour ):prt(prt), ns(nameServer), id(id), sodaCost(sodaCost),
    maxStockPerFlavour(maxStockPerFlavour), inventoryList(new unsigned int[numFlavours]), stocked(true) {
    	for( unsigned int i = 0; i < numFlavours; i++ ) inventoryList[i] = 0;
        ns.VMregister(this);
}


/**
 * Purchase the student's favourite flavour using the watcard provided
 * @param flavour student's favourite flavour
 * @param card the card of the student making the purchase
 * @return the status of the purchase
 */
VendingMachine::Status VendingMachine::buy( VendingMachine::Flavours flavour, WATCard &card ) {
	if ( inventoryList[flavour] == 0 ) return STOCK;
	if ( card.getBalance() < sodaCost ) return FUNDS;
	card.withdraw(sodaCost);
	inventoryList[flavour]--;
	prt.print(Printer::Vending, id, (char)StudentPurchase, flavour, inventoryList[flavour]);
	return BUY;
}

/**
 * @return the vending machines inventory
 */
unsigned int* VendingMachine::inventory() {
    stocked = false;
	return inventoryList;
}

/**
 * signal the uCondition to unblock the vending machines main
 */
void VendingMachine::restocked() {
    stocked = true;
}

/**
 * @return the cost of soda
 */
_Nomutex unsigned int VendingMachine::cost() { return sodaCost; }

/**
 * @return the id
 */
_Nomutex unsigned int VendingMachine::getId() { return id; }

/**
 * Destructor
 */
VendingMachine::~VendingMachine() {
	prt.print(Printer::Vending, id, (char)Finished);
	delete[] inventoryList;
}