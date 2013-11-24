#ifndef __VENDINGMACHINE_H__
#define __VENDINGMACHINE_H__

#include <uC++.h>
#include "watcard.h"

#define numFlavours 4

_Monitor Printer;
_Task NameServer;


_Task VendingMachine {
    Printer &prt;
    NameServer &ns;
    uCondition waiting;
    unsigned int id, sodaCost,
                 maxStockPerFlavour;
    unsigned int *inventoryList;
    enum States { Starting = 'S', Restocking = 'r',
                  RestockingComplete = 'R', StudentPurchase = 'B',
                  Finished = 'F' };

    bool stocked;

    void main();
  public:
    enum Flavours { Cherry = 0, Strawberry, Orange, Grape };                 // flavours of soda (YOU DEFINE)
    enum Status { BUY, STOCK, FUNDS };     // purchase status: successful buy, out of stock, insufficient funds
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    Status buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();

    ~VendingMachine();
};

#endif