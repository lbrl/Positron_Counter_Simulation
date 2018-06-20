#include "PCDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "PCRunAction.hh"
#include <vector>
#include "G4Run.hh"
#include "PCTrackerSD.hh"
#include "PCRun.hh"
#include "HistoManager.hh"

PCRunAction::PCRunAction(HistoManager* histo)
  : G4UserRunAction(),
    fHistoManager(histo)
{}

PCRunAction::~PCRunAction()
{}

G4Run* PCRunAction::GenerateRun()
{
  return new PCRun();
}

void PCRunAction::BeginOfRunAction(const G4Run*)
{
  //  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  //  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  fHistoManager->Book();
  
}

void PCRunAction::EndOfRunAction(const G4Run* aRun)
{
  if(!IsMaster()) return;

  PCRun* theRun = (PCRun*)aRun;

  //  G4cout << "Total Count of PC1 = " << theRun->PC1TotalCount() << G4endl;


  theRun->PrintAllCount();
  theRun->WriteToText();


  fHistoManager->Save();

  // ntuple can be accesed by G4Ntuple* ntuple = man->GetNtuple(ntupleID)
  
}
