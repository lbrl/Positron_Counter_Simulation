
#include "PCRun.hh"
#include "G4SDManager.hh"

#include "PCTrackerSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//  Constructor. 

G4int fPC1TotalCount, fPC2TotalCount, fPC3TotalCount, f23Coin, fallCoin;

PCRun::PCRun()
  : G4Run(),
    fPCTrackerCollID(-1)
{
  // set totalcount,coincount of Positroncounters to 0 for each Run
  fPC1TotalCount=0;
  fPC2TotalCount=0;
  fPC3TotalCount=0;
  f23Coin=0;
  fallCoin=0;
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(fPCTrackerCollID<0)
    {
      G4String colName;
      fPCTrackerCollID = SDman->GetCollectionID(colName="PositronCollection");
    }

    
}

// Destructor
//    clear all data members.
PCRun::~PCRun()
{}

//  RecordEvent is called at end of event.
//  For accumulate the PositronCount, the resultant quantity in a event,
//  is accumulated during a Run.
void PCRun::RecordEvent(const G4Event* event)
{
 G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  PCTrackerHitsCollection* PCHC=0;
  
  if(HCE)
    {
      PCHC = (PCTrackerHitsCollection*)(HCE->GetHC(fPCTrackerCollID));
    }
  
    // Reset the count,time for each event
  G4int PC1count = 0, PC2count = 0, PC3count = 0;
  G4double PC1time = 0., PC2time = 0., PC3time =0. ;
 
     G4int nofHits = PCHC->entries();
     //    G4int nofHits2 = PCHC2->entries();
     //     G4int nofHits3 = PCHC3->entries();

     if(PCHC)
       {
	 for ( G4int i=0 ; i < nofHits ; i++ )
	   {
	     PCTrackerHit *PCHit = (*PCHC)[nofHits-1-i];
	     if(PCHit->GetPhysV()->GetName()=="PositronCounter1")
	       {
		 PC1count=1;
		 PC1time=PCHit->GetTime();
	       }
	     if(PCHit->GetPhysV()->GetName()=="PositronCounter2")
	       {
		 PC2count=1;
		 PC2time=PCHit->GetTime();
	       }
	     if(PCHit->GetPhysV()->GetName()=="PositronCounter3")
	       {
		 PC3count=1;
		 PC3time=PCHit->GetTime();
	       }
	   }
       }
   

     /*     G4cout << G4endl
	    << "PC1count = " << PC1count << G4endl
	    << "PC2count = " << PC2count << G4endl
	    << "PC3count = " << PC3count << G4endl;
     */

     //if there is a positron, count it
	 if(PC1count==1)
	   fPC1TotalCount+=1;
	 if(PC2count==1)
	   fPC2TotalCount+=1;
         if(PC3count==1)
	   fPC3TotalCount+=1;

	 //count coincidence by arrival time of positron ( PC#time==0 should be considered, but assume that positrons will always go 1->2->3)

	 if(PC2time!=0 && PC2time<PC3time)
	   f23Coin+=1;

	 if(PC1time*PC2time!=0 && PC1time < PC2time && PC2time < PC3time)
	   fallCoin+=1;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int PCRun::PC1TotalCount() {
  // const PCRun * localRun = static_cast<const PCRun *>(aRun);
  
  // return totalcount of PC1
  return fPC1TotalCount;
}


void PCRun::PrintAllCount() {

  G4cout << "PC1 totalcount = " << fPC1TotalCount << G4endl
	 << "PC2 totalcount = " << fPC2TotalCount << G4endl
	 << "PC3 totalcount = " << fPC3TotalCount << G4endl
	 << "PC2&3 Coincidence count = " << f23Coin << G4endl
	 << "PCAll Coincidence count = " << fallCoin << G4endl ;
	
}
