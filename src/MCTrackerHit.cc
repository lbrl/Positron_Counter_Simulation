#include "MCTrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<MCTrackerHit>* MCTrackerHitAllocator=0;


MCTrackerHit::MCTrackerHit()
 : G4VHit(),
   fTrackID(-1),
   fVolume(0)
   // fTime(0.)
{}


MCTrackerHit::~MCTrackerHit() {}


MCTrackerHit::MCTrackerHit(const MCTrackerHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  fVolume    = right.fVolume;
  //  fTime      = right.fTime;
}


const MCTrackerHit& MCTrackerHit::operator=(const MCTrackerHit& right)
{
  fTrackID   = right.fTrackID;
  fVolume    = right.fVolume;
  //  fTime      = right.fTime;

  return *this;
}


G4int MCTrackerHit::operator==(const MCTrackerHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}





void MCTrackerHit::Print()
{
  G4cout
    << "  trackID: " << fTrackID << " LogicalVolume: " << fVolume->GetName()
    //     << "Time: "
    //     << std::setw(7) << G4BestUnit(fTime,"Time")
     << G4endl;
}

