
#ifndef PCTrackerHit_h
#define PCTrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "tls.hh"


/// Tracker hit class
///
/// It defines data members to store the time, LogicVolume, trackID of e+

class PCTrackerHit : public G4VHit
{
  public:
    PCTrackerHit();
    PCTrackerHit(const PCTrackerHit&);
    virtual ~PCTrackerHit();

    // operators
    const PCTrackerHit& operator=(const PCTrackerHit&);
    G4int operator==(const PCTrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetPhysV   (G4VPhysicalVolume* phyV)      { fVolume = phyV; };
    void SetTime     (G4double time){ fTime = time; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4VPhysicalVolume* GetPhysV() const   { return fVolume; };
    G4double GetTime() const     { return fTime; };
    
  private:

      G4int fTrackID;
      G4double fTime;
      G4VPhysicalVolume*  fVolume;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<PCTrackerHit> PCTrackerHitsCollection;

extern G4ThreadLocal G4Allocator<PCTrackerHit>* PCTrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* PCTrackerHit::operator new(size_t)
{
  if(!PCTrackerHitAllocator)
      PCTrackerHitAllocator = new G4Allocator<PCTrackerHit>;
  return (void *) PCTrackerHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PCTrackerHit::operator delete(void *hit)
{
  PCTrackerHitAllocator->FreeSingle((PCTrackerHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
