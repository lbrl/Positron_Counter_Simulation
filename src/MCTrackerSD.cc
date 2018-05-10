#include "MCTrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

MCTrackerSD::MCTrackerSD(G4String name) 
  : G4VSensitiveDetector(name),fHitsCollection(0)
{
  G4String HCname;
  collectionName.insert(HCname="MuonCollection");
  
}


MCTrackerSD::~MCTrackerSD() 
{}


void MCTrackerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsCollection 
    = new MCTrackerHitsCollection(SensitiveDetectorName, collectionName[0]);
   
  // Add this collection in hce

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );
  
}


G4bool MCTrackerSD::ProcessHits(G4Step* aStep, 
                                     G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;
  
  MCTrackerHit* newHit = new MCTrackerHit();
  
  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetPhysV(aStep->GetPreStepPoint()->GetTouchableHandle()
		    ->GetVolume());
  //  newHit->SetTime(aStep->GetPreStepPoint()->GetGlobalTime());

  fHitsCollection->insert( newHit );

  return true;
}


void MCTrackerSD::EndOfEvent(G4HCofThisEvent*)
{}

