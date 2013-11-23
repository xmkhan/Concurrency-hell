#include "vendingmachine.h"
#include "printer.h"
#include "nameserver.h"

/**
 * simulate vending machine purchases and 
 * restocking
 */
void VendingMachine::main() {
	prt.print(Printer::Vending, (char)Starting, sodaCost);
	ns.VMregister(this);
	for ( ;; ) {
		_Accept(~VendingMachine) {
			break;
		} or _Accept(buy) {
		} or _Accept(inventory) {
			waiting.wait();
			prt.print(Printer::Vending, (char)RestockingComplete);
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
                    maxStockPerFlavour(maxStockPerFlavour), inventoryList(new unsigned int[numFlavours]) {}


/**
 * Purchase the student's favourite flavour using the watcard provided
 * @param flavour student's favourite flavour
 * @param card the card of the student making the purchase
 * @return the status of the purchase
 */
VendingMachine::Status VendingMachine::buy( VendingMachine::Flavours flavour, WATCard &card ) {
	if ( inventoryList[flavour] == 0 ) return STOCK;
	if ( card.getBalance() < sodaCost ) return FUNDS;
	card.deposit(sodaCost);
	inventoryList[flavour]--;
	prt.print(Printer::Vending, (char)StudentPurchase, flavour, inventoryList[flavour]);
	return BUY;
}

/**
 * @return the vending machines inventory
 */
unsigned int* VendingMachine::inventory() {
	prt.print(Printer::Vending, (char)Restocking);
	return inventoryList;
}

/**
 * signal the uCondition to unblock the vending machines main
 */
void VendingMachine::restocked() { waiting.signal(); }

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
VendingMachine::~VendingMachine() { delete[] inventoryList; }