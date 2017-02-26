#include "synchrotronElectrons.hh"


SynchrotronElectrons :: SynchrotronElectrons(int new_IMAX, int new_JMAX) : Electrons(new_IMAX, new_JMAX, 1., 1.E8, false)
{
}

SynchrotronElectrons :: ~SynchrotronElectrons()
{
}


double SynchrotronElectrons :: Cooling(double gam)
{
  double val;
  
  val = sigT / (6. * pi * me * c) ;
  val *= B*B*gam*gam;
  return val;

}


double SynchrotronElectrons :: ElectronSource(double gam)
{
  //Power-law electron source
  double val;

  if(gam >= eGamma_min && gam<=eGamma_max)
    {
      val = (p-1)*(ne_acc/eGamma_min)*pow(gam/eGamma_min,-p);
    }
  else
    {
      val = 0.0;
    }
  return val;

}
