#ifndef _PHOTONS_HH_
#define _PHOTONS_HH_

#include "spectrum.hh"
#include "electrons.hh"




class Photons : public Spectrum
{
public:

  Photons(int new_IMAX, int new_JMAX);
  ~Photons();

  void FillSpectrum(int i, int j, double flux);
  


  


};

#endif
