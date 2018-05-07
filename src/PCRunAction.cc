#include "B1DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "PCRunAction.hh"
#include <vector>
#include "G4Run.hh"
#include "PCTrackerSD.hh"
#include "PCRun.hh"

PCRunAction::PCRunAction()
  : G4UserRunAction()
{
  // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(1000);     
}

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
}

void PCRunAction::EndOfRunAction(const G4Run* aRun)
{
  if(!IsMaster()) return;

  PCRun* theRun = (PCRun*)aRun;

  //  G4cout << "Total Count of PC1 = " << theRun->PC1TotalCount() << G4endl;


  theRun->PrintAllCount();
  

}
