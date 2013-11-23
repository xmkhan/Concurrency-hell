#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <uC++.h>

_Monitor Printer {
	public:
		enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
		Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
        ~Printer();
		void print( Kind kind, char state );
		void print( Kind kind, char state, int value1 );
		void print( Kind kind, char state, int value1, int value2 );
		void print( Kind kind, unsigned int lid, char state );
		void print( Kind kind, unsigned int lid, char state, int value1 );
		void print( Kind kind, unsigned int lid, char state, int value1, int value2 );

	private:
        void output(); // internal output function
        void outputFinish(unsigned int id); //output the finish row
        void flush(); // clear the buffer
        unsigned int getStateIndex( Kind kind, unsigned int id );

        // number of students, vending machines, couriers, and total number of columns
        unsigned int numStudents, numVendingMachines, numCouriers, numColumns;

        /**
         * StateNode used to represent the complete State information
         */
        struct StateNode {
     	    bool isUsed;
     	    char state;
     	    int a;
     	    int b;
     	};
     	StateNode *states;
};

#endif