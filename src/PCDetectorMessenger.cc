#include "PCDetectorMessenger.hh"
#include "PCDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

PCDetectorMessenger::PCDetectorMessenger(PCDetectorConstruction* Det)
  :G4UImessenger(),
   fDetectorConstruction(Det)
{
  fPCDirectory = new G4UIdirectory("/PC/");
  fPCDirectory->SetGuidance("UI commands specific to the PositronCounter's axis distance");

  fDistToAxisX = new G4UIcmdWithADoubleAndUnit("/PC/setDistToAxisX",this);
  fDistToAxisX->SetGuidance("Set the DistToAxisX");
  fDistToAxisX->SetParameterName("DistToAxisX",false);
  fDistToAxisX->SetUnitCategory("Length");
  fDistToAxisX->AvailableForStates(G4State_Idle);

  fDistToAxisY = new G4UIcmdWithADoubleAndUnit("/PC/setDistToAxisY",this);
  fDistToAxisY->SetGuidance("Set the DistToAxisY");
  fDistToAxisY->SetParameterName("DistToAxisY",false);
  fDistToAxisY->SetUnitCategory("Length");
  fDistToAxisY->AvailableForStates(G4State_PreInit,G4State_Idle);

}

PCDetectorMessenger::~PCDetectorMessenger()
{
  delete fDistToAxisX;
  delete fDistToAxisY;
  delete fPCDirectory;
  //  delete fDetDirectory;
}

void PCDetectorMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
  if ( cmd == fDistToAxisX )
    { fDetectorConstruction -> SetDistToAxisX(fDistToAxisX->GetNewDoubleValue(newValue));}

   if ( cmd == fDistToAxisY )
    { fDetectorConstruction -> SetDistToAxisY(fDistToAxisY->GetNewDoubleValue(newValue));}

}
