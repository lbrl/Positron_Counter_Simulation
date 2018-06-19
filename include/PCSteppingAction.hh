#ifndef PCSteppingAction_h
#define PCSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class PCDetectorConstruction;
class PCEventAction;

class PCSteppingAction : public G4UserSteppingAction
{
public:
  PCSteppingAction(PCDetectorConstruction*, PCEventAction*);
  virtual ~PCSteppingAction();

  virtual void PCUserSteppingAction(const G4Step*);
    
private:
  PCDetectorConstruction* fDetector;
  PCEventAction*          fEventAction;  
};


#endif
