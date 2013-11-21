#include "student.h"

void Student::main() {	
	wc = wco->create(id, 5);
	vm = ns->getMachine(id);

	while ( numPurchases > 0 ) {
		yiled(RNG(1, 10));
	}
}

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
             unsigned int maxPurchases ): prt(&prt), ns(&nameServer), wco(&WATCardOffice),
             id(id), flavour(VendingMachine::Flavours(RNG(3))), numPurchases(RNG(1, maxPurchases)) {}