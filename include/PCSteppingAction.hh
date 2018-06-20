#ifndef PCSteppingAction_h
#define PCSteppingAction_h 1

#include "G4UserSteppingAction.hh"


class PCEventAction;
class PCDetectorConstruction;

class G4LogicalVolume;

class PCSteppingAction : public G4UserSteppingAction
{
public:
  PCSteppingAction(PCDetectorConstruction*, PCEventAction*);
  virtual ~PCSteppingAction();

  virtual void UserSteppingAction(const G4Step*);
    
private:
  PCEventAction*          fEventAction;
  PCDetectorConstruction* fDetector;
};


#endif
