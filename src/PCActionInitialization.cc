
#include "PCActionInitialization.hh"
#include "PCPrimaryGeneratorAction.hh"
#include "PCRunAction.hh"
#include "PCEventAction.hh"
#include "PCSteppingAction.hh"
#include "HistoManager.hh"



PCActionInitialization::PCActionInitialization(PCDetectorConstruction* det)
  : G4VUserActionInitialization(),
    fDetector(det)
{}


PCActionInitialization::~PCActionInitialization()
{}


void PCActionInitialization::BuildForMaster() const
{
  HistoManager* histo = new HistoManager();
  
  SetUserAction(new PCRunAction(histo));
}


void PCActionInitialization::Build() const
{
    // Histo manager
  HistoManager* histo = new HistoManager();
  
  // Actions
  //
  SetUserAction(new PCPrimaryGeneratorAction());
  
  PCRunAction* runAction = new PCRunAction(histo);  
  SetUserAction(runAction);
  
  PCEventAction* eventAction = new PCEventAction(histo);
  SetUserAction(eventAction);

  PCSteppingAction* steppingAction = new PCSteppingAction(fDetector, eventAction);
  SetUserAction(steppingAction);
}  

