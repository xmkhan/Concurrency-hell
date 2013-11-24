#include "printer.h"
#include <iostream>

#define NUM_OTHERS 5

using namespace std;

/**
 * Constructor
 * @param numStudents         Number of stuents
 * @param numVendingMachines  Number of vending machines
 * @param numCouriers         Number of couriers
 */
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers )
: numStudents(numStudents),
  numVendingMachines(numVendingMachines),
  numCouriers(numCouriers),
  numColumns(numStudents + numVendingMachines + numCouriers + NUM_OTHERS),
  states(new StateNode[numColumns]) {
	// Print the name of single instance tasks
	cout << "Parent\tWATOFF\tNames\tTruck\tPlant\t";

	// Print student, vending machine, and courier names
	for (unsigned int i = 0; i < numStudents; i++)        cout << "Stud" << i << "\t";
	for (unsigned int i = 0; i < numVendingMachines; i++) cout << "Mach" << i << "\t";
	for (unsigned int i = 0; i < numCouriers-1; i++)      cout << "Cour" << i << "\t";
	cout << "Cour" << (numCouriers-1) << endl;

    // Print underlining name row
	for (unsigned int i = 0; i < numColumns-1; i++) {
		cout << "******\t";
	}
	cout << "******\n";
	flush();
}

/**
 * Printer function used to append state to id's buffers
 * @param kind   kind of task
 * @param state  new state for task
 */
void Printer::print( Kind kind, char state ) {
 	print(kind, 0, state, -1, -1);
}

/**
 * Printer function used to append state to id's buffers
 * @param kind   kind of task
 * @param state  new state for task
 * @param value1 first value of the state
 */
void Printer::print( Kind kind, char state, int value1 ) {
	print(kind, 0, state, value1, -1);
}

/**
 * Printer function used to append state to id's buffers
 * @param kind   kind of task
 * @param state  new state for task
 * @param value1 first value of the state
 * @param value2 second value of the state
 */
void Printer::print( Kind kind, char state, int value1, int value2 ) {
	print(kind, 0, state, value1, value2);
}

/**
 * Printer function used to append state to id's buffers
 * @param kind   kind of task
 * @param id     identifier for task
 * @param state  new state for task
 */
void Printer::print( Kind kind, unsigned int lid, char state ) {
	print(kind, lid, state, -1, -1);
}

/**
 * Printer function used to append state to id's buffers
 * @param kind   kind of task
 * @param id     identifier for task
 * @param state  new state for task
 * @param value1 first value of the state
 */
void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
	print(kind, lid, state, value1, -1);
}

/**
 * Printer function used to append state to id's buffers
 * @param kind   kind of task
 * @param id     identifier for task
 * @param state  new state for task
 * @param value1 first value of the state
 * @param value2 second value of the state
 */
void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
	unsigned int index = getStateIndex(kind, lid);
	bool isOverriden = (states[index].isUsed) ? true : false;

	if (isOverriden || state == 'F') {
		output();
		flush();
	}
	if (state == 'F') {
		outputFinish(index);
	} else {
		states[index].isUsed = true;
		states[index].state = state;
		states[index].a = value1;
		states[index].b = value2;
	}
}

/**
 * Get the state index of the task
 * @param kind  kind of task
 * @param lid   identifier for task
 * @return      state index of the task
 */
unsigned int Printer::getStateIndex( Kind kind, unsigned int lid ) {
	unsigned int index = 0;
	switch ( kind ) {
		case Courier:       index += numVendingMachines;
		case Vending:       index += numStudents;
		case Student:       index += 1;
		case BottlingPlant: index += 1;
		case Truck:         index += 1;
		case NameServer:    index += 1;
		case WATCardOffice: index += 1;
    default:            break;
	}
	return index + lid;
}

/**
 * Destructor
 */
Printer::~Printer() {
	delete[] states;
	cout << "***********************" << endl;
}

/**
 * Output the current state of each task
 */
void Printer::output() {
  // find last valid state to prevent printing out tabs
  int lastIndex = -1;
  for (unsigned int i = 0; i < numColumns; i++) {
    if (states[i].isUsed) lastIndex = i;
  }

  for (unsigned int i = 0; i < numColumns; i++) {
    if (states[i].isUsed) {
      cout << (char)states[i].state; // output state

      if (states[i].a != -1 )  cout << states[i].a;
      if ( states[i].b != -1 ) cout << "," << states[i].b;
    }
    if (lastIndex >= 0 && i == (unsigned int)lastIndex) {
      cout << endl;
      break;
    }
    if (i < numColumns - 1) cout << "\t";
    else cout << endl;
  }
}

/**
 * Outputs the finish row (with all ..., except for id)
 * @param id identifier of the finished task
 */
void Printer::outputFinish(unsigned int id) {
  for (unsigned int i = 0; i < numColumns; i++) {
    if (i != id) cout << "...";
    else cout << "F";
    if (i < numColumns -1) cout <<"\t";
  }
  cout << endl;
}

/**
 * [Printer::flush clears the states buffer]
 */
void Printer::flush() {
  for (unsigned int i = 0; i < numColumns; i++) {
    states[i].isUsed = false;
    states[i].a = -1;
    states[i].b = -1;
  }
}