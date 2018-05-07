
#ifndef PCEventAction_h
#define PCEventAction_h 1

#include "G4UserEventAction.hh"

#include "globals.hh"

/// Event action class

class PCEventAction : public G4UserEventAction
{
  public:
    PCEventAction();
    virtual ~PCEventAction();

    virtual void  BeginOfEventAction(const G4Event* );
    virtual void    EndOfEventAction(const G4Event* );

private:
  G4int fPCTrackerCollID;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
