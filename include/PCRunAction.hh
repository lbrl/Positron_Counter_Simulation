#ifndef PCRunAction_h 
#define PCRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class HistoManager;

class PCRunAction : public G4UserRunAction
{
  
public:
  PCRunAction(HistoManager* );
  virtual ~PCRunAction();

  virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

private:
  HistoManager* fHistoManager;

};

#endif
