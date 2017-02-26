#ifndef _SPECTRUM_HH_
#define _SPECTRUM_HH_

#include <iostream>
#include <fstream>
#include <vector>
//#include <sstream>
#include <string>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

/*

 Spectrum is a class to create time resolved spectra. The number of time
 steps (IMAX) can be static or variable as specified in the constructor.
 The number of energy steps (JMAX) is specfied in the constructor.

 The [] operator allows access to the spectra e.g. time 11 energy 90
 would be accessed as Spectrum sp[10][90]. 

 The destructor takes care of the clean up of the arrays.


 J.M Burgess 07/06/2012

 */


class Spectrum
{
public:
  Spectrum(int new_IMAX, int new_JMAX, bool new_variable);
  ~Spectrum();

  double operator() (int i, int j);
  double* GetSpectrum(int i){return spectra[i];};
  double GetSpectralValue(int i, int j){return spectra[i][j];}
  double GetEnergyValue(int j){return eGrid[j];};
  int GetIMAX();
  int GetJMAX();

  // void WriteSpectralEvolution();
  void WriteSpectrum(int i, std::string fileName);
  void WriteInterpolatedSpectrum(double gMin, double gMax, std::string fileName);

  void InterpolateTimeSlice(int i, int jMin, int jMax); //Interpolates a time slice
  double InterpolatedSpectrum(double eValue); //Returns the flux of the interp'ed spectrum
  void FreeGSL(); // Must be called b4 making another time slice

protected:
  int IMAX, JMAX;
  bool variable;
  double **spectra;
  double *eGrid;
  gsl_interp_accel *acc;
  gsl_spline *spline;

 
  
  

};


#endif 
