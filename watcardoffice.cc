#include "watcardoffice.h"
#include "MPRNG.h"
#include "printer.h"
#include "bank.h"

/**
 * Default constructor
 */
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers )
: printer(prt),
  bank(bank),
  numCouriers(numCouriers),
  terminated(false),
  couriers(new Courier*[numCouriers]) {
    for (unsigned int i = 0; i < numCouriers; i++)
      couriers[i] = new Courier( prt, bank, *this, i );
  }

/**
 * Destructor
 */
WATCardOffice::~WATCardOffice() {
  terminated = true;
  while(!jobs.empty()) {
    Job *job = jobs.front(); jobs.pop();
    delete job;
  }

  while(!waiting.empty()) waiting.signalBlock();
  for (unsigned int i = 0; i < numCouriers; i++) delete couriers[i];
  delete[] couriers;
  printer.print(Printer::WATCardOffice, (char)WATCardOffice::Finished);
}

/**
 * Creates a job on the job queue for creating a watcard to be
 * managed by the courier on behalf of the WATCardOffice
 * @param  sid    student id
 * @param  amount amount to create with
 */
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  printer.print(Printer::WATCardOffice, (char)WATCardOffice::Creation, sid, amount);
  Job *job = new Job(Args(sid, amount, new WATCard()));
  jobs.push(job);
  return job->result;
}

/**
 * Creates a job on the job queue for transferring money for a
 * specific student's WATCard
 * @param  sid    student id
 * @param  amount amount to transfer
 * @param  card   represents the WATCard
 */
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
  printer.print(Printer::WATCardOffice, (char)WATCardOffice::Transfer, sid, amount);
  Job *job = new Job(Args(sid, amount, card));
  jobs.push(job);
  return job->result;
}

/**
 * Blocking call: Couriers request work from the WATCardOffice
 * @return Job struct containing details of the job to be performed
 */
WATCardOffice::Job *WATCardOffice::requestWork() {
  if (jobs.empty() && !terminated) {
    waiting.wait();
  }
  if (terminated) return NULL;
  printer.print(Printer::WATCardOffice, (char)WATCardOffice::Waiting);
  Job *job = jobs.front(); jobs.pop();
  return job;
}

/**
 * WATCardOffice looks for calls to its public interface, blocks when
 * there are no callers. When create/transfer are called, we unblock
 * the appropriate couriers waiting for work
 */
void WATCardOffice::main() {
  printer.print(Printer::WATCardOffice, (char)WATCardOffice::Starting);

  for ( ;; ) {
    _Accept(~WATCardOffice) {
      break;
    } or _Accept(create, transfer) {
      // Unblock Courier because job is now available
      waiting.signalBlock();
    } or _Accept(requestWork) {}
  }
}

/**
 * Default constructor
 */
WATCardOffice::Courier::Courier(Printer &prt, Bank &bank, WATCardOffice &watCardOffice, unsigned int id)
: printer(prt),
  bank(bank),
  watCardOffice(watCardOffice),
  id(id) {}

/**
 * Destructor
 */
WATCardOffice::Courier::~Courier() {
  printer.print(Printer::Courier, id, (char)WATCardOffice::Courier::Finished);
}

/**
 * Courier task requests work from WATCardOffice, blocks if no work
 * If job == null, it terminates
 * Otherwise, it performs the appropriate functions with the bank,
 * and has a 1/6 chance to lose the student's WATCard
 */
void WATCardOffice::Courier::main() {
  printer.print(Printer::Courier, id, (char)WATCardOffice::Courier::Starting);
  for ( ;; ) {
    // Block: if no work
    WATCardOffice::Job *job = watCardOffice.requestWork();
    if (job == NULL) break;
    printer.print(Printer::Courier, id, (char)WATCardOffice::Courier::FundTransfer, job->args.sid, job->args.amount);
    bank.withdraw(job->args.sid, job->args.amount);
    job->args.watcard->deposit(job->args.amount);
    printer.print(Printer::Courier, id, (char)WATCardOffice::Courier::FundComplete, job->args.sid, job->args.amount);
    if (RNG(1,6) == 1) {
      // 1 in 6 chance that courier loses a student's watcard
      job->result.exception(new Lost);
      delete job->args.watcard;
    } else {
      job->result.delivery(job->args.watcard);
    }
    delete job;
  }
}