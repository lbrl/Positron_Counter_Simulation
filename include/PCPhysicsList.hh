
#ifndef PCPhysicsList_h
#define PCPhysicsList_h 1

#include "G4VModularPhysicsList.hh"


class PCPhysicsList: public G4VModularPhysicsList
{
public:
  PCPhysicsList();
  virtual ~PCPhysicsList();

  virtual void SetCuts();
  virtual void ConstructProcess();


};


#endif

