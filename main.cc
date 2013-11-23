#include <uC++.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>

#include "MPRNG.h"
#include "config.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "watcardoffice.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "bottlingplant.h"
#include "student.h"

using namespace std;

MPRNG RNG;

void usage(char *argv[]) {
  cerr << "Usage: " << argv[0] << " [ config-file [ random-seed (> 0) ] ]" << endl;
  exit(EXIT_FAILURE);
}

void uMain::main() {
  int s = getpid();
  std::string configFilename("soda.config");

  switch (argc) {
    case 3:
      s = atoi(argv[2]);
    case 2:
      configFilename = std::string(argv[1]);
    default:
      break;
  }
  if (s <= 0) usage(argv);

  RNG.seed(s);
  ConfigParms config;
  processConfigFile(configFilename.c_str(), config);

  Printer printer(config.numStudents, config.numVendingMachines, config.numCouriers);
  Bank bank(config.numStudents);
  Parent parent(printer, bank, config.numStudents, config.parentalDelay);
  WATCardOffice watCardOffice(printer, bank, config.numCouriers);
  NameServer nameServer(printer, config.numVendingMachines, config.numStudents);

  VendingMachine *vendingMachines[config.numVendingMachines];
  for (unsigned int i = 0; i < config.numVendingMachines; i++) {
    vendingMachines[i] = new VendingMachine(printer, nameServer, i, config.sodaCost, config.maxStockPerFlavour);
  }

  BottlingPlant *bottlingPlant = new BottlingPlant(printer, nameServer, config.numVendingMachines,
    config.maxShippedPerFlavour, config.maxStockPerFlavour, config.timeBetweenShipments);

  Student *students[config.numStudents];
  for(unsigned int i = 0; i < config.numStudents; i++) {
    students[i] = new Student(printer, nameServer, watCardOffice, i, config.maxPurchases);
  }

  for (unsigned int i = 0; i < config.numStudents; i++) {
    delete students[i];
  }

  delete bottlingPlant;

  for (unsigned int i = 0; i < config.numVendingMachines; i++) {
    delete vendingMachines[i];
  }
}
