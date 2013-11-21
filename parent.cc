#include "parent.h"
#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

/**
 * Default constructor
 * @param prt printer
 * @param bank Bank used for deposits
 * @param numStudents the No. of students
 * #param parentalDelay the delay between gifts
 */
Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay )
: printer(prt),
  bank(bank),
  numStudents(numStudents),
  parentalDelay(parentalDelay) {}

/**
 * Destructor
 */
Parent::~Parent() {
  printer.print(Printer::Parent, (char)Parent::Finished);
}

/**
 * Continually gives money to a random student
 */
void Parent::main() {
  printer.print(Printer::Parent, (char)Parent::Starting);
  for ( ;; ) {
    _Accept(~Parent) {
      break;
    } _Else {
      yield(parentalDelay);
      // Deposit [1,3] to to student [0, numStudents - 1]
      unsigned int studentId = RNG(0, numStudents - 1);
      unsigned int gift = RNG(1, 3);
      printer.print(Printer::Parent, (char)Parent::Deposit, studentId, gift);
      bank.deposit(studentId, gift);
    }
  }
}