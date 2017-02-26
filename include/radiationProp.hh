#ifndef _RADIATIONPROP_HH_
#define _RADIATIONPROP_HH_

#include "shellState.hh"
#include "collisionHist.hh"
#include "jet.hh"
#include "shell.hh"
#include "radiator.hh"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream> 

class RadiationProp
{
public:
  RadiationProp();
  ~RadiationProp();

  void ReadJet(Jet *jet);
  void WriteTrajectoryHistory();
  void WriteCollisionHistory();
  void RadiateJet();




private:

  int NUM_HIST;


  std::vector<CollisionHist*> collHist;
  std::vector<SSRow> jetState;
  Shell **shell;
  
  
  void CalculateOpacity();
  double LightTravelTime(double emissionRadius, double shellRadius, double  shellVelocity, double emissionTime);


};

#endif
