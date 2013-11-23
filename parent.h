#ifndef __PARENT_H__
#define __PARENT_H__

_Monitor Printer;
_Monitor Bank;

_Task Parent {
    void main();
  public:
    enum States { Starting = 'S', Deposit = 'D', Finished = 'F' };
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
    ~Parent();
  private:
    Printer &printer;
    Bank &bank;
    unsigned int numStudents;
    unsigned int parentalDelay;
};

#endif