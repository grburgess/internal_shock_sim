#include "electrons.hh"


Electrons :: Electrons(int new_IMAX, int new_JMAX, double new_gamma_min, double new_gamma_max, bool new_constantInjection) : Spectrum(new_IMAX,new_JMAX, false)
{

  JMAX = new_JMAX;
  constantInjection = new_constantInjection;

  gamma_min = new_gamma_min;
  gamma_max = new_gamma_max;

 

  double *tmpGamma = new double[JMAX+2]; //To create half grid

  gammaHalf = new double[JMAX+2];



  //  Determine the number of time steps
  //  for now this is fixed for each sim. 
  //  soon I want to implement a variable time step.
  
   // Initialize the gamma arrays
  
 
  for(int j=-1;j<=JMAX;j++)
    {
      tmpGamma[j+1]=gamma_min*pow(gamma_max/gamma_min,(double)j/((double)JMAX-1));
    }
  for(int j=-1;j<JMAX;j++)
    {
      gammaHalf[j+1]=  pow( 10. ,(log10(tmpGamma[j+1])+log10(tmpGamma[j+2]))/2.);
    }
  
  
  //Use the tmpGamma to create the real gamma span 
  for(int j=0;j<JMAX;j++)
    {
      eGrid[j]=tmpGamma[j+1];
      
    }
  //get rid of tmpGamma
  delete tmpGamma;


}

Electrons :: ~Electrons()
{
   delete gammaHalf;
}

void Electrons :: Fokker()
{
 /*
    This  routine numerically integrates the Fokker-Plack
    equation using the fully-implicit method of 
    Chang and Cooper 1970. Note that this implementaion
    does not include a diffusive term.

   */

  //Tridiagonal matrix coeffs
  double *V3 = new double[JMAX]; 
  double *V2 = new double[JMAX];
  double *source = new double[JMAX]; // Allocate the right-hand side of the FP equation 
  double deltaGamma;

  //Initialize the first electron distribution
  spectra[0] = new double[JMAX];
  for(int j=0; j<JMAX; j++)
    {
      spectra[0][j]=ElectronSource(eGrid[j]);
    }
 
  
   
  for(int i=0; i< IMAX-1; i++)// 
    {
      
      // This is the main loop for solving the equation
      // This is where the past radiation field will be used to calc the IC cooling!
      
      spectra[i+1] = new double[JMAX]; //Allocate the next electron time step
     
   
      for(int j=1;j<JMAX;j++)
	{
	  //std::cout<< i<<" here"<<j <<"\n";
	  //Tridiagonal solver. No diffusion => V1 = a = 0. V2 = b. V3 = c
	  // Therefore no renormalization as desribed in Press et al.

	  //Set the gamma step-size
	  deltaGamma = gammaHalf[j]-gammaHalf[j-1]; //Half steps are at j+.5 and j-.5

	  //Set the coeffs. At some point this should be virtualized
	  V3[j] = -(deltaT*Cooling(gammaHalf[j]) )/ deltaGamma;
	  V2[j] = 1 + deltaT/tEsc + (deltaT*Cooling(gammaHalf[j-1]) )/ deltaGamma;
	  
	  if(constantInjection) //If so we need the Q matrix
	    {
	      source[j] = ElectronSource(eGrid[j])+spectra[i][j]; //Set the RHS 
	    }
	  else
	    {
	      source[j]=spectra[i][j];
	    }

	}
      
      spectra[i+1][JMAX-1]=source[JMAX-1]/V2[JMAX-1];
      
      for(int j = JMAX-2; j>=0; j--)
	{
	  spectra[i+1][j] = (source[j] - V3[j]*spectra[i+1][j+1])/V2[j];
	}

      
    }

  return;


}





double Electrons :: Cooling(double gam)
{
  std::cout<< "Base class Cooling\n";
  return 0.;
}


double Electrons :: ElectronSource(double gam)
{
  std::cout<< "Base class Source\n";
  return 0.;  
}



int* Electrons :: Find_Min_Max(int i)
{
  int *minMax= new int[2];
  bool flag = true;
  int j=0.;
  double minFlux =1E-15;
      std::cout<<"fuck you\n\n";  
  while(flag)
    {
      //if(spectra[i][j]>spectra[i][j+1])
      if(spectra[i][j]>minFlux)
	{
	  minMax[0]=j;
	  flag=false;
	}
      else
	{
	  j++;
	}



    }

  flag=true;
  j=JMAX;

   while(flag)
    {
      if(spectra[i][j]>minFlux)
	{
	  minMax[1]=j;
	  flag=false;
	}
      else
	{
	  j--;
	}

    }

  return minMax;
}
