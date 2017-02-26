#ifndef _SYNCHROTRONELECTRONS_HH_
#define _SYNCHROTRONELECTRONS_HH_

#include "electrons.hh"

class SynchrotronElectrons : public Electrons
{
public:
  SynchrotronElectrons(int new_IMAX, int new_JMAX);
  ~SynchrotronElectrons();

  void SetB(double new_B){B=new_B; return;};
  void SetEGammaMax(double new_EGammaMax){eGamma_max = new_EGammaMax; return;};
  void SetEGammaMin(double new_EGammaMin){eGamma_min = new_EGammaMin; return;};
  void SetIndex(double new_p){p=new_p; return;};
  void SetNacc(double new_ne_acc){ne_acc = new_ne_acc; return;};
  

private:

  double B, eGamma_max, eGamma_min, p, ne_acc;
  

  const static double pi=   3.1415926535;
  const static double sigT= 0.6652458E-24;
  const static double me=   9.1093897E-28;
  const static double mp=   1.6726231E-24;
  const static double e=    4.8032068E-10;
  const static double h=    6.6260755E-27;
  const static double c=    2.99792458E10;


  double Cooling(double gamma); //Defined here as the Synchrotron Cooling
  double ElectronSource(double gamma); // PL injection source



};


#endif
