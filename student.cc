#include "student.h"
#include "MPRNG.h"
#include "printer.h"
#include "nameserver.h"
#include "watcardoffice.h"
#include "vendingmachine.h"

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
      L:
      try{
          if (retry) watCard = watCardOffice.create(id, 5);
          VendingMachine::Status status = vendingMachine->buy((VendingMachine::Flavours)favouriteFlavour, *watCard());
          switch (status) {
            case VendingMachine::FUNDS:
              watCard = watCardOffice.transfer(id, vendingMachine->cost() + 5, watCard());
              retry = false;
              goto L;
            case VendingMachine::STOCK:
              vendingMachine = nameServer.getMachine(id);
              retry = false;
              goto L;
            case VendingMachine::BUY:
              purchasedBottles += 1;
              retry = false;
            default:
            break;
          }
      } catch (WATCardOffice::Lost &e) {
        retry = true;
        goto L;
      }
  }
}