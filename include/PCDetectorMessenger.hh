
#ifndef PCDetectorMessenger_h
#define PCDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PCDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcommand;
class G4UIcmdWithABool;

class PCDetectorMessenger : public G4UImessenger
{
public :
  PCDetectorMessenger(PCDetectorConstruction* );
  virtual ~PCDetectorMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  PCDetectorConstruction* fDetectorConstruction;

  G4UIdirectory* fPCDirectory;
  //  G4UIdirectory* fDetDirectory;
  G4UIdirectory* fVolumeDir;

  
  G4UIcmdWithADoubleAndUnit* fDistToAxisX;
  G4UIcmdWithADoubleAndUnit* fDistToAxisY;
  G4UIcmdWithADoubleAndUnit* fDistToPC_Offset;

  G4UIcmdWithADoubleAndUnit* fAngleCmd;
  G4UIcmdWithADoubleAndUnit* fThicknessCmd;
  G4UIcmdWithABool* fMirrorCmd;
  G4UIcmdWithABool* fStopperCmd;
  G4UIcmdWithABool* fPCPositionCmd; // 0 for close, 1 for far
  G4UIcmdWithABool* fCsITypeCmd; // 0 for foil, 1 for crystal

  G4UIcommand* fDefaultsCmd;
  


};

#endif
