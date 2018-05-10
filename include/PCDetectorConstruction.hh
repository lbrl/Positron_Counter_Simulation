#ifndef PCDetectorConstruction_h
#define PCDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;


class PCDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    PCDetectorConstruction();
    virtual ~PCDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

private:

  G4LogicalVolume** fPC_logic;
  G4LogicalVolume* fTarget;
  
  //  G4LogicalVolume* PC_logic1;
  //G4LogicalVolume* PC_logic2;
  //G4LogicalVolume* PC_logic3;
    
 
};


#endif

