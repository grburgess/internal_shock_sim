#ifndef _SHELL_HH_
#define _SHELL_HH_


#include <vector>
#include <algorithm> //For min
#include <math.h>
#include <iostream>
#include "collisionHist.hh"

const double c = 2.99E10;


class Shell
{
public:
  Shell();
  ~Shell();


  void SetRadius(double inRadius){radius = inRadius; return;};
  void SetMass(double inMass){mass = inMass; return;};
  void SetGamma(double inGamma){gamma = inGamma; return;};
  void SetInitialTime(double inTime){initialTime = inTime; return;};
  //void SetEnergy(double inEnergy){energy = inEnergy; return;};

  double GetVelocity();
  double GetEnergy();
  double GetRadius(){return radius;};
  double GetMass(){return mass;};
  double GetGamma(){return gamma;};
  double GetDeathTime(){return deathTime;};

  void PrintShellInfo();
  

  bool Status(){return active;};
  


  void MoveShell(double deltaTime);

  
  void Deactivate(double time);
  void Activate(double time);


  CollisionHist *Collide(Shell *other);


private:

  bool active;
  double radius, mass, gamma, energy, initialTime, deathTime;
  //std::vector<ShellHist*> history;
  
  

  





};


#endif
