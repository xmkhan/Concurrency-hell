#include "nameserver.h"
#include "vendingmachine.h"
#include "printer.h"


/**
 * Constructor
 * @param prt printer
 * @param numVendingMachines number of vending machines
 * @param numStudents number of students
 */
NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ):
	prt(prt), numVendingMachines(numVendingMachines), numStudents(numStudents),
    vms(new VendingMachine*[numVendingMachines]), studentToVM(new int[numStudents]), sIndex(0), vmIndex(0) {

    // assign studnets to an initial vending machine
    for ( unsigned int i = 0; i < numStudents; i++ ) studentToVM[i % numVendingMachines] = i % numVendingMachines;

    prt.print(Printer::NameServer, (char)Starting);
}

/**
 * Register a vending machine so it could be pass to a student
 * @param vendingMachine the vending machine to be registered
 */
void NameServer::VMregister( VendingMachine *vendingMachine) {
	// register he vending machine
	vms[vmIndex++] = vendingMachine;

	prt.print(Printer::NameServer, (char)RegisterVendingMachine,
		vendingMachine->getId());
}

/**
 * @param id of the student
 * @return a vending machine to a student to purchase from
 */
VendingMachine *NameServer::getMachine( unsigned int id ) {
	int curVmId = studentToVM[id];
	studentToVM[id] = (curVmId + 1)%numVendingMachines;
	prt.print(Printer::NameServer, (char)NewVendingMachine, id,
		vms[curVmId]->getId());
	return vms[curVmId];
}

/**
 * @return the list of vending machines
 */
VendingMachine **NameServer::getMachineList() {
	return vms;
}

/**
 * Destructor
 */
NameServer::~NameServer() {
    delete[] vms;
	delete studentToVM;
	prt.print(Printer::NameServer, (char)Finished);
}

/**
 * Main, runs until destructor is called
 */
void NameServer::main() {
	for( ;; ) {
		_Accept(~NameServer) { break; }
		or _Accept(VMregister, getMachine, getMachineList) {}
	}
}
