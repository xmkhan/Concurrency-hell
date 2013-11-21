#include <iostream>
#include "q2printer.h"
#include "q2phil.h"


Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) {

}

/*
 * Call the second print function, with 0 as the numBlocked 
 * value
 */
void Printer::print( unsigned int id, Philosopher::States state ) {
    unsigned int tabs = 0, numNulls = 0;
    
    if( !states.at(id) && state != Philosopher::Finished) { 
        states.at(id) = state;
        return;
    }

    // Output the states if one of them needs to be overwritten
    for( unsigned int i = 0; i < numColumns; i++ ) {
        if (!states.at(i)) {
            numNulls++;
            tabs++;
            continue;
        }
        for ( unsigned int j = 0; j < tabs; j++ ) std::cout<<'\t';
        tabs = 1;
        std::cout<<states.at(i);
        if ( states.at(i) == Philosopher::Eating ) {
            std::cout<<eating.at(i)<<","<<leaving.at(i);
        }
        else if ( states.at(i) == Philosopher::Waiting ) {
            std::cout<<i<<","<<(i+1)%numColumns;
        }

    }

    // If all the columns were empty, do not 
    // output a newline ( in case of state = finished )
    if ( numNulls != numColumns ) std::cout<<"\n";

    // Output the Finish line if required
    if ( state == Philosopher::Finished ) {
        outputFinish(id);        
    }

    // flush the states
    states.clear();
    states.resize(numColumns);

    // If state is != Finished, store it
    if ( state != Philosopher::Finished ) {
        states.at(id) = state;
    }
}

void Printer::outputFinish( unsigned int id ) {
    for ( unsigned int i = 0; i < numColumns; i++ ) {
        if ( i != id ) std::osacquire(std::cout)<<"...";
        else std::osacquire(std::cout)<<'F';
        if ( i < numColumns-1 ) std::osacquire(std::cout)<<"\t";
    }
    std::osacquire(std::cout)<<"\n";   
}

/*
 * If the id already contains a state, print all the states
 * we have stored, flush the states, and store the new value
 * ******** Parameters
 * id: Id of the task that called print
 * state: state of the task
 * numBlocked: Number of tasks blocked (done)consulting and
 *             (done)delivering
 */
void Printer::print( unsigned int id, Philosopher::States state, unsigned int bite, unsigned int noodles ) {
    print(id, state);

    eating.at(id) = bite;
    leaving.at(id) = noodles;
}