
#include "PCDetectorConstruction.hh"
#include "PCDetectorMessenger.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VisAttributes.hh"

#include "MCTrackerSD.hh"
#include "PCTrackerSD.hh"
#include "G4SDManager.hh"
#include "G4VSDFilter.hh"
#include "G4SDParticleWithEnergyFilter.hh"


PCDetectorConstruction::PCDetectorConstruction()
  : G4VUserDetectorConstruction(),
    fPC_logic(NULL),
    fTarget(NULL),
    DistToAxisX(0.),DistToAxisY(0.)
{
  fMessenger = new PCDetectorMessenger(this);
  
  fPC_logic = new G4LogicalVolume*[3];
}


PCDetectorConstruction::~PCDetectorConstruction()
{
  delete [] fPC_logic ;
  delete fTarget ;
  delete fMessenger;

}


G4VPhysicalVolume* PCDetectorConstruction::Construct()
{
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //Define materials
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  G4String name,symbol;
  G4double a,z,density,pressure,temperature,fraction;
  G4int ncomp,natoms;
  a= 12.01*g/mole;
  G4Element* elC = new G4Element(name="Carbon", symbol="C", z=6., a);
  a= 18.998*g/mole;
  G4Element* elN = new G4Element(name="Nitrogen", symbol="N", z=7., a);
  a= 1.01*g/mole;
  G4Element* elH = new G4Element(name="Hydrogen", symbol="H", z=1., a);
  a= 16.00*g/mole;
  G4Element* elO = new G4Element(name="Oxygen", symbol="O", z=8., a);

  G4Material* Al = nist->FindOrBuildMaterial("G4_Al");
  // G4Material* Al = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  // G4Material* Al2O3 = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  
  a= 26.98*g/mole;
  G4Element* elAl = new G4Element(name="Aluminum",symbol="Al",z=13.,a);
  G4Material* Air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");

  G4Material* CsI = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  G4Material* Tl = nist->FindOrBuildMaterial("G4_Tl");
   
  //  pressure = 0.0001*atmosphere ;
  //  density = 0.0012*g/cm3;
  density = 2.376e-15*g/cm3;
  pressure = 2.0e-7*bar;
  temperature = 288.*kelvin;
  G4Material* vacuum = new G4Material(name="vacuum", density, ncomp=1,
				      kStateGas,temperature,pressure);
  vacuum->AddMaterial(Air,fraction=100*perCent);
 
  density = 1.39*g/cm3;
  G4Material* Mylar = new G4Material(name="Mylar", density, ncomp=3);
  Mylar->AddElement(elO,2);
  Mylar->AddElement(elC,5);
  Mylar->AddElement(elH,4);
    
  density = 2.7*g/cm3;
  G4Material* Albuild = new G4Material(name="Albuild",density,ncomp=1);
  Albuild->AddElement(elAl, natoms=1);

  density = 4.51*g/cm3 ;
  G4Material* targetMaterial = new G4Material(name="CsI_Tl", density, ncomp = 2);
  targetMaterial->AddMaterial(CsI,fraction=99.999*perCent);
  targetMaterial->AddMaterial(Tl,fraction=0.001*perCent);

   
  // Option to switch on/off checking of volumes overlaps
 
  G4bool checkOverlaps = true; // it's not important
 
  // World
  G4double worldX  = 100.*cm, worldY = 300.*cm, worldZ = 250.*cm;
  
  G4Box* world_solid = new G4Box("World", 0.5*worldX, 0.5*worldY, 0.5*worldZ);      
  G4LogicalVolume* world_logic =  new G4LogicalVolume(world_solid, Air,"World");                                    
  G4VPhysicalVolume* world_physics = new G4PVPlacement(0, G4ThreeVector(), world_logic, "World", 0, false, 0, checkOverlaps);      

    
  // define vacuum chamber
  G4double chamberX = 288.*mm, chamberY = 365*mm, chamberZ = 310*mm;
  G4double gapX = 15.*mm, gapY = 15.*mm, gapZ = 15.*mm ;
  G4double innerchamberX = chamberX-2.*gapX, innerchamberY = chamberY-2.*gapY, innerchamberZ = chamberZ-2.*gapZ;
  
  G4VSolid* chamber_solid = new G4Box("Vacuum_Chamber", 0.5*chamberX, 0.5*chamberY, 0.5*chamberZ);
  G4LogicalVolume* chamber_logic = new G4LogicalVolume(chamber_solid, Al, "Vacuum_Chamber");
    
  G4VSolid* innerVacuum_solid = new G4Box("Inner_Vacuum", 0.5*innerchamberX, 0.5*innerchamberY, 0.5*innerchamberZ);
  G4LogicalVolume* innerVacuum_logic = new G4LogicalVolume(innerVacuum_solid, vacuum, "Inner_Vacuum");

  G4double hole1_Radius = 30.*mm, hole1_Z = gapY+0.1*mm ; // hole for collimator
  G4double hole2_Radius = 75.*mm, hole2_Z = gapY+0.1*mm ; // hole for opposite side of collimator
  G4double hole3_Radius = 50.*mm, hole3_Z = gapZ+0.1*mm ; // hole for direction to Positron Counters

  G4VSolid* chamber_hole1_solid = new G4Tubs("Chamber_Hole1", 0., hole1_Radius, 0.5*hole1_Z, 0., 360.*degree);
  G4VSolid* chamber_hole2_solid = new G4Tubs("Chamber_Hole2", 0., hole2_Radius, 0.5*hole2_Z, 0., 360.*degree);
  G4VSolid* chamber_hole3_solid = new G4Tubs("Chamber_Hole3", 0., hole3_Radius, 0.5*hole3_Z, 0., 360.*degree);  

  G4LogicalVolume* chamber_hole1_logic = new G4LogicalVolume(chamber_hole1_solid, vacuum, "Chamber_Hole1");
  G4LogicalVolume* chamber_hole2_logic = new G4LogicalVolume(chamber_hole2_solid, vacuum, "Chamber_Hole2");
  G4LogicalVolume* chamber_hole3_logic = new G4LogicalVolume(chamber_hole3_solid, vacuum, "Chamber_Hole3");
  
  //  G4double hole2_RadiusMin = 60.*mm, hole2_RadiusMax = 75.*cm, hole2_Z = 96.*mm ; // Al_hole at the end
  
  G4double cap_Radius = 100.*mm, cap_Z = 21.*mm ;
  G4double cap_tube_Radius = hole2_Radius - 15.*mm, cap_tube_Z = 96.* mm;
  
  G4VSolid* chamber_cap1_solid = new G4Tubs("Chamber_Cap1", hole2_Radius, cap_Radius, 0.5*(cap_Z+0.001*mm), 0., 360.*degree);
  G4VSolid* chamber_cap2_solid = new G4Tubs("Chamber_Cap2", 0., cap_Radius, 0.5*(cap_Z+0.001*mm), 0., 360.*degree);
  G4VSolid* chamber_cap_tube_solid = new G4Tubs("Chamber_Cap_Tube", cap_tube_Radius, hole2_Radius, 0.5*cap_tube_Z, 0., 360.*degree);
  G4VSolid* chamber_inner_cap_tube_solid = new G4Tubs("Chamber_Inner_Cap_Tube", 0., hole2_Radius, 0.5*(cap_tube_Z+0.001*mm), 0., 360.*degree);

  G4LogicalVolume* chamber_cap1_logic = new G4LogicalVolume(chamber_cap1_solid, Al ,"Chamber_Cap1") ;
  G4LogicalVolume* chamber_cap2_logic = new G4LogicalVolume(chamber_cap2_solid, Al ,"Chamber_Cap2") ;
  G4LogicalVolume* chamber_cap_tube_logic = new G4LogicalVolume(chamber_cap_tube_solid, Al ,"Chamber_Cap_Tube") ;
  G4LogicalVolume* chamber_inner_cap_tube_logic = new G4LogicalVolume(chamber_inner_cap_tube_solid, vacuum ,"Chamber_Inner_Cap_Tube") ;

  G4double chamber_cap3_Radius = hole3_Radius + 15.*mm, chamber_cap3_Z = 15.*mm;
  G4VSolid* chamber_cap3_solid = new G4Tubs("Chamber_Cap3", hole3_Radius, chamber_cap3_Radius, 0.5*chamber_cap3_Z, 0.,360.*degree);
  G4LogicalVolume* chamber_cap3_logic = new G4LogicalVolume(chamber_cap3_solid, Al, "Chamber_Cap3");

  

// define collimator
  G4double colRadius = 34.*mm, colZ = 165.*mm;
  G4double innercolRadius = 30.*mm, innercolZ = 140.*mm; 
  G4double innercolRadius2 = 16.5*mm, innercolZ2 = 25.001*mm;//add 0.001 to escape the layer problem
  G4double col_LeadRadiusMin1 = 9.*mm, col_LeadRadiusMin2 = 7.*mm, col_LeadZ = 50.*mm ;

  G4VSolid* collimator_solid = new G4Tubs("Collimator", 0., colRadius, 0.5*colZ, 0., 360.*degree);
  G4LogicalVolume* collimator_logic = new G4LogicalVolume(collimator_solid, Al, "Collimator");

  G4VSolid* colVacuum1_solid = new G4Tubs("Collimator_Vacuum1", 0., innercolRadius, 0.5*innercolZ, 0., 360.*degree);
  G4LogicalVolume* colVacuum1_logic = new G4LogicalVolume(colVacuum1_solid, vacuum, "Collimator_Vacuum1");
  
  G4VSolid* colVacuum2_solid = new G4Tubs("Collimator_Vacuum2", 0., innercolRadius2, 0.5*innercolZ2, 0., 360.*degree);
  G4LogicalVolume* colVacuum2_logic = new G4LogicalVolume(colVacuum2_solid, vacuum, "Collimator_Vacuum2");

  G4VSolid* colLead1_solid=new G4Tubs("Collimator_Lead1", col_LeadRadiusMin1, innercolRadius, 0.5*col_LeadZ, 0., 360.*degree);
  G4LogicalVolume* colLead1_logic = new G4LogicalVolume(colLead1_solid, Pb, "Collmator_Lead1");
  
  G4VSolid* colLead2_solid=new G4Tubs("Collimator_Lead1", col_LeadRadiusMin2, innercolRadius, 0.5*col_LeadZ, 0., 360.*degree);
  G4LogicalVolume* colLead2_logic = new G4LogicalVolume(colLead2_solid, Pb, "Collmator_Lead2");


  //define Muon beam tube from 1 to 6...
  
  G4double tubeRadiusMin11 = colRadius, tubeRadiusMax11= 195./2.*mm, tubeZ11= 0.5*41.25*mm ;
  G4double tubeRadiusMin12 = 130./2.*mm, tubeRadiusMax12 = tubeRadiusMax11, tubeZ12=tubeZ11;
  G4double tubeRadiusMin2 = (130./2.-20.)*mm, tubeRadiusMax2= 130./2.*mm, tubeZ2= 90.*mm;
  G4double tubeRadiusMin3 = tubeRadiusMax2, tubeRadiusMax3= 233./2.*mm, tubeZ3= 37.5*mm;
  G4double tubeRadiusMin4 = tubeRadiusMax3, tubeRadiusMax4= 352./2.*mm, tubeZ4= 219.5*mm;
  G4double tubeRadiusMin5 = 100.*mm, tubeRadiusMax5= 282./2.*mm, tubeZ5= 75.*mm; // MinRadius is the size of vacuum in the beam line
  G4double tubeRadiusMin6 = tubeRadiusMin5, tubeRadiusMax6= 500.*mm, tubeZ6= 400.*mm; // MaxRadius is asuumed, Pb composed
  

  G4VSolid* beamtube11_solid = new G4Tubs("BeamTube11", 0., tubeRadiusMax11, 0.5*tubeZ11, 0., 360.*degree);
  G4VSolid* beamtubeVacuum11_solid = new G4Tubs("BeamTube11_Vacuum", 0., tubeRadiusMin11, 0.5*tubeZ11, 0., 360.*degree);
  G4LogicalVolume* beamtube11_logic = new G4LogicalVolume(beamtube11_solid, Al, "BeamTube11");
  G4LogicalVolume* beamtubeVacuum11_logic = new G4LogicalVolume(beamtubeVacuum11_solid, vacuum, "BeamTube11_Vacuum");

  G4VSolid* beamtube12_solid = new G4Tubs("BeamTube12", 0., tubeRadiusMax12, 0.5*tubeZ12, 0., 360.*degree);
  G4VSolid* beamtubeVacuum12_solid = new G4Tubs("BeamTube12_Vacuum", 0., tubeRadiusMin12, 0.5*tubeZ12, 0., 360.*degree);
  G4LogicalVolume* beamtube12_logic = new G4LogicalVolume(beamtube12_solid, Al, "BeamTube12");
  G4LogicalVolume* beamtubeVacuum12_logic = new G4LogicalVolume(beamtubeVacuum12_solid, vacuum, "BeamTube12_Vacuum");

  G4VSolid* beamtube2_solid = new G4Tubs("BeamTube2", 0., tubeRadiusMax2, 0.5*tubeZ2, 0., 360.*degree);
  G4VSolid* beamtubeVacuum2_solid = new G4Tubs("BeamTube2_Vacuum", 0., tubeRadiusMin2, 0.5*tubeZ2, 0., 360.*degree);
  G4LogicalVolume* beamtube2_logic = new G4LogicalVolume(beamtube2_solid, Al, "BeamTube2");
  G4LogicalVolume* beamtubeVacuum2_logic = new G4LogicalVolume(beamtubeVacuum2_solid, vacuum, "BeamTube2_Vacuum");

  G4VSolid* beamtube3_solid = new G4Tubs("BeamTube3", 0., tubeRadiusMax3, 0.5*tubeZ3, 0., 360.*degree);
  G4VSolid* beamtubeVacuum3_solid = new G4Tubs("BeamTube3_Vacuum", 0., tubeRadiusMin3, 0.5*tubeZ3, 0., 360.*degree);
  G4LogicalVolume* beamtube3_logic = new G4LogicalVolume(beamtube3_solid, Al, "BeamTube3");
  G4LogicalVolume* beamtubeVacuum3_logic = new G4LogicalVolume(beamtubeVacuum3_solid, vacuum, "BeamTube3_Vacuum");

  G4VSolid* beamtube4_solid = new G4Tubs("BeamTube4", 0., tubeRadiusMax4, 0.5*tubeZ4, 0., 360.*degree);
  G4VSolid* beamtubeVacuum4_solid = new G4Tubs("BeamTube4_Vacuum", 0., tubeRadiusMin4, 0.5*tubeZ4, 0., 360.*degree);
  G4LogicalVolume* beamtube4_logic = new G4LogicalVolume(beamtube4_solid, Al, "BeamTube4");
  G4LogicalVolume* beamtubeVacuum4_logic = new G4LogicalVolume(beamtubeVacuum4_solid, vacuum, "BeamTube4_Vacuum");

  G4VSolid* beamtube5_solid = new G4Tubs("BeamTube5", 0., tubeRadiusMax5, 0.5*tubeZ5, 0., 360.*degree);
  G4VSolid* beamtubeVacuum5_solid = new G4Tubs("BeamTube5_Vacuum", 0., tubeRadiusMin5, 0.5*tubeZ5, 0., 360.*degree);
  G4LogicalVolume* beamtube5_logic = new G4LogicalVolume(beamtube5_solid, Al, "BeamTube5");
  G4LogicalVolume* beamtubeVacuum5_logic = new G4LogicalVolume(beamtubeVacuum5_solid, vacuum, "BeamTube5_Vacuum");

  G4VSolid* beamtube6_solid = new G4Tubs("BeamTube6", 0., tubeRadiusMax6, 0.5*tubeZ6, 0., 360.*degree);
  G4VSolid* beamtubeVacuum6_solid = new G4Tubs("BeamTube6_Vacuum", 0., tubeRadiusMin6, 0.5*tubeZ6, 0., 360.*degree);
  G4LogicalVolume* beamtube6_logic = new G4LogicalVolume(beamtube6_solid, Pb, "BeamTube6");
  G4LogicalVolume* beamtubeVacuum6_logic = new G4LogicalVolume(beamtubeVacuum6_solid, vacuum, "BeamTube6_Vacuum");

  // define targets(CsI(Tl) crystal, foil), Al_stopper, Mirror
  G4double targetRadius, halfThickness;

  G4double StopperXY = 120.*mm ; // It's not sure
  G4double StopperZ = 4.*mm ;
  
  G4VSolid* Foil_5um_solid = new G4Tubs("Foil_5um", 0., targetRadius = 60.*mm, halfThickness = 0.5*5.*um, 0., 360.*degree);
  G4LogicalVolume* Foil_5um_logic = new G4LogicalVolume(Foil_5um_solid, targetMaterial, "Foil_5um");

  fTarget = Foil_5um_logic;

  G4VSolid* Mirror_solid = new G4Tubs("Mirror", 0., targetRadius = 60.*mm, halfThickness = 0.5*2.9*um, 0., 360.*degree);
  G4LogicalVolume* Mirror_logic = new G4LogicalVolume(Mirror_solid, Mylar, "Mirror"); // Ignore the Al(50nm)
  
  G4VSolid* AlStopper_solid = new G4Box("Al_Stopper", 0.5*StopperXY, 0.5*StopperXY, 0.5*StopperZ);
  G4LogicalVolume* AlStopper_logic = new G4LogicalVolume(AlStopper_solid, Al, "Al_Stopper");

  //define Lead shield
  G4double LeadShieldX = 50.*mm, LeadShieldY = 100.*mm, LeadShieldZ = 200.*mm ;
  G4VSolid* LeadShield_solid = new G4Box("LeadShield", 0.5*LeadShieldX, 0.5*LeadShieldY, 0.5*LeadShieldZ);
  G4LogicalVolume* LeadShield_logic = new G4LogicalVolume(LeadShield_solid, Pb, "LeadShield");
   
  //define supporting frame

  G4double AlsupportingX = 30.*mm, AlsupportingY = 10.5/6.1*chamberY, AlsupportingZ= 1600.*mm;
  G4VSolid* Alsupporting_solid = new G4Box("AlSupporting", 0.5*AlsupportingX, 0.5*AlsupportingY, 0.5*AlsupportingZ);
  G4LogicalVolume* Alsupporting_logic = new G4LogicalVolume(Alsupporting_solid, Al, "AlSupporting");
  

  
  // Arrange each part in physical world

  // Order of arrangement is { Chamber -> Collimator -> MuonBeamTube -> Target -> Mirror, Al_Stoppers -> Lead shield }

  // center of Mirror is the center of the world_logic : (0,0,0)


  // set the chamber
  G4double MirrorToTarget = -55.*mm ; // y-direction
  G4double MirrorToChamberCenterZ = -35.*mm ; // z-direction, 120(dist from mirror to side of chamber) - 0.5*chamberZ
  G4double MirrorToChamberCenterX = 25.*mm ; // x-direction
  G4double MirrorToChamberCenterY = -13.*mm ; // y-direction
  
  G4double DistToHole1X = -1.*MirrorToChamberCenterX ; // x-direction
  G4double DistToHole1Z = -1.*MirrorToChamberCenterZ ; // z-direction
  G4double DistToHole1Y = -0.5*chamberY + 0.5*hole1_Z ; // y-direction

  G4double DistToHole2X = -1.*MirrorToChamberCenterX ;
  G4double DistToHole2Y = 0.5*chamberY - 0.5*hole2_Z ;
  G4double DistToHole2Z = -23.*mm ; // derive from the picture only
  
  G4double DistToHole3Y = -1.*MirrorToChamberCenterY ; // y-direction
  G4double DistToHole3Z = 0.5*chamberZ -0.5*hole3_Z ; // z-direction
  G4double DistToHole3X = -1.*MirrorToChamberCenterX ; // x-direction, height of hole3 is assumed to be same with Mirror

  G4double MirrorToCapTubeCenterZ = MirrorToChamberCenterZ + DistToHole2Z ;
  G4double MirrorToCapTubeCenterY = MirrorToChamberCenterY + 0.5*chamberY + 0.5*cap_tube_Z ;
  G4double MirrorToCapTubeCenterX = MirrorToChamberCenterX + DistToHole2X ; // = 0

  G4double MirrorToCap3CenterX = MirrorToChamberCenterX + DistToHole3X ;
  G4double MirrorToCap3CenterY = MirrorToChamberCenterY + DistToHole3Y ;
  G4double MirrorToCap3CenterZ = MirrorToChamberCenterZ + DistToHole3Z + 0.5*hole3_Z + 0.5*chamber_cap3_Z ;

  G4RotationMatrix* xRot = new G4RotationMatrix();
  xRot->rotateX(90.*degree); // rotation matrix of hole1, hole2, collimator, beamtube

  new G4PVPlacement(0, G4ThreeVector(MirrorToChamberCenterX,MirrorToChamberCenterY,MirrorToChamberCenterZ), chamber_logic, "Vacuum_Chamber", world_logic, false, 0, checkOverlaps);

  new G4PVPlacement(0, G4ThreeVector(0,0,0), innerVacuum_logic, "Inner_Vacuum", chamber_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(DistToHole1X,DistToHole1Y,DistToHole1Z), chamber_hole1_logic, "Chamber_Hole1", chamber_logic, false, 0, checkOverlaps);
  
  new G4PVPlacement(xRot, G4ThreeVector(DistToHole2X,DistToHole2Y,DistToHole2Z), chamber_hole2_logic, "Chamber_Hole2", chamber_logic, false, 0, checkOverlaps);

  new G4PVPlacement(0, G4ThreeVector(DistToHole3X,DistToHole3Y,DistToHole3Z), chamber_hole3_logic, "Chamber_Hole3", chamber_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(MirrorToCapTubeCenterX,MirrorToCapTubeCenterY,MirrorToCapTubeCenterZ), chamber_inner_cap_tube_logic, "Chamber_Inner_Cap_Tube", world_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(MirrorToCapTubeCenterX,MirrorToCapTubeCenterY,MirrorToCapTubeCenterZ), chamber_cap_tube_logic, "Chamber_Cap_Tube", world_logic, false, 0, checkOverlaps);
  
  new G4PVPlacement(xRot, G4ThreeVector(MirrorToCapTubeCenterX,MirrorToCapTubeCenterY + 0.5*cap_tube_Z - 0.5* cap_Z ,MirrorToCapTubeCenterZ), chamber_cap1_logic, "Chamber_Cap1", world_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(MirrorToCapTubeCenterX,MirrorToCapTubeCenterY + 0.5*cap_tube_Z + 0.5* cap_Z ,MirrorToCapTubeCenterZ), chamber_cap2_logic, "Chamber_Cap2", world_logic, false, 0, checkOverlaps);

  new G4PVPlacement(0, G4ThreeVector(MirrorToCap3CenterX,MirrorToCap3CenterY,MirrorToCap3CenterZ), chamber_cap3_logic, "Chamber_Cap3", world_logic, false, 0, checkOverlaps);
  

  
  // set the Collimator, BeamTube
  G4double frameThickness = 8.*mm ; // thickness of frame of Foil, Mirror
  G4double MirrorToCollimator = MirrorToTarget - 0.5*frameThickness - 0.5*colZ ; // y-direction
  G4double MirrorToBeamTube11 = MirrorToChamberCenterY - 0.5*chamberY - 0.5*tubeZ11 ; // y-direction
  G4double MirrorToBeamTube12 = MirrorToBeamTube11 - 0.5*tubeZ11 - 0.5*tubeZ12 ;
  G4double MirrorToBeamTube2 = MirrorToBeamTube12 - 0.5*tubeZ12 - 0.5*tubeZ2 ;
  G4double MirrorToBeamTube3 = MirrorToBeamTube2 - 0.5*tubeZ2 - 0.5*tubeZ3 ;
  G4double MirrorToBeamTube4 = MirrorToBeamTube3 - 0.5*tubeZ3 - 0.5*tubeZ4 ;
  G4double MirrorToBeamTube5 = MirrorToBeamTube4 - 0.5*tubeZ4 - 0.5*tubeZ5 ;
  G4double MirrorToBeamTube6 = MirrorToBeamTube5 - 0.5*tubeZ5 - 0.5*tubeZ6 ;

  G4double CollCenterToVacuum1 = 0.5*innercolZ - 0.5*colZ ; // y-direction
  G4double CollCenterToVacuum2 = 0.5*colZ - 0.5*innercolZ2 ; // y-direction
  
  G4double CollToLead1 = 0.5*innercolZ - 0.5*col_LeadZ ; // y-direction
  G4double CollToLead2 = 0.5*innercolZ - 1.5*col_LeadZ ; // y-direction

  //  G4double CollToLead1 = 0.5*colZ - (innercolZ2 + 0.5*col_LeadZ) ; // y-direction
  //  G4double CollToLead2 = 0.5*colZ - (innercolZ2 + 1.5*col_LeadZ) ; // y-direction

  new G4PVPlacement(xRot, G4ThreeVector(0.,MirrorToCollimator,0.), collimator_logic, "Collimator", world_logic, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0,CollCenterToVacuum1), colVacuum1_logic, "Collimator_Vacuum1", collimator_logic, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0,CollCenterToVacuum2), colVacuum2_logic, "Collimator_Vacuum2", collimator_logic, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0,CollToLead1), colLead1_logic, "Collimator_Lead1", colVacuum1_logic, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0,CollToLead2), colLead2_logic, "Collimator_Lead2", colVacuum1_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube11,0), beamtube11_logic, "BeamTube11", world_logic, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum11_logic, "BeamTube11_Vacuum", beamtube11_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube12,0), beamtube12_logic, "BeamTube12", world_logic, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum12_logic, "BeamTube12_Vacuum", beamtube12_logic, false, 0, checkOverlaps);
  
  new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube2,0), beamtube2_logic, "BeamTube2", world_logic, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum2_logic, "BeamTube2_Vacuum", beamtube2_logic, false, 0, checkOverlaps);
  
  new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube3,0), beamtube3_logic, "BeamTube3", world_logic, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum3_logic, "BeamTube3_Vacuum", beamtube3_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube4,0), beamtube4_logic, "BeamTube4", world_logic, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum4_logic, "BeamTube4_Vacuum", beamtube4_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube5,0), beamtube5_logic, "BeamTube5", world_logic, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum5_logic, "BeamTube5_Vacuum", beamtube5_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube6,0), beamtube6_logic, "BeamTube6", world_logic, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum6_logic, "BeamTube6_Vacuum", beamtube6_logic, false, 0, checkOverlaps);


  //set the Target, Mirror, Al_Stopper

  G4RotationMatrix* xRot2 = new G4RotationMatrix();
  xRot2->rotateX(45.*degree); // rotation matrix of Mirror, AlStopper

  new G4PVPlacement(xRot, G4ThreeVector(-MirrorToChamberCenterX,-MirrorToChamberCenterY+MirrorToTarget,-MirrorToChamberCenterZ), fTarget, "Foil_5um", innerVacuum_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot2, G4ThreeVector(-MirrorToChamberCenterX,-MirrorToChamberCenterY,-MirrorToChamberCenterZ), Mirror_logic, "Mirror", innerVacuum_logic, false, 0, checkOverlaps);
  
  new G4PVPlacement(xRot2, G4ThreeVector(-MirrorToChamberCenterX,-MirrorToChamberCenterY+10.*1.414,-MirrorToChamberCenterZ), AlStopper_logic, "Al_Stopper", innerVacuum_logic, false, 0, checkOverlaps);

  // set the Alsupporting frame

  new G4PVPlacement(0, G4ThreeVector(-(119.+62.+30./2.)*mm, 0.75/10.5*AlsupportingY, 0), Alsupporting_logic, "AlSupporting", world_logic, false, 0, checkOverlaps);
  

  // set the LeadShield[i] i=0 to 5 (chamberX(height)/LeadShieldX = 288/50 ~ 5.xx) 

  G4double MirrorToLeadX[6] ; // only height(X) of each LeadShield are different
  G4double MirrorToLeadY = MirrorToChamberCenterY - chamber_cap3_Radius - 0.5*LeadShieldY ;
  G4double MirrorToLeadZ = MirrorToChamberCenterZ + 0.5*chamberZ + 0.5*LeadShieldZ ;
  
  for(G4int i=0 ; i!=6 ; i++)
    {
      MirrorToLeadX[i] = MirrorToChamberCenterX - 0.5*chamberX + (-0.5+i)*LeadShieldX ;
           
      new G4PVPlacement(0, G4ThreeVector(MirrorToLeadX[i],MirrorToLeadY,MirrorToLeadZ), LeadShield_logic, "LeadShield", world_logic, false, 0, checkOverlaps);

    }

  
  //  new G4PVPlacement(0, G4ThreeVector(,,), chamber_hole1_logic, "Chamber_Hole1", chamber_logic, false, 0, checkOverlaps);

 
  
  //define Positron Counters and Arrange each one

  G4VSolid* PC_solid1 = new G4Box("PCsolid1",0.5*35.*mm,0.5*30.*mm,0.5*5.*mm);
  G4VSolid* PC_solid2 = new G4Box("PCsolid2",0.5*65.*mm,0.5*60.*mm,0.5*10.*mm);
  G4VSolid* PC_solid3 = new G4Box("PCsolid3",0.5*55.*mm,0.5*60.*mm,0.5*5.*mm);

  G4double DistToPC1 = (667.+135.)*mm ;
  G4double DistToPC2 = DistToPC1 + 40.*mm ;
  G4double DistToPC3 = DistToPC2 + 43.*mm ;

  //  DistToAxisY = MirrorToChamberCenterY - 40.*mm ; // vertical dist to Axis of PCs from center of Mylar in y-direction
  //  DistToAxisX = 0.*mm ;
  //in x-direction, axis is assumed to be at same height with the center of Mylar(Mirror)  
  fPC_logic[0] = new G4LogicalVolume(PC_solid1,Mylar,"PositronCounter");
  new G4PVPlacement(0,G4ThreeVector(DistToAxisX,DistToAxisY,DistToPC1),fPC_logic[0],"PositronCounter1", world_logic,false,0,checkOverlaps);
 
  fPC_logic[1]= new G4LogicalVolume(PC_solid2,Mylar,"PositronCounter");
  new G4PVPlacement(0,G4ThreeVector(DistToAxisX,DistToAxisY,DistToPC2),fPC_logic[1],"PositronCounter2", world_logic,false,0,checkOverlaps);

  fPC_logic[2]= new G4LogicalVolume(PC_solid3,Mylar,"PositronCounter");
  new G4PVPlacement(0,G4ThreeVector(DistToAxisX,DistToAxisY,DistToPC3),fPC_logic[2],"PositronCounter3", world_logic,false,0,checkOverlaps);
  

  //setting the Visualization attributes, world to invisible
  world_logic->SetVisAttributes (G4VisAttributes::GetInvisible());

  /*
  G4VisAttributes* ChamberVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.3));
  chamber_logic->SetVisAttributes(ChamberVisAtt);
  */
  
  //
  //always return the physical World
  //
  return world_physics;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void PCDetectorConstruction::ConstructSDandField()
{
  // Set PositronCounters as Sensitive Detectors
  PCTrackerSD* PC = new PCTrackerSD("PC");
  // PCTrackerSD* PC2 = new PCTrackerSD("/PC2");
  //PCTrackerSD* PC3 = new PCTrackerSD("/PC3");

  G4SDManager* sdman = G4SDManager::GetSDMpointer();
  sdman->AddNewDetector(PC);
  SetSensitiveDetector("PositronCounter",PC, true);

   /*
   sdman->AddNewDetector(PC2);
    PC_logic2->SetSensitiveDetector(PC2);
  sdman->AddNewDetector(PC3);
    PC_logic3->SetSensitiveDetector(PC3);
   */

  G4SDParticleWithEnergyFilter* positronFilter = new G4SDParticleWithEnergyFilter("positronFilter");
  positronFilter->add("e+");
  positronFilter->SetKineticEnergy(1.0,60.0*MeV);
  PC->SetFilter(positronFilter);

  //  PC2->SetFilter(positronFilter);
  //  PC3->SetFilter(positronFilter);


  // Set Target as a Sensitive Detector to count the muons which hit target
  MCTrackerSD* MC = new MCTrackerSD("MC");
  sdman->AddNewDetector(MC);
  SetSensitiveDetector("Foil_5um",MC,true);

  G4SDParticleWithEnergyFilter* muonFilter = new G4SDParticleWithEnergyFilter("muonFilter");
  muonFilter->add("mu+");
  muonFilter->SetKineticEnergy(0.01,5.*MeV);
  MC->SetFilter(muonFilter);
  

}


void PCDetectorConstruction::SetDistToAxisX(G4double dist)
{
  DistToAxisX = dist ;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PCDetectorConstruction::SetDistToAxisY(G4double dist)
{
  DistToAxisY = dist;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

