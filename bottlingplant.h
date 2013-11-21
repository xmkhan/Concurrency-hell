#ifndef __BOTTLINGPLANT_H__
#define __BOTTLINGPLANT_H__

_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
_Task Truck;

_Task BottlingPlant {
	Truck *truck;
    Printer& prt;
    NameServer& ns;
    unsigned int* shippment;
    unsigned int numVendingMachines, maxShippedPerFlavour,
                 maxStockPerFlavour, timeBetweenShipments;

    const unsigned int numOfFlavours = 4;

    void main();
  public:
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    bool getShipment( unsigned int cargo[] );
};

#endif