#include "photons.hh"

Photons :: Photons(int new_IMAX, int new_JMAX) : Spectrum(new_IMAX, new_JMAX, false)
{
  for(int i=0; i<IMAX; i++)
    {
      spectra[i]=new double[JMAX];
    }

  double nu_min, nu_max;
  
  //The eGrid is in normalized units e=hv/m_e c**2 but we need to do it in nu space
  double eMin, eMax;
  double conversion;
  double me= 9.1093897E-28;
  double h= 6.6260755E-27;
  double c=2.99792458E10;
  eMin = 1.E-6; eMax= 1.E5;
  conversion = me*c*c/h;
  

  nu_min = eMin*conversion; nu_max = eMax*conversion;
  



  for(int j=0;j<JMAX;j++)
    {
      eGrid[j]=nu_min*pow(nu_max/nu_min,(double)j/((double)JMAX-1));
    }


}

Photons :: ~Photons()
{

}

 

void Photons :: FillSpectrum(int i, int j, double flux)
{
  spectra[i][j]=flux;
  return;
}
