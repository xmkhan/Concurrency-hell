#include "student.h"
#include "MPRNG.h"
#include "printer.h"
#include "nameserver.h"
#include "watcardoffice.h"

/**
 * Default constructor
 */
Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases )
: printer(prt),
  nameServer(nameServer),
  watCardOffice(cardOffice),
  id(id),
  maxPurchases(maxPurchases) {}

/**
 * Destructor
 */
Student::~Student() { printer.print(Printer::Student, (char)Student::Finished); }

/**
 * Student starts buying it's favourite soda from it's assigned vending machine.
 * If the student's WATCard gets lost along the way, he tries to re-create a new one
 * Additionally, if the vending machine is out of the student's favourite flavour
 * he gets another vending machine by the name server.
 */
void Student::main() {
  unsigned int numberOfBottles = RNG(1, maxPurchases);
  unsigned int favouriteFlavour = RNG(0, 3);
  printer.print(Printer::Student, (char)Student::Starting);

  WATCard::FWATCard watCard = watCardOffice.create(id, 5);
  VendingMachine *vendingMachine = nameServer.getMachine(id);
  printer.printer(Printer::Student, (char)Student::Vending, vendingMachine->getId());

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
              retry = false;
              goto L;
            case STOCK:
              vendingMachine = nameServer.getMachine(id);
              printer.printer(Printer::Student, (char)Student::Vending, vendingMachine->getId());
              retry = false;
              goto L;
            case BUY:
              purchasedBottles += 1;
              printer.print(Printer::Student, (char)Student::Bought, watCard()->getBalance());
              retry = false;
            default:
            break;
          }
      } catch (watCardOffice::Lost &e) {
        printer.print(Printer::Student, (char)Student::Lost);
        retry = true;
        goto L;
      }
  }
}