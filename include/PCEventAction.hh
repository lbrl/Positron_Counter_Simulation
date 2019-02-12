
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
  void AddTarget(G4double de) {
      if( fTargetEdep < 0. ){
          fTargetEdep = 0.;
      }
      fTargetEdep += de;
  }

  //  void SetOrigin(G4int origin) {fOrigin = origin;}
  void SetVtxPositionX(G4double vtxX) { fVtxPositionX = vtxX ;}
  void SetVtxPositionY(G4double vtxY) { fVtxPositionY = vtxY ;}
  void SetVtxPositionZ(G4double vtxZ) { fVtxPositionZ = vtxZ ;}
  void SetTargetX(G4double x) { fTargetX = x; }
  void SetTargetY(G4double y) { fTargetY = y; }
  void SetTargetZ(G4double z) { fTargetZ = z; }
  
private:
  G4int fPCTrackerCollID;
  G4double fCollEdep, fTubeEdep, fChamEdep;
  G4double fVtxPositionX, fVtxPositionY, fVtxPositionZ;
  G4double fTargetEdep;
  G4double fTargetX, fTargetY, fTargetZ;
  //  G4int fOrigin;
  HistoManager* fHistoManager;
  
};


#endif
