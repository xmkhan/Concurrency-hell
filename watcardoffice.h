#ifndef __WATCARDOFFICE_H__
#define __WATCARDOFFICE_H__

#include <uC++.h>
#include <queue>
#include "watcard.h"

_Monitor Printer;
_Monitor Bank;

_Task WATCardOffice {
    struct Args {
        unsigned int sid;
        unsigned int amount;
        WATCard *watcard;
        Args( unsigned int sid, unsigned int amount, WATCard *watcard) : sid(sid), amount(amount), watcard(watcard) {}
    };
    struct Job {                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;                   // return future
        Job( Args args ) : args( args ) {}
    };
    _Task Courier {
        void main();
      public:
        enum States { Starting = 'S', FundTransfer = 't', FundComplete = 'T', Finished = 'F' };
        Courier( Printer &prt, Bank &bank, WATCardOffice &watCardOffice );
        ~Courier();
      private:
        Printer &printer;
        Bank &bank;
        WATCardOffice &watCardOffice;
    };
    void main();
  public:
    _Event Lost {};                        // uC++ exception type, like "struct"
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    enum States { Starting = 'S', Waiting = 'W', Creation = 'C', Transfer = 'T', Finished = 'F'};
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
  private:
    uCondition waiting;
    Printer &printer;
    Bank &bank;
    unsigned int numCouriers;
    bool terminated;
    Courier **couriers;
    std::queue<Job*> jobs;
};

#endif