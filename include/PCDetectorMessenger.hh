
#ifndef PCDetectorMessenger_h
#define PCDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PCDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;

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

  G4UIcmdWithADoubleAndUnit* fDistToAxisX;
  G4UIcmdWithADoubleAndUnit* fDistToAxisY;
};

#endif
