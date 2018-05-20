
#ifndef PCRun_h
#define PCRun_h 1

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"

#include "globals.hh"

class PCRun : public G4Run {

public:
  
  PCRun();
  virtual ~PCRun();

public:
 
  virtual void RecordEvent(const G4Event*);
  void PrintAllCount();
  //return the totalcount of PC1
  G4int PC1TotalCount();
  G4double GetDistToAxis(G4int i);
  void WriteToText();

private:
  G4int fPCTrackerCollID;
  G4int fMCTrackerCollID;
  G4Box* fDistBox ;
  
  // G4int fPCTrackerCollID2;
  //  G4int fPCTrackerCollID3;
  
};

//

#endif
