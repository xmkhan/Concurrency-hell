#include "bank.h"

/**
 * Default constructor
 * @param numStudents the No. of students
 */
Bank::Bank( unsigned int numStudents )
: waiting(new uCondition[numStudents]),
  bank(new unsigned int[numStudents]),
  numStudents(numStudents) {
    for (unsigned int i = 0; i < numStudents; i++) bank[i] = 0;
  }

/**
 * Destructor
 */
Bank::~Bank() {
  delete[] waiting;
  delete[] bank;
}

/**
 * Deposits the amount to the bank under student's id,
 * unblocking all possible withdraw requests
 * @param id     identifier of the student (bank account)
 * @param amount money to deposit
 */
void Bank::deposit( unsigned int id, unsigned int amount ) {
  bank[id] += amount;
  // Unblock all withdraw requests as long as student has enough money
  if (bank[id] >= 0) waiting[id].signal();
}

/**
 * Withdraws the amount from bank under student's id,
 * blocks if the student does not have enough money
 * @param id     identifier of the student (bank account)
 * @param amount money to withdraw
 */
void Bank::withdraw( unsigned int id, unsigned int amount ) {
  bank[id] -= amount;
  if (bank[id] < 0) waiting[id].wait();
}
