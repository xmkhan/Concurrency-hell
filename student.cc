#include "student.h"
#include "MPRNG.h"
#include "printer.h"
#include "nameserver.h"
#include "watcardoffice.h"

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases )
: printer(prt),
  nameServer(nameServer),
  watCardOffice(cardOffice),
  id(id),
  maxPurchases(maxPurchases) {

  }
Student::~Student() { }

void Student::main() {
  unsigned int numberOfBottles = RNG(1, maxPurchases);
  unsigned int favouriteFlavour = RNG(0, 3);
  WATCard::FWATCard watCard = watCardOffice.create(id, 5);
  VendingMachine *vendingMachine = nameServer.getMachine(id);

  unsigned int purchasedBottles = 0;
  bool retry = false;
  while (purchasedBottles < numberOfBottles) {
    yield(RNG(1, 10));
      try{
        L:
          if (retry) watCard = watCardOffice.create(id, 5);
          VendingMachine::Status status = vendingMachine->buy(favouriteFlavour, *watCard());
          switch (status) {
            case FUNDS:
              watCard = watCardOffice.transfer(vendingMachine->cost() + 5);
              break;
            case STOCK:
              vendingMachine = nameServer.getMachine(id);
              retry = false;
              goto L;
            case BUY:
            default: break;
          }
          retry = false;
          purchasedBottles += 1;
      } catch (watCardOffice::Lost &e) {
        retry = true;
        goto L;
      }
  }
}