
#ifndef PCTrackerHit_h
#define PCTrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "tls.hh"


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
    void SetEdep     (G4double de)    {fEdep = de;}

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4VPhysicalVolume* GetPhysV() const   { return fVolume; };
    G4double GetTime() const     { return fTime; };
    G4double GetEdep() const { return fEdep; }
    
  private:

      G4int fTrackID;
      G4double fTime;
      G4VPhysicalVolume*  fVolume;
      G4double fEdep;
};


typedef G4THitsCollection<PCTrackerHit> PCTrackerHitsCollection;

extern G4ThreadLocal G4Allocator<PCTrackerHit>* PCTrackerHitAllocator;


inline void* PCTrackerHit::operator new(size_t)
{
  if(!PCTrackerHitAllocator)
      PCTrackerHitAllocator = new G4Allocator<PCTrackerHit>;
  return (void *) PCTrackerHitAllocator->MallocSingle();
}


inline void PCTrackerHit::operator delete(void *hit)
{
  PCTrackerHitAllocator->FreeSingle((PCTrackerHit*) hit);
}


#endif
