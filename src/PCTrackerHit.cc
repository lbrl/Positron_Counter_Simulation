
#include "PCTrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<PCTrackerHit>* PCTrackerHitAllocator=0;


PCTrackerHit::PCTrackerHit()
 : G4VHit(),
   fTrackID(-1),
   fVolume(0),
   fTime(0.)
{}


PCTrackerHit::~PCTrackerHit() {}


PCTrackerHit::PCTrackerHit(const PCTrackerHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  fVolume    = right.fVolume;
  fTime      = right.fTime;
}


const PCTrackerHit& PCTrackerHit::operator=(const PCTrackerHit& right)
{
  fTrackID   = right.fTrackID;
  fVolume    = right.fVolume;
  fTime      = right.fTime;

  return *this;
}


G4int PCTrackerHit::operator==(const PCTrackerHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}





void PCTrackerHit::Print()
{
  G4cout
    << "  trackID: " << fTrackID << " LogicalVolume: " << fVolume->GetName()
     << "Time: "
     << std::setw(7) << G4BestUnit(fTime,"Time")
     << G4endl;
}

