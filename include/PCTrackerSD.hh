
#ifndef PCTrackerSD_h
#define PCTrackerSD_h 1

#include "G4VSensitiveDetector.hh"

#include "PCTrackerHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class PCTrackerSD : public G4VSensitiveDetector
{
  public:
    PCTrackerSD(G4String name);
    virtual ~PCTrackerSD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
  PCTrackerHitsCollection* fHitsCollection;
  // PCTrackerHitsCollection* fHitsCollection2;
  //PCTrackerHitsCollection* fHitsCollection3;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
