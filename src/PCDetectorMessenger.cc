#include "PCDetectorMessenger.hh"
#include "PCDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcommand.hh"

#include "G4RunManager.hh"

PCDetectorMessenger::PCDetectorMessenger(PCDetectorConstruction* Det)
  :G4UImessenger(),
   fDetectorConstruction(Det)
{
  fPCDirectory = new G4UIdirectory("/PC/");
  fPCDirectory->SetGuidance("UI commands specific to the PositronCounter's axis distance");

  fVolumeDir = new G4UIdirectory("/volumes/");
  fVolumeDir->SetGuidance("Enable/disable or change the position of volumes");
  
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

  fDistToPC_Offset = new G4UIcmdWithADoubleAndUnit("/PC/setDistToPC_Offset",this);
  fDistToPC_Offset->SetGuidance("Set the DistToPC_Offset");
  fDistToPC_Offset->SetParameterName("DistToPC_Offset",false);
  fDistToPC_Offset->SetUnitCategory("Length");
  fDistToPC_Offset->AvailableForStates(G4State_PreInit,G4State_Idle);

  fMirrorCmd = new G4UIcmdWithABool("/volumes/Mirror",this);
  fMirrorCmd->SetGuidance("Enable/Disable the mirror");
  fMirrorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fStopperCmd = new G4UIcmdWithABool("/volumes/Stopper",this);
  fStopperCmd->SetGuidance("Enable/Disable the Al stopper");
  fStopperCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fPCPositionCmd = new G4UIcmdWithABool("/PC/dist_configuration",this);
  fPCPositionCmd->SetGuidance("False for close, True for far");
  fPCPositionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fCsITypeCmd = new G4UIcmdWithABool("/volumes/CsI_type",this);
  fCsITypeCmd->SetGuidance("False for foil, True for crystal");
  fCsITypeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fAngleCmd = new G4UIcmdWithADoubleAndUnit("/volumes/CsI_angle",this);
  fAngleCmd->SetGuidance("Set the angle of CsI(Tl) foil");
  fAngleCmd->SetParameterName("CsI_angle",false);
  fAngleCmd->SetDefaultUnit("degree");
  fAngleCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fThicknessCmd = new G4UIcmdWithADoubleAndUnit("/volumes/CsI_thickness",this);
  fThicknessCmd->SetGuidance("Set the thickness of CsI(Tl) foil/crystal");
  fThicknessCmd->SetParameterName("CsI_thickness",false);
  fThicknessCmd->SetDefaultUnit("um");
  fThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fDefaultsCmd = new G4UIcommand("/volumes/defaults",this);
  fDefaultsCmd->SetGuidance("Set all detector geometry valye to defaults");
  fDefaultsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

PCDetectorMessenger::~PCDetectorMessenger()
{
  delete fDistToAxisX;
  delete fDistToAxisY;
  delete fDistToPC_Offset;
  delete fPCDirectory;
  delete fVolumeDir;
  delete fMirrorCmd;
  delete fStopperCmd;
  delete fPCPositionCmd;
  delete fCsITypeCmd;
  delete fAngleCmd;
  delete fThicknessCmd;
  delete fDefaultsCmd;
  //  delete fDetDirectory;
}
  
void PCDetectorMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
  
  if ( cmd == fDistToAxisX )
    { fDetectorConstruction -> SetDistToAxisX(fDistToAxisX->GetNewDoubleValue(newValue));}

  else if ( cmd == fDistToAxisY )
    { fDetectorConstruction -> SetDistToAxisY(fDistToAxisY->GetNewDoubleValue(newValue));}

  else if ( cmd == fDistToPC_Offset )
    { fDetectorConstruction -> SetDistToPC_Offset(fDistToPC_Offset->GetNewDoubleValue(newValue));}

  else if ( cmd == fMirrorCmd )
    { fDetectorConstruction -> SetMirrorOn(fMirrorCmd->GetNewBoolValue(newValue));}

  else if ( cmd == fStopperCmd )
    { fDetectorConstruction -> SetStopperOn(fStopperCmd->GetNewBoolValue(newValue)); }

  else if ( cmd == fPCPositionCmd )
    { fDetectorConstruction -> SetPCPosition(fPCPositionCmd->GetNewBoolValue(newValue)); }

  else if ( cmd == fCsITypeCmd )
    { fDetectorConstruction -> SetCsIType(fCsITypeCmd->GetNewBoolValue(newValue)); }

  else if ( cmd == fAngleCmd )
    { fDetectorConstruction -> SetAngle(fAngleCmd->GetNewDoubleValue(newValue)); }

  else if ( cmd == fThicknessCmd )
    { fDetectorConstruction -> SetThickness(fThicknessCmd->GetNewDoubleValue(newValue)); }

  else if ( cmd == fDefaultsCmd )
    {
      fDetectorConstruction -> SetDefaults();
      G4RunManager::GetRunManager()->ReinitializeGeometry();      
    }
 
}
