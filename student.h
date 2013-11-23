#ifndef __STUDENT_H__
#define __STUDENT_H__

#include <uC++.h>

_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;

_Task Student {
    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
             unsigned int maxPurchases );
    ~Student();
  private:
    Printer &printer;
    NameServer &nameServer;
    WATCardOffice &watCardOffice;
    unsigned int id;
    unsigned int maxPurchases;
};

#endif