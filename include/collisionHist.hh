#ifndef _COLLISIONHIST_HH_
#define _COLLISIONHIST_HH_

struct CollisionHist
{
  double time;  // When the collision took place
  double radiatedEnergy; // The amount of energy radiated 
  double radius;  //Where the collision took place
  double gammaR;
  int front; //Front shell's index
  int back;  //Back shell's index
  //double opacity; //The amount of material ahead of the photon shell
};


#endif
