#ifndef _ELECTRONS_HH_
#define _ELECTRONS_HH_

#include "spectrum.hh"

#include <math.h>

class Electrons : public Spectrum
{
public:
  
  Electrons(int new_IMAX, int new_JMAX, double new_gamma_min, double new_gamma_max, bool new_constantInjection);
  ~Electrons();
 
  void SetTesc(double new_tEsc){tEsc = new_tEsc; return;};
  void SetDeltaT(double new_deltaT){deltaT = new_deltaT; return;};
  int* Find_Min_Max(int i); //Find the min and max j of the electrons that fall below a limit


   void Fokker();

protected:
  double gamma_min, gamma_max, deltaT, tEsc;
  

  virtual double ElectronSource(double gam);
  virtual double Cooling(double gam);
  
  
  
private:
  

 
  double *gammaHalf;
  bool constantInjection;


 
  
 



};


#endif
