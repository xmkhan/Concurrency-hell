#ifndef __TRUCK_H__
#define __TRUCK_H__

#include <uC++.h>

_Monitor Printer;
_Monitor NameServer;
_Task Truck;

_Task Truck {
    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif