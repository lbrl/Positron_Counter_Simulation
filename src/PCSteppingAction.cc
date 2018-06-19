#include "PCSteppingAction.hh"

#include "PCDetectorConstruction.hh"
#include "PCEventAction.hh"
#include "PCAnalysis.hh"

#include "G4Step.hh"

PCSteppingAction::PCSteppingAction(PCDetectorConstruction* det,
                                         PCEventAction* evt)
: G4UserSteppingAction(), 
  fDetector(det), fEventAction(evt)                                         
{ }

PCSteppingAction::~PCSteppingAction()
{ }

void PCSteppingAction::PCUserSteppingAction(const G4Step* aStep)
{
  // get volume of the origin (where muon stop)

  fEventAction->SetOrigin(3);  // 0 = beamTube, 1 = Collimator, 2 = Stopper, 3 = others

  if(aStep->GetTrack()->GetDefinition()->GetParticleName() == "e+")
    {
      if(aStep->GetTrack()->GetOriginTouchable()->GetVolume()->GetName() == "Collimator")
	{
	  fEventAction->SetOrigin(1);
	}

      // get logical volume of the current step
      G4LogicalVolume* volume 
	= aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
      
      // collect energy and track length step by step
      G4double edep = aStep->GetTotalEnergyDeposit();
      
      if (volume == fDetector->GetColl()) fEventAction->AddColl(edep);
      if (volume == fDetector->GetTube()) fEventAction->AddTube(edep);
      if (volume == fDetector->GetCham()) fEventAction->AddCham(edep);
      
    }
   
 
}
