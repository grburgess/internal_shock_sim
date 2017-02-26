#ifndef _RADIATOR_HH_
#define _RADIATOR_HH_

#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include "collisionHist.hh"
#include "electrons.hh"
#include "synchrotronElectrons.hh"
#include "photons.hh"
#include <gsl/gsl_integration.h>
#include <gsl/gsl_sf_synchrotron.h>
#include <gsl/gsl_sf_result.h>



//typedef double Spectrum[NUM_EBINS]


class Radiator
{
public:

  Radiator();
  Radiator(double new_p, double new_eFrac, double new_bFrac,double new_accelerationFrac);
  ~Radiator();

  void SetEdot(double new_Edot){Edot = new_Edot; return;};

  void ReadCollision(CollisionHist* colHist);
  void SetGammaR(double new_gammaR){gammaR = new_gammaR; return;};
  void SetRadius(double new_radius){radius = new_radius; return;};
  void SetRadiatedEnergy(double new_radiatedEnergy){radiatedEnergy = new_radiatedEnergy; return;};

  double GetB(){return B;};
  double GetEGammaMin(){return Egamma_min;};
  double GetNu(){return nu;};
  //double GetTesc(){return tEsc};
  
  void SetupProperties();
 


private:

  const static  double pi=3.1415926535;
  const static double sigT = 0.6652458E-24; // CHECK THIS VALUE OUT!!!
  const static double me= 9.1093897E-28;
  const static double mp= 1.6726231E-24;
  const static double e= 4.8032068E-10;
  const static double h= 6.6260755E-27;
  const static double c=2.99792458E10;

  SynchrotronElectrons *electrons;
  Photons *photons;
  
  int IMAX; // Time itereator. Set in Initialize()
  int JMAX, KMAX;
  // Numerical quantities
  double B, Egamma_max, Egamma_min, gamma_max, gamma_min; 
  double deltaT, tEsc, p, Edot, eFrac, accelerationFrac, bFrac, ne_acc;
  
 
  //Quantities that can be set independently for testing purposes
  double radiatedEnergy, radius, gammaR;


   // //Synchrotron Emission
  double P_syn(double new_nu, double gamma);
  double SynchrotronTimeScale(double gamma);

  //Photon Generation
  double nu;
  void GeneratePhotonSpectrum(int i); //Generate the photon spectrum of time-slice i
  static double EmissionIntegrand(double gamma, void *p);
  
  
  void Initialize();
  //  void ReadCollision(CollisionHist *colHist);
  

};



#endif
