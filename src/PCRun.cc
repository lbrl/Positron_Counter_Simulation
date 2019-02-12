
#include "PCRun.hh"
#include "G4SDManager.hh"
#include "PCDetectorConstruction.hh"
#include "PCTrackerSD.hh"
#include "MCTrackerSD.hh"
#include <iostream>
#include <fstream>
#include "HistoManager.hh"

#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

G4int fPC1TotalCount, fPC2TotalCount, fPC3TotalCount, f23Coin, fallCoin;
G4int fMCTotalCount ;
G4int fTotalEvent; 

PCRun::PCRun()
  : G4Run(),
    fPCTrackerCollID(-1),
    fMCTrackerCollID(-1)
 
{
  // set totalcount,coincount of Positroncounters,Muoncounter to 0 for each Run

  fPC1TotalCount = 0 ;
  fPC2TotalCount = 0 ;
  fPC3TotalCount = 0 ;
  f23Coin = 0 ;
  fallCoin = 0 ;
  fMCTotalCount = 0;
 
  // Count total event (launched muon number) from zero
  fTotalEvent = 0;
  
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(fPCTrackerCollID<0)
    {
      G4String colName;
      fPCTrackerCollID = SDman->GetCollectionID(colName="PositronCollection");
    }
  if(fMCTrackerCollID<0)
    {
      G4String colName;
      fMCTrackerCollID = SDman->GetCollectionID(colName="MuonCollection");
    }
    
}


PCRun::~PCRun()
{}

//  RecordEvent is called at end of event.
//  For accumulate the PositronCount, the resultant quantity in a event,
//  is accumulated during a Run.
void PCRun::RecordEvent(const G4Event* event)
{
  fTotalEvent+=1;
  
  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  PCTrackerHitsCollection* PCHC=0;
  MCTrackerHitsCollection* MCHC=0;
  
  if(HCE)
    {
      PCHC = (PCTrackerHitsCollection*)(HCE->GetHC(fPCTrackerCollID));
      MCHC = (MCTrackerHitsCollection*)(HCE->GetHC(fMCTrackerCollID));
    }
  
    // Reset the count,time for each event
  G4int PC1count = 0, PC2count = 0, PC3count = 0;
  G4double PC1time = 0., PC2time = 0., PC3time =0. ;
  G4double PC1Edep = 0., PC2Edep = 0., PC3Edep =0. ;
  G4int MCcount=0;
  
     G4int nofPCHits = PCHC->entries();
     //    G4int nofHits2 = PCHC2->entries();
     //     G4int nofHits3 = PCHC3->entries();

     if(PCHC)
       {
	 for ( G4int i=0 ; i < nofPCHits ; i++ )
	   {
	     PCTrackerHit *PCHit = (*PCHC)[nofPCHits-1-i];
	     if(PCHit->GetPhysV()->GetName()=="PositronCounter1")
	       {
		 PC1count=1;
		 PC1time=PCHit->GetTime();
		 PC1Edep += PCHit->GetEdep();
	       }
	     if(PCHit->GetPhysV()->GetName()=="PositronCounter2")
	       {
		 PC2count=1;
		 PC2time=PCHit->GetTime();
		 PC2Edep += PCHit->GetEdep();
	       }
	     if(PCHit->GetPhysV()->GetName()=="PositronCounter3")
	       {
		 PC3count=1;
		 PC3time=PCHit->GetTime();
		 PC3Edep += PCHit->GetEdep();
	       }
	   }
       }

          G4int nofMCHits = MCHC->entries();

     if(MCHC)
       {
	 for ( G4int i=0 ; i < nofMCHits ; i++ )
	   {
	     MCTrackerHit *MCHit = (*MCHC)[nofMCHits-1-i];
	     if(MCHit->GetPhysV()->GetName()=="CsI_Tl")
	       MCcount=1;
	   }
        }
     

     if(MCcount==1)
       fMCTotalCount+=1;
     

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

     fHistoManager->FillNtupleHC(PC1time/ns, PC2time/ns, PC3time/ns,
				 PC1Edep/keV, PC2Edep/keV , PC3Edep/keV);		 
     
     /*     auto man = G4AnalysisManager::Instance();
     man -> FillNtupleDColumn(4,PC1time);
     man -> FillNtupleDColumn(5,PC2time);
     man -> FillNtupleDColumn(6,PC3time);
     man -> FillNtupleDColumn(7,PC1Edep);
     man -> FillNtupleDColumn(8,PC2Edep);
     man -> FillNtupleDColumn(9,PC3Edep);
     man -> AddNtupleRow();
     */	 

}



G4int PCRun::PC1TotalCount() {
  // const PCRun * localRun = static_cast<const PCRun *>(aRun);
  
  // return totalcount of PC1
  return fPC1TotalCount;
}

G4double PCRun::GetDistToAxis(G4int axis) {

  const PCDetectorConstruction* PCdetector
    = static_cast<const PCDetectorConstruction*>
    (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  G4double DistToAxisX = PCdetector->DistToAxisX ;
  G4double DistToAxisY = PCdetector->DistToAxisY ;
  G4double DistToPC_Offset = PCdetector->DistToPC_Offset;

  if(axis==1)
    return DistToAxisX;
  else if(axis==2)
    return DistToAxisY;
  else if(axis==3)
    return DistToPC_Offset;
  else
    return 100000. ; // indicates error
 
}


void PCRun::PrintAllCount() {

   G4cout << "PC1 totalcount = " << fPC1TotalCount << G4endl
	  << "PC2 totalcount = " << fPC2TotalCount << G4endl
	  << "PC3 totalcount = " << fPC3TotalCount << G4endl
	  << "PC2&3 Coincidence count = " << f23Coin << G4endl
	  << "PCAll Coincidence count = " << fallCoin << G4endl
	  << "MC totalcount = " << fMCTotalCount << G4endl
	  << "Total launched Muon = " << fTotalEvent << G4endl 
	  << "DistToAxisX = " << GetDistToAxis(1) << G4endl 
	  << "DistToAxisY = " << GetDistToAxis(2) << G4endl
	  << "DistToPC_Offset = " << GetDistToAxis(2) << G4endl ;
	
}


void PCRun::WriteToText() {

  std::ofstream out("SimulationResult.txt",std::ios::app);

  if(out.is_open()){
    out << GetDistToAxis(1) << "\t" << GetDistToAxis(2) << "\t" << GetDistToAxis(3) << "\t" << fPC1TotalCount << "\t"
	<< fPC2TotalCount << "\t" << fPC3TotalCount << "\t" << f23Coin << "\t"
	<< fallCoin << "\t" << fMCTotalCount << "\t" << fTotalEvent << "\n" ;

      }

}
