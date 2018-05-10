
#ifndef MCTrackerHit_h
#define MCTrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "tls.hh"


class MCTrackerHit : public G4VHit
{
  public:
    MCTrackerHit();
    MCTrackerHit(const MCTrackerHit&);
    virtual ~MCTrackerHit();

    // operators
    const MCTrackerHit& operator=(const MCTrackerHit&);
    G4int operator==(const MCTrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetPhysV   (G4VPhysicalVolume* phyV)      { fVolume = phyV; };
  //    void SetTime     (G4double time){ fTime = time; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4VPhysicalVolume* GetPhysV() const   { return fVolume; };
  //    G4double GetTime() const     { return fTime; };
    
  private:

      G4int fTrackID;
  //      G4double fTime;
      G4VPhysicalVolume*  fVolume;
};


typedef G4THitsCollection<MCTrackerHit> MCTrackerHitsCollection;

extern G4ThreadLocal G4Allocator<MCTrackerHit>* MCTrackerHitAllocator;


inline void* MCTrackerHit::operator new(size_t)
{
  if(!MCTrackerHitAllocator)
      MCTrackerHitAllocator = new G4Allocator<MCTrackerHit>;
  return (void *) MCTrackerHitAllocator->MallocSingle();
}


inline void MCTrackerHit::operator delete(void *hit)
{
  MCTrackerHitAllocator->FreeSingle((MCTrackerHit*) hit);
}


#endif
