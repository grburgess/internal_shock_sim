#include "spectrum.hh"

Spectrum :: Spectrum(int new_IMAX,int  new_JMAX, bool new_variable=false )
{
  IMAX=new_IMAX;
  JMAX=new_JMAX;
  variable=new_variable;


  if(variable)
    {
      //    spectra = std::vector<double*>;
    }
  else
    {
      spectra = new double*[IMAX];
    }

  eGrid = new double[JMAX];

  
  

}


Spectrum :: ~Spectrum()
{
  if(!variable)
    {
      for(int i=0; i< IMAX;i++)
	{
	  delete spectra[i];
	}
      delete spectra;
    }


}


double Spectrum :: operator() (int i, int j)
{
  return spectra[i][j];
}


int Spectrum ::GetIMAX()
{
  if(variable)
    {
      //   return spectra.size();
    }
  else
    {
      return IMAX;
    }

}

int Spectrum :: GetJMAX()
{
  return JMAX;
}

void Spectrum :: WriteSpectrum(int i, std::string fileName)
{
  std::ofstream eFile;
  
  eFile.open(fileName.c_str());

  for(int j=0; j<JMAX; j++) 
    {
      eFile<<eGrid[j]<<"\t"<<spectra[i][j]<<"\n";	
    }
  eFile.close();
  eFile.clear();
  

  return;

}


// void Spectrum :: WriteSpectralEvolution()
// {
//   std::ofstream eFile;
//   std::stringstream filename;

//   for(int i=0; i<IMAX; i++)
//     {
//       filename.clear();
//       filename.str("");
//       filename<<"electrons/edist_"<<i<<".txt";  
//       eFile.open(filename.str().c_str());

//       for(int j=0; j<JMAX; j++) 
//       {
// 	//	eFile<<gamma[j]<<"\t"<<spectra[i][j]<<"\n";	
//       }
//       eFile.close();
//       eFile.clear();


//     }



//   return;
// }
void Spectrum :: InterpolateTimeSlice(int i, int jMin, int jMax)
{
  int jStep;
  jStep = jMax - jMin;
  
  acc = gsl_interp_accel_alloc ();
  spline = gsl_spline_alloc (gsl_interp_cspline, jStep);

  double tmpSpectrum[jStep], tmpEgrid[jStep]; // Allocate arrays to hold tmp values for interpolation

  for(int j=0; j<jStep; j++)
    {
      tmpSpectrum[j]=spectra[i][j+jMin];
      tmpEgrid[j]=eGrid[j+jMin];

    }
  

  gsl_spline_init (spline, tmpEgrid, tmpSpectrum, jStep); // Interpolate the slice

}

double Spectrum :: InterpolatedSpectrum(double eValue)
{
  return gsl_spline_eval (spline, eValue, acc);  
}

void Spectrum :: FreeGSL()
{
  
  //Clean up the GSL workspaces!!!!
  gsl_spline_free (spline); 
  gsl_interp_accel_free (acc);

}

void Spectrum :: WriteInterpolatedSpectrum(double gMin, double gMax, std::string fileName)
{
  std::ofstream eFile;
  
  eFile.open(fileName.c_str());

  for(double g=gMin; g<gMax; g+=10.) 
    {
      eFile<<g<<"\t"<<InterpolatedSpectrum(g)<<"\n";	
    }
  eFile.close();
  eFile.clear();
  

  return;


}
