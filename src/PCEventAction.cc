
#include "PCEventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "PCTrackerHit.hh"
#include "G4SDManager.hh"
#include "HistoManager.hh"


PCEventAction::PCEventAction(HistoManager* histo)
: G4UserEventAction(),
  fPCTrackerCollID(-1), fTubeEdep(0.), fCollEdep(0.), fChamEdep(0.),
  fTargetEdep(-666.),
  fVtxPositionX(0.), fVtxPositionY(0.), fVtxPositionZ(0.),
  fTargetX(0.), fTargetY(0.), fTargetZ(0.),
  fHistoManager(histo)
{}


PCEventAction::~PCEventAction()
{}


void PCEventAction::BeginOfEventAction(const G4Event*)
{
  fTubeEdep = 0.;  fCollEdep = 0.;  fChamEdep = 0.;
  fTargetEdep = -666.;
  fVtxPositionX = 0. ;
  fVtxPositionY = 0. ;
  fVtxPositionZ = 0. ;
  fTargetX = 0.;
  fTargetY = 0.;
  fTargetZ = 0.;
  //  fOrigin   = 3 ; // others
}


void PCEventAction::EndOfEventAction(const G4Event* event)
{
  
  G4int eventID = event->GetEventID();
  if ( eventID % 1000 == 0)
    {    G4cout << ">>> Event: " << eventID  << G4endl;    }
  

  fHistoManager->FillNtupleEvent(fVtxPositionX, fVtxPositionY, fVtxPositionZ, fTubeEdep, fCollEdep, fChamEdep,
          fTargetEdep, fTargetX, fTargetY, fTargetZ);
}  

