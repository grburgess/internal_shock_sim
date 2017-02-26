#ifndef _JET_HH_
#define _JET_HH_


#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <valarray>
#include "shell.hh"
#include "collisionHist.hh"
#include "shellState.hh"

class Jet
{
public:
  Jet();
  ~Jet();

  void Initialize();
  void Start();
  void SetVariabilityTime(double TV){tv = TV; return;};
  void SetRmin(double rmin){Rmin=rmin; return;};


  double GetTotalEnergy();
  std::vector<CollisionHist*> GetCollisionHistory(){return collHist;};
  std::vector<SSRow> GetJetHistory(){return jetState;};
  Shell ** GetShells(){return shell;};


private:


  //File reading
  typedef std::vector<double> Row;
  std::vector<Row> table;
  std::ifstream file;

  //Shell setup
  int NUM_SHELLS;
  Shell **shell;
  
  std::vector<CollisionHist*> collHist;

  
  std::vector<SSRow> jetState;

  bool status;
  double time, tv, Rmin;
  int nextShellEmitted;
  
  void AdvanceTime();
  void MoveShells(double dT);
  void RecordJetState();
  void EmitShell(int i);
  void CheckJet();
  double DeltaTime(Shell *back, Shell *front);
  

  


};

#endif
