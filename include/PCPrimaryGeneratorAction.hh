
#ifndef PCPrimaryGeneratorAction_h
#define PCPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

#include <fstream>

class G4ParticleGun;
class G4Event;
class G4Box;


class PCPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PCPrimaryGeneratorAction();    
    virtual ~PCPrimaryGeneratorAction();

     virtual void GeneratePrimaries(G4Event*);         
  
     const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
  private:
    std::ifstream ifin_primaries;
    G4ParticleGun*  fParticleGun; 
};

#endif
