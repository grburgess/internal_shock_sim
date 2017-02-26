
#include "jet.hh"
#include "radiationProp.hh"
#include "radiator.hh"

int main()
{
   Jet *jet = new Jet;
   RadiationProp radProp;

   jet->Initialize();
   jet->SetVariabilityTime(.002);
   jet->SetRmin(0.);
   jet->Start();


   radProp.ReadJet(jet );
   delete jet;
   radProp.WriteTrajectoryHistory();
   radProp.WriteCollisionHistory();

  // Radiator *rad = new Radiator();

  // rad->SetGammaR(220.);
  // rad->SetRadius(2.8E15);
  // rad->SetRadiatedEnergy(1.8E19);
  // rad->SetEdot(5.E52);
  // rad->SetupProperties();
  //rad->WriteElectronEvolution();

  return 0;
}

