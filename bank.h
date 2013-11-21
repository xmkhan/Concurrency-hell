#ifndef __BANK_H__
#define __BANK_H__

#include <uC++.h>
#include <queue>


_Monitor Bank {
  public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
  private:
    uCondition *waiting;
    unsigned int *bank; // holds bank accounts for students
    std::queue<unsigned int> *queues; // queue of funds demanded for students
    unsigned int numStudents;
};

#endif