#include "radiationProp.hh"


RadiationProp :: RadiationProp()
{

}

RadiationProp :: ~RadiationProp()
{

}

void RadiationProp :: ReadJet(Jet *jet)
{

  

  //Check to make sure that the # of collisions == # of hist records
  if(jet->GetCollisionHistory().size() != jet->GetJetHistory().size() )
    {
      std::cout<< "\nThere are "<<jet->GetCollisionHistory().size()<<" collisions\n";
      std::cout<< "and are  "<< jet->GetJetHistory().size() <<" hist records.\n";
      std::cout<< "EXITING!\n\n";
      return;
    }





  jetState = jet->GetJetHistory();
  collHist = jet->GetCollisionHistory();
  shell = jet->GetShells();
  NUM_HIST = jetState.size();
  std::cout<<"\nThere are "<< NUM_HIST << " collisions to be processed.\n\n";

  //CalculateOpacity();

  return;

}


void RadiationProp :: RadiateJet()
{
  

  for(int i=0; i<NUM_HIST;i++)
    {
      Radiator *rad = new Radiator();
      rad->SetEdot(1.E52);
      rad->ReadCollision(collHist[i]);
      


    }





}




void :: RadiationProp :: CalculateOpacity()//This should eventually be parallel. <<<<<<<<<<<<
{
  double emissionRadius, emissionTime, photonCollisionTime, tmpTime;
  double mass, radius, tmp, Kt;//Thomson Opacity
  SSRow row, row2;
  bool notFound, hitShell = false, rightShell =false;
  int j, k, m, nextShell, rowSize, frontShellIndx;
  

  Kt = 0.4;//cm^2/g

  std::cout<<"\n\n> > > > > > > > > > > > > > > > >\n\n";
  std::cout<<"Begining radiation propagation\n";
  // Need to find out if the radiation can escape.
  // We look at successive photon shell - mass shell collisions 
  // until tau = 1 or there are no more shells
  for(int i=0; i<NUM_HIST;i++)
    {

      std::vector<double> tau_i;
      
      emissionRadius = collHist[i]->radius;
      emissionTime = collHist[i]->time;
      frontShellIndx = collHist[i]->front;
      if (frontShellIndx != 0)
	{
	  // std::cout<<"\n\nLooking at collision "<<i<<" occuring at "<< emissionTime<<".\n";
	  
	  
      //Get the next shell for collision
	  row = jetState[i];
	  rowSize = row.size();
	  notFound = true;
	  
	  
	  j=0;
	    std::cout<<"Shell " <<frontShellIndx<< " is emitting radiation into:\n"<<std::endl;
	  while(notFound && (j<rowSize)) //This is the slow way but...
	    {
	      
	      if( row[j]->indx == frontShellIndx )
		{
		  std::cout<<"\tshell: "<<row[j-1]->indx<<"\n";
		  //The row[j-1]->indx is the next shell the light will hit
		  notFound =false;
		}
	      else
		{
		  j++;
		}
	    }
	  
	  
	  for(int lastJ=j-1;lastJ>=0;lastJ-- )
	    {
	      hitShell=false;
	     
	      nextShell = row[lastJ]->indx;
	      
	      //When would the light hit
	      photonCollisionTime = LightTravelTime(emissionRadius, row[nextShell]->radius, c*sqrt((row[nextShell]->gamma)*(row[nextShell]->gamma)-1.)/row[nextShell]->gamma,emissionTime  );
	      
	      // std::cout<< "The photon shell will collide with mass shell "<<nextShell<<"\n";
	      // std::cout<<" at T="<<photonCollisionTime<<"\n";
	      // std::cout<<"Shell "<<nextShell<<" dies at T="<<shell[nextShell]->GetDeathTime()<<" so";
	      
	      
	      //See if the shell is still alive
	      if(shell[nextShell]->GetDeathTime() > photonCollisionTime)
		{
		  // std::cout<<" we must find its state before that time.\n";
		  tmpTime=0.;
		  hitShell=true;
		  k=i+1;
		  while(tmpTime<=photonCollisionTime)
		    {
		      //Find the last time B4 the photon collides to get the jetState index
		      
		      tmpTime = collHist[k]->time;
		      k++;	      
		    }
		  //std::cout<<"The photon shell collides @ T="<<photonCollisionTime<<" and the tmp time is "<<tmpTime<<"\n";
		  k--;
		  //Now I need the shell's mass at t=c*(photonCollisionTime - emissionRadius)
		  //Need to search through the kth jetState
		  row2 = jetState[k];
		  m=0;
		  while(!rightShell)
		    {
		      if(row2[m]->indx == nextShell)
			{
			  rightShell = true;
			}
		      else
			{
			  m++;
			}
		    }
		  //Append these to the tau_i valarray;
		  mass = row2[m]->mass;
		  radius = c*(photonCollisionTime - emissionRadius);

		  tmp = mass/(4*radius*radius*3.14);
		  tau_i.push_back(tmp);
		  


		  
		}
	      else
		{
		  //The shell was not hit
		  //	  std::cout<<" we must find the next live shell.\n";
		}
	      
	    }
	  if(!hitShell)
	    {
	      	      std::cout<<"The photon shell from collision "<<i<< " propagates freely\n\n";
	    }
	  else
	    {
	      // std::cout<<"THe Photon shell collides\n\n\n\n"; 
	      std::cout<<"The number of photon-shell collision: "<<tau_i.size()<<"\n";
	      double opacity = 0.;
	      for(int y=0; y<tau_i.size(); y++)
		{
		  opacity+=tau_i[y];
		}
	      opacity*=Kt;
	      std::cout<<"The opacity is: "<<opacity<<std::endl;
	      
	      
	    }


	  
	}
      else
	{
	  std::cout<<"Shell "<<frontShellIndx<<" is emitting into free space.\n\n";
	}
      

    }
  

  return;

}





double  RadiationProp :: LightTravelTime(double emissionRadius, double shellRadius, double  shellVelocity, double emissionTime)
{
  float deltaR, Tf;

  deltaR = shellRadius-emissionRadius;
  Tf = deltaR/(c-shellVelocity) + emissionTime;
  return Tf;

}



// FILE WRITING FUNCTIONS
void RadiationProp :: WriteTrajectoryHistory()
{
  std::ofstream trajFile;
  std::stringstream filename;

  trajFile.precision(13);
  trajFile.setf(std::ios::fixed);
  trajFile.setf(std::ios::showpoint);
    

  SSRow row;

  for(int i=0; i<NUM_HIST;i++)
    {
      filename.clear();
      filename.str("");
      filename<<"trajectory/traj_"<<i<<".txt";      
      trajFile.open(filename.str().c_str());
      row = jetState[i];
      trajFile<<"radius"<<"\t"<<"mass"<<"\t"<<"gamma"<<"\t"<<"time\n";
      for(int j = 0; j<row.size();j++)//Loop over all the shells at this event
	{
	  trajFile<<row.at(j)->radius<<"\t";
	  trajFile<<row.at(j)->mass<<"\t";
	  trajFile<<row.at(j)->gamma<<"\t";
	  trajFile<<row.at(j)->time<<"\n";
	  
	  
	}
      
      //std::cout<< "Wrote "<<filename.str()<<std::endl;
      
      trajFile.close();
      trajFile.clear();
      
	


    }
  return;
}

void RadiationProp :: WriteCollisionHistory()
{
  std::ofstream collFile;
  std::stringstream filename;

  collFile.precision(13);
  collFile.setf(std::ios::fixed);
  collFile.setf(std::ios::showpoint);
  


  
  filename.clear();
  filename.str("");
  filename<<"trajectory/collision_hist.txt";      
  collFile.open(filename.str().c_str());
  collFile<<"radius"<<"\t"<<"radEnergy"<<"\t"<<"gammaR"<<"\t"<<"time\t"<<"B"<<"\t"<<"gmin\n";
  for(int i=0; i<NUM_HIST;i++)
    {
     
      collFile<<collHist[i]->radius<<"\t";
      collFile<<collHist[i]->radiatedEnergy<<"\t";
      collFile<<collHist[i]->gammaR<<"\t";
      // collFile<<collHist[i]->front<<"\t";
      //collFile<<collHist[i]->back<<"\t";
      
      collFile<<collHist[i]->time<<"\t";
      Radiator *rad = new Radiator();
      rad->SetEdot(2*1.E51/(12.4));
      rad->ReadCollision(collHist[i]);
      collFile<<rad->GetB()<<"\t";
      collFile<<rad->GetEGammaMin()<<"\n";
      //    delete rad;

      
    }

  //std::cout<< "Wrote "<<filename.str()<<std::endl;
      
  collFile.close();
  collFile.clear();
      
  return;
}
