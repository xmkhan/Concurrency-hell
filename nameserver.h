#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

#include <uC++.h>

_Monitor Printer;
_Task VendingMachine;

_Task NameServer {
	Printer& prt;
	unsigned int numVendingMachines, numStudents;
	VendingMachine **vms;
	int *studentToVM;
	unsigned int sIndex, vmIndex;
	uCondition waiting;

	enum States { Starting = 'S', RegisterVendingMachine = 'R', 
	              NewVendingMachine = 'N', Finished = 'F' };

    void main();
    
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
    ~NameServer();
};

#endif