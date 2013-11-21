#ifndef __STUDENT_H__
#define __STUDENT_H__

#include <uC++.h>
#include "watcard.h"

_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
enum VendingMachine::Flavour;

_Task Student {
    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
             unsigned int maxPurchases );
};

#endif