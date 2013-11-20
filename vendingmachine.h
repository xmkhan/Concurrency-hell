#ifndef __VENDINGMACHINE_H__
#define __VENDINGMACHINE_H__

#include <uC++.h>
#include "watcard.h"

_Monitor Printer;
_Monitor NameServer;


_Task VendingMachine {
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
};

#endif