#include "jet.hh"

Jet :: Jet()
{
  nextShellEmitted = 0;
}
Jet :: ~Jet(){}

void Jet :: AdvanceTime()
{
  


  CollisionHist *collision = NULL; //This will be returned from Shell :: Collide
  double minimumDeltaT = 1E300, dT;
  double timeTillNextEmission = tv;
  int collisionIndx, nextShell, bestNext;
  bool notFound;
  std::vector<double> allDT;
  std::vector<int> allCollisionIndx;
  //std::vector<double> dT;



  
  int numActive = 0;

 
  
  for(int i=0; i< NUM_SHELLS-1; i++) // -1 bc there are no shells behind the last shell !!!
    {
      
      // Check to make sure the shell is still active
      if( (shell[i])->Status())
	{
	  numActive++;
	  notFound=true;
	  //First we must find the next active shell
	  nextShell = i+1;
	  while(notFound &&  (nextShell<NUM_SHELLS))
	    {
	      if(shell[nextShell]->Status())
		{
		  notFound =false;
		}
	      else
		{
		  nextShell++;
		}
	    }
	  

	  //  std::cout<<"This shell: "<<i<<"\n";
	  // std::cout<<"Nextshell: "<<nextShell<<"\n";

	  

	  

	   // if(nextShell==NUM_SHELLS && nextShellEmitted>=NUM_SHELLS)
	   //   {
	      
	   //     std::cout << "\n>>\tNo more collisions----- > NEXTSHELL == NUM_SHELLS\n";
	   //     status = false;
	   //     return;
      
	   //   }

	  //Is the coming shell slower than this shell?
	  if ( (shell[i])->GetGamma() < (shell[nextShell])->GetGamma() && nextShell < NUM_SHELLS)
	    {
	      //std::cout<<"here\n";
	       // std::cout<<"\n\nFRONT: "<<i<<"\n";
	       // shell[i]->PrintShellInfo();
	       // std::cout<<"\n\nBACK: "<<nextShell<<"\n";
	       // shell[nextShell]->PrintShellInfo();

	      
	      dT = DeltaTime(shell[nextShell] /*Back*/  ,shell[i] /*Front*/) ;
	      allDT.push_back(dT);

	      //  std::cout<<dT<<std::endl;

	      if( dT < minimumDeltaT )
		{
		  
		  collisionIndx = i;

		  bestNext = nextShell;
		  minimumDeltaT = dT;

	
		  
		}

	      
	      
	      
	    }

	  else //No shells colliding this time!
	    {
	      dT = 1E5; //Set a dummy dT
	    }


	}

    }


 


 

  if(collisionIndx == -1 and nextShellEmitted>=NUM_SHELLS)
    {
      std::cout << "\n>>\tNo more collisions\n";
      status = false;
      return;
    }

  // std::cout<<"DT: "<<dT<<"\n";


  // Here we must check if the collision time
  // is less than the emission time.
  // If not then we need to advace to the emission time
  // Of the next shell
  
  if( (dT > timeTillNextEmission) && (nextShellEmitted<NUM_SHELLS))
    {
      // Set minimumDeltaT to the next emission time
      minimumDeltaT = timeTillNextEmission;
      MoveShells(minimumDeltaT);   //Move the shells to the emission time
      time+=minimumDeltaT;        //Advance the time
      
      EmitShell(nextShellEmitted);   //Emit the shell      
      nextShellEmitted++;
      timeTillNextEmission=tv;
      //      std::cout << "Emitting shell "<<nextShellEmitted-1<<" at t="<<time<<"\n\n";
    }

  else
    {
     
      MoveShells(minimumDeltaT);
      time+=minimumDeltaT;
      timeTillNextEmission-=minimumDeltaT; //Reduce the timeTillNextEmission

      //     std::cout<< "\n************************ Colliding shells "<<collisionIndx<<" and "<< bestNext <<"\n\n";

      collision = (shell[collisionIndx])->Collide(shell[bestNext]);
  
      //Deactivate the destroyed shells
      shell[bestNext]->Deactivate(time);
      // std::cout<<"\n-------> Shell "<<bestNext<<" is now dead\n\n";
      // std::cout<<"\n>>>>>>>> Shell "<<collisionIndx<< "'s updated info:\n";
      // shell[collisionIndx]->PrintShellInfo();

      collision->time = time;
      collision->front = collisionIndx;
      collision->back = bestNext;
  
      //collision->opacity = Opactity(collisionIndx);

      collHist.push_back(collision);
      RecordJetState();

    }

  // std::cout<<"numActive shells: "<<numActive<<"\n";
  
  //Assess the order of the Lorentz factors
  if(nextShellEmitted>=NUM_SHELLS)
    {
      CheckJet();
    }


  return;

}



double Jet :: DeltaTime(Shell *back, Shell *front)
{
  double deltaTime;
 
  if (front->GetRadius() < back->GetRadius() )
    {
      std::cout << "Shells are backward in Jet::DeltaTime()\n";
      return -1.;
    }

  deltaTime = (front->GetRadius() - back->GetRadius())/(back->GetVelocity() - front->GetVelocity());

  
  return deltaTime;
}



void Jet :: MoveShells(double dT)
{
 for(int i=0; i< NUM_SHELLS; i++)
    {
      // Check to make sure the shell is still active
      if( (shell[i])->Status())
	{

	  (shell[i])->MoveShell(dT);
	}

    }

 return;
}


void Jet :: Initialize()
{
  /*
    This reads in the txt file with the inital info
    and populates the shells.


   */

  // Data type where to load the table

  std::ifstream file;


  file.open("initialize.txt");

  while (file)
    {
      std::string line;
      std::getline(file, line);
      std::istringstream is(line);
      Row row;
      while (is)
        {
          double data;
          is >> data;
          row.push_back(data);
        }
      table.push_back(row);
    }

  file.close();
  NUM_SHELLS = table.size() - 1;

  shell = new Shell*[NUM_SHELLS];

  for(int i=0;i<NUM_SHELLS;i++)
    {
      //  std::cout<<"\n\nReading the "<<i<<"th shell params.\n\n";
      //Allocate the shell 
      shell[i] = new Shell;
      //Set values from file

      //Modified now to try new emission scheme
      (shell[i])->SetRadius( Rmin );    //Uncomment to return to original

      (shell[i])->SetGamma ( (table[i])[2] );
      (shell[i])->SetMass  ( (table[i])[3] );

      // std::cout<<"SHELL: "<<i<<"\n";
      // (shell[i])->PrintShellInfo();



  

    }







      std::cout<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< GRB SIM >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n\n";

 
      std::cout<< "                                                                                      ]    |\n";
      std::cout<< "                                                                                 ]    ]    |\n";
      std::cout<< "                                                                            )    ]    ]    |\n";
      std::cout<< "                                                                            )    ]    ]    |\n";
      std::cout<< "                                                                      )     )    ]    ]    |\n";
      std::cout<< "                                                                  )   )     )    ]    ]    |\n";     
      std::cout<< "                                                              )   )   )     )    ]    ]    |\n";      
      std::cout<< ">>>>>>>>>>>> > > > > > > > > > > > >  >  >   >  >  >   >   >   )   )   )     )    ]    ]    |\n";
      std::cout<< ">>>>>>>>>>>> > > > > > > > > > > > >  >  >   >  >  >   >   >   )   )   )     )    ]    ]    |\n";      
      std::cout<< ">>>>>>>>>>>> > > > > > > > > > > > >  >  >   >  >  >   >   >   )   )   )     )    ]    ]    |\n";      
      std::cout<< ">>>>>>>>>>>> > > > > > > > > > > > >  >  >   >  >  >   >   >   )   )   )     )    ]    ]    |\n";      
      std::cout<< ">>>>>>>>>>>> > > > > > > > > > > > >  >  >   >  >  >   >   >   )   )   )     )    ]    ]    |\n";      
      std::cout<< "                                                              )   )   )     )    ]    ]    |\n";
      std::cout<< "                                                                  )   )     )    ]    ]    |\n";
      std::cout<< "                                                                      )     )    ]    ]    |\n";
      std::cout<< "                                                                            )    ]    ]    |\n";
      std::cout<< "                                                                            )    ]    ]    |\n";
      std::cout<< "                                                                                 ]    ]    |\n";
      std::cout<< "                                                                                      ]    |\n\n";
     
      std::cout<< "|||||||||||||                    Ejecting "<<NUM_SHELLS <<" shells into OUTER SPACE             |||||||||||\n";      
     


  return;

}

void Jet :: Start()
{
  status = true; 
  time = 0.;


  // Here is where the first shell is emitted

  (shell[nextShellEmitted])->Activate(time);



  MoveShells(tv);              //Advance the shells
  time+=tv;                    //Advance the time by the variablitity time
  nextShellEmitted++;



  //Emit the second shell
  (shell[nextShellEmitted])->Activate(time);

  MoveShells(tv);
  time+=tv;
  nextShellEmitted++;


    

  // Unitl the jet is monotonically ordered in Gamma
  // Keep iterating 
  while(status)
    {

      AdvanceTime();

    }

  for(int i=0; i< NUM_SHELLS; i++)
    {
      if( (shell[i])->Status())
	{
	  shell[i]->Deactivate(time);

	}
    }


  std::cout<< "\n<<<<<< Jet Complete >>>>>>\n";



}

void Jet :: RecordJetState()
{
  SSRow ssr;

  for(int i=0; i< NUM_SHELLS; i++) 
    {
      

      // Check to make sure the shell is still active
      if( (shell[i])->Status())
	{
	  ShellState *ss = new ShellState;
	  ss->radius = shell[i]->GetRadius();
	  ss->gamma = shell[i]->GetGamma();
	  ss->mass = shell[i]->GetMass();
	  ss->time = time;
	  ss->indx = i;
	  ssr.push_back(ss);
	  
	}

    }

  jetState.push_back(ssr);
  return;
 
}


void Jet :: EmitShell(int i)
{

  (shell[i])->Activate(time);
  //(shell[i])->SetRadius(Rmin);
  

  return;

}

void Jet :: CheckJet()
{
  double thisGamma;
  
  
 for(int i=0; i< NUM_SHELLS-1; i++)
    {


      // Check to make sure the shell is still active
      if( (shell[i])->Status())
	{


	  thisGamma = (shell[i])->GetGamma();
	  
	  for (int j =i+1; j<NUM_SHELLS; j++ )
	    {

	      // Check to make sure the shell is still active
	      if( (shell[j])->Status())
		{

		  if(thisGamma < (shell[j])->GetGamma())
		    {

		      // std::cout<<"At least one shell is faster than the rest\n";
		      // std::cout<<"FRONT: "<<i<<"\n";
		      // (shell[i])->PrintShellInfo();
		      // std::cout<<"BACK: "<<j<<"\n";
		      // (shell[j])->PrintShellInfo();


		      return;
		    }



		}

	    }

	}

    }


 status = false;
 return;

}



