#ifndef PCDetectorConstruction_h
#define PCDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class PCDetectorMessenger;

class PCDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    PCDetectorConstruction();
    virtual ~PCDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  G4LogicalVolume* GetColl() const { return fColl; }
  G4LogicalVolume* GetTube() const { return fTube; }
  G4LogicalVolume* GetCham() const { return fCham; }

  G4double DistToAxisX, DistToAxisY;

  // Set methods
  void SetDistToAxisX(G4double );
  void SetDistToAxisY(G4double );
  
  

private:

  G4LogicalVolume** fPC_logic;
  G4LogicalVolume* fTarget;

  G4LogicalVolume* fTube;
  G4LogicalVolume* fColl;
  G4LogicalVolume* fCham;

  PCDetectorMessenger* fMessenger ;

  
  //  G4LogicalVolume* PC_logic1;
  //G4LogicalVolume* PC_logic2;
  //G4LogicalVolume* PC_logic3;
    
 
};


#endif

