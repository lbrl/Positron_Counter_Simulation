
#include "PCActionInitialization.hh"
#include "PCPrimaryGeneratorAction.hh"
#include "PCRunAction.hh"
#include "PCEventAction.hh"



PCActionInitialization::PCActionInitialization()
 : G4VUserActionInitialization()
{}


PCActionInitialization::~PCActionInitialization()
{}


void PCActionInitialization::BuildForMaster() const
{
    SetUserAction(new PCRunAction);
}


void PCActionInitialization::Build() const
{
  SetUserAction(new PCPrimaryGeneratorAction);
  SetUserAction(new PCRunAction);
  SetUserAction(new PCEventAction);
}  

