#include "shell.hh"

Shell :: Shell()
{
  //In the new scheme, shells start out dead

  active=false;

}
Shell :: ~Shell(){}


double Shell :: GetVelocity()
{
  double velocity;
  velocity = c * sqrt(1.0-(1.0/(gamma*gamma)));
  return velocity;
}

double Shell :: GetEnergy()
{
  double energy;
  energy = gamma * mass * c * c;
  return energy;
}

void Shell ::MoveShell(double deltaTime)
{
  radius += GetVelocity()*deltaTime;
}

CollisionHist* Shell :: Collide(Shell *other)
{
  double gammaR, gammaF, Eint, totalEnergy, Ek, tmpM1, tmpM2, kappa, a;
  CollisionHist *collision = new CollisionHist;

  if(other->GetRadius() > radius)
    {
      std::cout<< "Shells are backwards in Shell :: Colliside\n";
      return collision;
    }

  //From Daigne 1998
  gammaR = sqrt(gamma * other->GetGamma());
  //gammaF = sqrt(gamma * other->GetGamma() * ( (mass * gamma + other->GetMass()*other->GetGamma())/(mass*other->GetGamma() + other->GetMass() * gamma)   )  );

  //kappa = other->GetGamma()/gamma;
  



  //From Damien
  a=(mass*gamma+other->GetMass()*other->GetGamma())/(mass*sqrt(gamma*gamma-1.)+other->GetMass()*sqrt(other->GetGamma()*other->GetGamma()-1.));

  gammaF = sqrt(a*a/(a*a-1.));					// Lorentz factor of the merged shell


  

  // *********** Energy Calculations

  
  
  //From Daigne 1998
  //  Eint = pow(sqrt(kappa)-1,2)*c*c/(2 * sqrt(kappa)); // The internal radiated energy 

  //From Damien


  Eint = mass*(gamma/gammaF-1.)+other->GetMass()*(other->GetGamma()/gammaF-1.);  // The internal radiated energy



  mass += other->GetMass();       //Change the mass of this shell
  gamma = gammaF;                 //Change Gamma of this shell
  gammaR = gammaF;    //This should be checked


  

  //Update the collision history
  collision->radiatedEnergy = Eint;
  collision->gammaR=gammaR;
  collision->radius = radius;
  
  return collision;




}

void Shell :: Deactivate(double time)
{
  /*
    This function marks the shell inactive and records its death in a shellHist.
    The shell will be skipped and 

   */
  active = false;
  deathTime = time;

  return;

}


void Shell :: Activate(double time)
{
  /*
    This function activates a shell when it is emitted and records 
    its initial time

   */
  active = true;
  initialTime = time;

  return;

}




void Shell :: PrintShellInfo()
{
  std::cout<<"\n********************\n";
  std::cout<<"** SHELL READOUT ***\n";
  std::cout<<"RADIUS: "<<radius<<std::endl;
  std::cout<<"GAMMA:  "<<gamma<<std::endl;
  std::cout<<"MASS:   "<<mass<<std::endl;
  std::cout<<"********************\n";
  
  return;


}
