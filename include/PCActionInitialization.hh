
#ifndef PCActionInitialization_h
#define PCActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class PCActionInitialization : public G4VUserActionInitialization
{
  public:
    PCActionInitialization();
    virtual ~PCActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};


#endif

    
