#include "radiator.hh"
#include <time.h>

Radiator ::  Radiator()
{  
  p=2.5; eFrac = 1./3.; accelerationFrac=1.E-2; bFrac = 1./3.;
  gamma_min=10.; gamma_max = 1.E5; JMAX=200; KMAX=200;
}

Radiator ::  Radiator(double new_p, double new_eFrac, double  new_bFrac, double new_accelerationFrac)
{  
  p=new_p; eFrac = new_eFrac; accelerationFrac=new_accelerationFrac; new_bFrac=bFrac;
  JMAX = 200; KMAX=200;
 
}



Radiator :: ~Radiator()
{
  delete electrons;
 
}





void Radiator :: Initialize()
{
   double tCool;
  //  Determine the number of time steps
  //  for now this is fixed for each sim. 
  //  soon I want to implement a variable time step.
  
  //Calculate the cooling time of the most energetic electron from the shock.
  tCool = SynchrotronTimeScale(Egamma_max);
  std::cout<<"Tcool: "<<tCool<<"\n";

  tCool=4.; // OBVIOUSLY CHANGE THIS LATER

  deltaT = tCool;
  
  IMAX = (int)floor(tEsc/tCool);
  //IMAX = 10000;
  std::cout<<"There are  "<<IMAX<<" time steps.\n";
  
  electrons = new SynchrotronElectrons(IMAX, JMAX);
  photons = new Photons(IMAX, KMAX);
  
  
  electrons->SetB(B);
  electrons->SetTesc(tEsc);
  electrons->SetEGammaMax(Egamma_max);
  electrons->SetEGammaMin(Egamma_min);
  electrons->SetIndex(p);
  electrons->SetNacc(ne_acc);
  electrons->SetDeltaT(tCool);


  //Debuging
  // clock_t start, end;
  //double runTime;

  //Evolve the electrons;
  // start = clock();

  // Dont need this now
  //  electrons->Fokker();


  std::stringstream filename;

  // for (int i=0; i<=IMAX; i++)
  //   {
  //     filename.clear();
  //     filename.str("");
  //     filename<<"electrons/time_"<<i<<".txt";
  //     electrons->WriteSpectrum(i,filename.str().c_str());




  //    }
  // std::ofstream eFile;
  //std::stringstream filename;  
  // start = clock();
  
  //  end = clock();
  // runTime = (end - start) / (double) CLOCKS_PER_SEC ;
  //  std::cout<<runTime<<std::endl;
  //std::cout<<t[0]<<"\t"<<t[1]<<"\n";
  // delete t;

  //std::cout<<electrons->GetSpectralValue(10,150)<<"\n"; //test
// 
   //The photon

  clock_t start, end;
  double runTime;

  //Dont need this for right now

  
  // start = clock();
  // GeneratePhotonSpectrum(0);
  // photons->WriteSpectrum(0,"photons/test0.txt");
  // GeneratePhotonSpectrum(10);
  // photons->WriteSpectrum(10,"photons/test1.txt");
  // GeneratePhotonSpectrum(100);
  // photons->WriteSpectrum(100,"photons/test2.txt");
  // end = clock();
  //  runTime = (end - start) / (double) CLOCKS_PER_SEC ;
  // std::cout<<runTime<<std::endl;
  
}


double Radiator :: SynchrotronTimeScale(double gamma)
{
  double val;
  
  val = 6*pi*me*c;
  val /= (sigT * B*B*gamma);
  return val;

}




void Radiator :: ReadCollision(CollisionHist* colHist)
{
  gammaR = colHist->gammaR;
  radius = colHist->radius;
  radiatedEnergy = colHist->radiatedEnergy;
  SetupProperties(); // Calculate the radiation stuff 

  return;
 
}

void Radiator :: SetupProperties()
{
  double tmp1, tmp2, density; 


  //The density here
  density = Edot/(4*pi*radius*radius*gammaR*gammaR*pow(c,3)); //Again

  //std::cout<<"Density: "<<density<<"\n";
  
  //Calculate Tesc
  tEsc = radius/(gammaR*c);

  //std::cout<<"Tesc: "<<tEsc<<"\n";

  //Set the min and max energies for the electrons injected;
  Egamma_min = ((p-2.) * mp  * eFrac * radiatedEnergy)/( (p-1.) * me  * accelerationFrac  * c*c ); 
  

  //std::cout<<"Gamma Min: "<<Egamma_min<<"\n";
  
  //Calculate the comoving mag field;
  B = sqrt(8.*pi*bFrac*density*radiatedEnergy);


  tmp1 = sqrt((6.*pi*e)/(sigT * B));
  tmp2 = e*B*tEsc/(me*c);

  

  Egamma_max = std::min(tmp1,tmp2);

  //std::cout<<"Gamma Max: "<<Egamma_max<<"\n";
  //std::cout<<"B: "<<B<<"\n";


  ne_acc = accelerationFrac*density/mp;
  //std::cout<<"ne_acc: "<<ne_acc<<"\n\n";


  //Initialize the system
  //Initialize();

  
  return;
}


void Radiator :: GeneratePhotonSpectrum(int i)
{
  double result, error;
  int jMin, jMax, workspaceSize =1E9;
  double gMin, gMax;


  std::cout<<"Generate Pht "<<i<<"\n";
  
  //GSL setup 
  gsl_set_error_handler_off();
  
  gsl_integration_workspace *w = gsl_integration_workspace_alloc (workspaceSize);
  gsl_function F;
  F.function = &Radiator::EmissionIntegrand;


 
  
  //Interpolate the electrons
  int *minMax = NULL;

  minMax = electrons->Find_Min_Max(i); //This may or may not change
 std::cout<<"here\n";  
  jMin=minMax[0]; jMax=minMax[1];
  delete minMax; //Clean up
  
  electrons->InterpolateTimeSlice(i,jMin,jMax); //The timeslice is interp'd

  
  //electrons->WriteSpectrum(0,"test.txt");
  
  // Get the max and min electron gammas from the eGrid
  
  gMin=electrons->GetEnergyValue(jMin+1);
  gMax=electrons->GetEnergyValue(jMax-1);
  electrons->WriteInterpolatedSpectrum(gMin, gMax,"test2.txt");
  std::cout<<gMin<<"***\n";
  std::cout<<gMax<<"***\n";
  
  
  //Loop through the photon energy grid and fill it with the emission 
  for(int j=0; j<JMAX; j++)
    {
      
      nu = photons->GetEnergyValue(j);//Grab the nu value
      // std::cout<<"NU "<< nu<<" "<<this->GetNu()<<"\n\n";
      F.params = this; //Set the GSL func pointer param to nu   
      gsl_integration_qag (&F, gMin, gMax, 0, 1e-5,6,workspaceSize,w, &result, &error);//intergrat
      
      photons->FillSpectrum(i,j,result);
    }
  
  
  electrons->FreeGSL(); //Clean up the electron gsl interpolation
  gsl_integration_workspace_free(w); //Free the workspace
  
  return;
  
}

double Radiator :: EmissionIntegrand(double gamma, void *p)
{
  double val;
  

  //  std::cout<<gamma<<"\n";
  Radiator *objPointer = (Radiator *) p;
  
  val = objPointer->electrons->InterpolatedSpectrum(gamma)*(objPointer->P_syn((objPointer->GetNu()),gamma));
  
  val/=(h*( objPointer->GetNu()));
 
  return val;
}

double Radiator :: P_syn(double new_nu, double gamma)
{
  double nu_syn, val, tmp;
  gsl_sf_result result;
  int status;
  
  nu_syn =3*e*B*B*gamma*gamma/(4*pi*me*c);
  status =  gsl_sf_synchrotron_1_e (new_nu/nu_syn, &result);
  if(status!=0)
    {
      tmp=0.0;
    }
  else
    {
      tmp = result.val;
    }
  
  val = sigT*me*c*c/(3*e)*B*tmp;

  return val;

}
