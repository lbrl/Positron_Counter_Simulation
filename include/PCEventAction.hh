
#ifndef PCEventAction_h
#define PCEventAction_h 1

#include "G4UserEventAction.hh"

#include "globals.hh"

/// Event action class

class HistoManager;

class PCEventAction : public G4UserEventAction
{
  public:
    PCEventAction(HistoManager*);
    virtual ~PCEventAction();

    virtual void  BeginOfEventAction(const G4Event* );
    virtual void    EndOfEventAction(const G4Event* );

  void AddColl(G4double de) {fCollEdep += de;}
  void AddTube(G4double de) {fTubeEdep += de;}
  void AddCham(G4double de) {fChamEdep += de;}

  //  void SetOrigin(G4int origin) {fOrigin = origin;}
  void SetVtxPositionX(G4double vtxX) { fVtxPositionX = vtxX ;}
  void SetVtxPositionY(G4double vtxY) { fVtxPositionY = vtxY ;}
  
private:
  G4int fPCTrackerCollID;
  G4double fCollEdep, fTubeEdep, fChamEdep;
  G4double fVtxPositionX, fVtxPositionY ;
  //  G4int fOrigin;
  HistoManager* fHistoManager;
  
};


#endif
