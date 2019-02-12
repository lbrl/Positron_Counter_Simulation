
#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFileName("Simulation")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetActivation(true);
      
  
  // Open an output file
  //
  G4bool fileOpen = analysisManager->OpenFile();
  if (! fileOpen) {
    G4cerr << "\n---> HistoManager::Book(): cannot open " 
           << analysisManager->GetFileName() << G4endl;
    return;
  }
 
  
  // Create ntuples.
  // Ntuples ids are generated automatically starting from 0.
  // The start value can be changed by:
  // analysisManager->SetFirstMtupleId(1);  
  
  analysisManager->CreateNtuple("PC", "Edep");
  analysisManager->CreateNtupleDColumn("OriginX")  ; // column Id = 0
  analysisManager->CreateNtupleDColumn("OriginY") ; // column Id = 1
  analysisManager->CreateNtupleDColumn("TubeEdep"); // column Id = 2
  analysisManager->CreateNtupleDColumn("CollEdep"); // column Id = 3
  analysisManager->CreateNtupleDColumn("ChamEdep"); // column Id = 4
  analysisManager->CreateNtupleDColumn("PC1Time"); // column Id = 5
  analysisManager->CreateNtupleDColumn("PC2Time"); // column Id = 6
  analysisManager->CreateNtupleDColumn("PC3Time"); // column Id = 7
  analysisManager->CreateNtupleDColumn("PC1Edep"); // column Id = 8
  analysisManager->CreateNtupleDColumn("PC2Edep"); // column Id = 9
  analysisManager->CreateNtupleDColumn("PC3Edep"); // column Id = 10
  analysisManager->CreateNtupleDColumn("OriginZ") ; // column Id = 11
  analysisManager->CreateNtupleDColumn("TargetEdep"); // column Id = 12
  analysisManager->CreateNtupleDColumn("TargetX"); // column Id = 13
  analysisManager->CreateNtupleDColumn("TargetY"); // column Id = 14
  analysisManager->CreateNtupleDColumn("TargetZ"); // column Id = 15

  analysisManager->FinishNtuple();

  G4cout << "\n----> Output file is open in " 
         << analysisManager->GetFileName() << "." 
         << analysisManager->GetFileType() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{
   
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();    
  analysisManager->Write();
  analysisManager->CloseFile(); 
   
  G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
      
  delete G4AnalysisManager::Instance();
}


void HistoManager::FillNtupleEvent(G4double OriginX, G4double OriginY, G4double OriginZ, G4double TubeEdep, G4double CollEdep, G4double ChamEdep,
        G4double TargetEdep, G4double TargetX, G4double TargetY, G4double TargetZ)
{                
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // Fill ntuple
  analysisManager->FillNtupleDColumn(0, OriginX);
  analysisManager->FillNtupleDColumn(1, OriginY);
  analysisManager->FillNtupleDColumn(11, OriginZ);
  analysisManager->FillNtupleDColumn(2, TubeEdep);
  analysisManager->FillNtupleDColumn(3, CollEdep);
  analysisManager->FillNtupleDColumn(4, ChamEdep);
  analysisManager->FillNtupleDColumn(12, TargetEdep);
  analysisManager->FillNtupleDColumn(13, TargetX);
  analysisManager->FillNtupleDColumn(14, TargetY);
  analysisManager->FillNtupleDColumn(15, TargetZ);
  //  analysisManager->AddNtupleRow();  
 
}

void HistoManager::FillNtupleHC(G4double time1, G4double time2, G4double time3,
				G4double Edep1, G4double Edep2, G4double Edep3)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  analysisManager->FillNtupleDColumn(5, time1);
  analysisManager->FillNtupleDColumn(6, time2);
  analysisManager->FillNtupleDColumn(7, time3);
  analysisManager->FillNtupleDColumn(8, Edep1);
  analysisManager->FillNtupleDColumn(9, Edep2);
  analysisManager->FillNtupleDColumn(10, Edep3);
  analysisManager->AddNtupleRow();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
