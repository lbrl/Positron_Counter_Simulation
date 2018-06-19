#include "PCDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "PCRunAction.hh"
#include <vector>
#include "G4Run.hh"
#include "PCTrackerSD.hh"
#include "PCRun.hh"
#include "PCAnalysis.hh"

PCRunAction::PCRunAction()
  : G4UserRunAction()
{
  // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(1000);

  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  
  auto man = G4AnalysisManager::Instance();
  man->SetNtupleMerging(true);
  man->SetVerboseLevel(1);
  man->SetFileName("Simulation");

  man->CreateNtuple("PC", "Origin, Edep, Angular Distribution, Time");
  man->CreateNtupleIColumn("Origin"); // column ID = 0, 0->beamTube, 1->Collimator, 2-> Stopper, 3-> Others
  man->CreateNtupleDColumn("TubeEdep"); // column ID = 1
  man->CreateNtupleDColumn("CollEdep"); // column ID = 2
  man->CreateNtupleDColumn("ChamEdep"); // column ID = 3
  man->CreateNtupleDColumn("PC1Time");  // column ID = 4
  man->CreateNtupleDColumn("PC2Time");  // column ID = 5
  man->CreateNtupleDColumn("PC3Time");  // column ID = 6
  man->CreateNtupleDColumn("PC1Edep");  // column ID = 7
  man->CreateNtupleDColumn("PC2Edep");  // column ID = 8
  man->CreateNtupleDColumn("PC3Edep");  // column ID = 9
  //  man->CreateNtupleDColumn("InnerAD");
  //  man->CreateNtupleDColumn("OuterAD");

  man->FinishNtuple();

}

PCRunAction::~PCRunAction()
{
  delete G4AnalysisManager::Instance();
}

G4Run* PCRunAction::GenerateRun()
{
  return new PCRun();
}

void PCRunAction::BeginOfRunAction(const G4Run*)
{
  //  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  //  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile();
  
}

void PCRunAction::EndOfRunAction(const G4Run* aRun)
{
  if(!IsMaster()) return;

  PCRun* theRun = (PCRun*)aRun;

  //  G4cout << "Total Count of PC1 = " << theRun->PC1TotalCount() << G4endl;


  theRun->PrintAllCount();
  theRun->WriteToText();


  // save ntuple
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  // ntuple can be accesed by G4Ntuple* ntuple = man->GetNtuple(ntupleID)
  
}
