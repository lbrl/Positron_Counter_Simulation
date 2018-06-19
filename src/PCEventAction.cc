
#include "PCEventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "PCTrackerHit.hh"
#include "G4SDManager.hh"
#include "PCAnalysis.hh"


PCEventAction::PCEventAction()
: G4UserEventAction(),
  fPCTrackerCollID(-1), fTubeEdep(0.), fCollEdep(0.), fChamEdep(0.), fOrigin(3)
{}


PCEventAction::~PCEventAction()
{}


void PCEventAction::BeginOfEventAction(const G4Event*)
{
  fTubeEdep = 0.;
  fCollEdep = 0.;
  fChamEdep = 0.;
  fOrigin   = 3 ;
}


void PCEventAction::EndOfEventAction(const G4Event* event)
{
  G4int eventID = event->GetEventID();
  if ( eventID < 100 || eventID % 100 == 0)
    {    G4cout << ">>> Event: " << eventID  << G4endl;    }
  

  auto man = G4AnalysisManager::Instance();
  man->FillNtupleIColumn(0,fOrigin);
  man->FillNtupleDColumn(1,fTubeEdep);
  man->FillNtupleDColumn(2,fCollEdep);
  man->FillNtupleDColumn(3,fChamEdep);
  
}  

