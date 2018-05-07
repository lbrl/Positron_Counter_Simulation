
#ifndef PCRun_h
#define PCRun_h 1

#include "G4Run.hh"
#include "G4Event.hh"

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

private:
  G4int fPCTrackerCollID;
  // G4int fPCTrackerCollID2;
  //  G4int fPCTrackerCollID3;
  
};

//

#endif
