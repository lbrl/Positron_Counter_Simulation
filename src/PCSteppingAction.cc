#include "PCSteppingAction.hh"

#include "PCDetectorConstruction.hh"
#include "PCEventAction.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

PCSteppingAction::PCSteppingAction(PCDetectorConstruction* det, PCEventAction* evt)
: G4UserSteppingAction(), 
  fEventAction(evt), fDetector(det)         
{ }

PCSteppingAction::~PCSteppingAction()
{ }

void PCSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // get volume of the origin (where muon stop)

  G4Track* aTrack = aStep->GetTrack();
  
  if(aTrack->GetDefinition()->GetParticleName() == "e+")
    {	 
      fEventAction->SetVtxPositionX(aTrack->GetVertexPosition().x()/mm) ;
      fEventAction->SetVtxPositionY(aTrack->GetVertexPosition().y()/mm) ;
      fEventAction->SetVtxPositionZ(aTrack->GetVertexPosition().z()/mm) ;
      
      /*
	if(aStep->GetTrack()->GetOriginTouchable()->GetVolume()->GetName() == "Collimator")
	{
	  fEventAction->SetOrigin(1); // 0 = beamTube, 1 = Collimator, 2 = Stopper, 3 = others
	  G4cout << "Collimator is hitted" << G4endl;
	}
      */
      
      // get logical volume of the current step
      G4LogicalVolume* volume 
	= aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
            
      // collect energy and track length step by step
      G4double edep = aStep->GetTotalEnergyDeposit();
      // if (edep <= 0.) return;
      
      if (volume == fDetector->GetTube()) fEventAction->AddColl(edep);
      if (volume == fDetector->GetColl()) fEventAction->AddTube(edep);
      if (volume == fDetector->GetCham()) fEventAction->AddCham(edep);

      
    }
  if(aTrack->GetDefinition()->GetParticleName() == "mu+"){
      G4LogicalVolume* volume 
        = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
      G4double edep = aStep->GetTotalEnergyDeposit();
      if( volume == fDetector->GetTarget() ){
          fEventAction->AddTarget( edep );
          G4double x = aTrack->GetPosition().x()/mm;
          G4double y = aTrack->GetPosition().y()/mm;
          G4double z = aTrack->GetPosition().z()/mm;
          fEventAction->SetTargetX( x );
          fEventAction->SetTargetY( y );
          fEventAction->SetTargetZ( z );
      }
  }
  
  
}
