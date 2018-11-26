// initial setting to 3.5MeV +muon

#include "PCPrimaryGeneratorAction.hh"


#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


PCPrimaryGeneratorAction::PCPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0) 
{

	G4int n_particle = 1;
	fParticleGun = new G4ParticleGun(n_particle);
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	G4ParticleDefinition* particle
	  = particleTable->FindParticle(particleName="mu+");
	fParticleGun->SetParticleDefinition(particle);
	
/*	G4double positionx = G4RandGauss::shoot(0,20.27);
	positionx *= mm;	
	G4double positiony = G4RandGauss::shoot(0,25.02);
	positiony *= mm;
	G4cout<<"position = "<<positionx<<":"<<positiony<<G4endl;
	fParticleGun->SetParticlePosition(G4ThreeVector(positionx,positiony,-277.796 *mm));
*/
  

//  fParticleGun->SetParticlePosition(G4ThreeVector(0.,-30.*cm, 0.));
//  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.));
  fParticleGun->SetParticleEnergy(3.5*MeV); // Kinematic Energy for muon momentum = 27.4 MeV
}


PCPrimaryGeneratorAction::~PCPrimaryGeneratorAction()
{
  delete fParticleGun;
}



void PCPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of ecah event

  G4double StartPointY = (-657.4)*mm; // last simulation : -347.25mm is used
  
	G4ParticleDefinition* particle;
	particle = fParticleGun->GetParticleDefinition();
	//	G4double Ekin = 4.1*MeV; 
	//	fParticleGun->SetParticleEnergy(Ekin);
//	G4Random::setTheEngine(CLHEP::HepRandomEngine*);
//	HepRandom::setTHeSeed(2);
	
	G4double anglex = G4RandGauss::shoot(0,0.0376);
	anglex *= rad;
	G4double anglez = G4RandGauss::shoot(0,0.0438);
	anglez *= rad;
	//	G4cout<<anglex<<":"<<angley<<G4endl;
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::tan(anglex),1*rad,std::tan(anglez)));

	//fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,1*rad,0));
	
	G4double positionx = G4RandGauss::shoot(0,27.55); //original : 25.02
        positionx *= mm;
        G4double positionz = G4RandGauss::shoot(0,21.03); //original : 20.27
        positionz *= mm;

	// G4cout<<"position = "<<positionx<<":"<<positiony<<G4endl;
        fParticleGun->SetParticlePosition(G4ThreeVector(positionx,StartPointY,positionz));

  
  fParticleGun->GeneratePrimaryVertex(anEvent);
}


