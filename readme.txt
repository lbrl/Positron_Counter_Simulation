-------------------------------------------------------------------

     =========================================================
     Geant4 - an Object-Oriented Toolkit for Simulation in HEP
     =========================================================

                            PositronCounter
                            ---------------

 Positron Counter Simulation demonstrates a very simple coincidence test
which uses each positron counter as scorers. They just count the number of
positrons which hit them.
 The main method to acquire coincidence number is using Global time of hits
in each positron counter. 
 
	
 1- GEOMETRY DEFINITION
	
   The geometry is constructed in the PCDetectorConstruction class. It defines
   needed material first, and then define each parts of detector. Only needed
   type of shape is G4Tubs and G4Box. 

   The center or Mirror is set to the origin of world, i.e, (0,0,0). All other main
   parts' position is determined relative to the position of Mirror(world's center).
   The chamber's center is written as 'MirrorToChamberCenterX, MirrorToChamberCenterY, 
   MirrorToChamberCenterZ'.
   
   The minor parts, such as vaccum in chamber, leads in collimator are defined relative
   to their main part. (Position(center) of Vacuum part in chamber is (0,0,0) relative
   to chamber's center)

   Each solid, logcial volume are defined first, and then place it at the same time. 

   Vacuum Chamber is made by G4Box, Aluminum material, and filled by vacuum. The
   3 holes are also made by G4Tubs, vacuum material. Aluminum caps are also defined.

   Collimators with lead tubs are defined. It locates at the hole1.

   A small muon beam tube is also defined. It locates at the end of collimator. 
   I set the length of it to 50.mm

   Target(CsI(Tl) foil_5um), Mirror, Al_Stopper are defined. I will upgrade the
   other targets with DetectorMessenger class as soon as possible.

   Finally, LeadShields are defined, but it's not exact position.

   		
 2- PHYSICS LIST
 
   I wrote needed physiclist for this simulation in "PCPhysicsList", but it has
   some problem about memory. Therefore I just use "QBBC.h" as exampleB1. That's
   enough for our simulation. 


 3- TrackerHits, TrackerSD

   PCTrackerHits, PCTrackerSD Classes are newly defined. The purpose of these classes
   is to get the information of TrackID, (Logical)Volume which positron hits, global
   time when a positron hit a positron counter. The global time of positrons at different
   hit is compared to make coincidence test.
   More specifically, if a positron hits the second and third positron counters, global time
   information at each hit is stored in hitscollection. At the end of event, if its order is
   like Global_Time2 < Global_Time3, it increases count of coincidence for 2&3 by one. Same
   logic applies to coincidence of all PC.


 4- ACTION INITALIZATION

   A newly introduced class, B1ActionInitialization, instantiates and registers 
   to Geant4 kernel all user action classes.

   While in sequential mode the action classes are instatiated just once,
   via invoking the method:
      PCActionInitialization::Build() 
   in multi-threading mode the same method is invoked for each thread worker
   and so all user action classes are defined thread-local.

   A run action class is instantiated both thread-local 
   and global that's why its instance is created also in the method
      PCActionInitialization::BuildForMaster() 
   which is invoked only in multi-threading mode.
  	 
 5- PRIMARY GENERATOR
  
   The primary generator is defined in the PCPrimaryGeneratorAction class.
   The default kinematics is a 3.5 MeV +muon(mu+), which is correspond to
   27.4MeV momentum.

   Its generated direction and position are set to same as past MCP-BPM's simulation
   with proper modification in direction.

 6- How to run

   After download whold file in ~/.../$WHERE_YOU_DOWNLOAD, I recommend to make a build
   directory in that directory. (such as 'mkdir ~/.../$WHERE_YOU_DOWNLOAD/build')
   Then, change directory to build, and command 'cmake ..'. After it, commands 'make -jN'
   where N is the number of core you can use. Then there will be an executable,
   'PositronCounter'. 
   
     
 7- Output in console

   Now, you can simulate the positron counter.
   If you want to see the simulation graphically, just type './PositronCounter'.
   Visulation method is OpenGL. You can shoot a muon by typing run/beamOn 1, or just click
   a button at the top menu bar.
   If you want to simulate a number of particle, typing './PositronCounter run1.mac' in the       
   console.
   Default setting is to generate 200000 mu+. You can change it at the line in run1.mac
   by '/run/beamOn number' with nubmer=what you want.

   Output information is 5 number. PC1 totalCount, PC2 totalCount, PC3 totalCount,
   PC2&3 Coincidence count, PCAll Coincidence count. They are not saved in a file,
   just output in console.



	
