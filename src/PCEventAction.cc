
#include "PCEventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "PCTrackerHit.hh"
#include "G4SDManager.hh"


PCEventAction::PCEventAction()
: G4UserEventAction(),
  fPCTrackerCollID(-1)
{}


PCEventAction::~PCEventAction()
{}


void PCEventAction::BeginOfEventAction(const G4Event*)
{
  /*

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(fPCTrackerCollID<0)
    {
      G4String colName;
      fPCTrackerCollID = SDman->GetCollectionID(colName="PositronCounter");
    }

  */
}


void PCEventAction::EndOfEventAction(const G4Event* event)
{


  G4int eventID = event->GetEventID();
  if ( eventID < 100 || eventID % 100 == 0)
    {    G4cout << ">>> Event: " << eventID  << G4endl;    }


  /*
    G4HCofThisEvent* HCE = event->GetHCofThisEvent();
    PCTrackerHitsCollection* PCHC=0;

  if(HCE)
    {
      PCHC = (PCTrackerHitsCollection*)(HCE->GetHC(fPCTrackerCollID));
    }
  
    // Reset the count,time for each event
  G4int PC1count = 0;
  G4double PC1time = 0.;
  //  if ( verboseLevel>1 ) { 
     G4int nofHits = PCHC->entries();
     G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits 
            << " hits in the tracker : " << G4endl;
     for ( G4int i=0 ; i < nofHits ; i++ )
       {
	 PCTrackerHit *PCHit = (*PCHC)[nofHits-1-i];
	 if(PCHit->GetLogicV()->GetName()=="Alelectrode")
	   {
	     PC1count=1;
	     PC1time=PCHit->GetTime();
	   }
       }
     G4cout << G4endl
	    << "PC1count = " << PC1count <<G4endl;
  //  }

  */ 
  
}  

