
#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"

#include "g4root.hh"
//#include "g4csv.hh"
//#include "g4xml.hh"

class HistoManager
{
  public:
    HistoManager();
   ~HistoManager();

    void Book();
    void Save();
    
  void FillNtupleEvent(G4double OriginX, G4double OriginY,  G4double TubeEdep, G4double CollEdep, G4double ChamEdep);
  void FillNtupleHC(G4double time1, G4double time2, G4double time3,
		    G4double Edep1, G4double Edep2, G4double Edep3);

  
  private:
  G4String fFileName;
};


#endif

