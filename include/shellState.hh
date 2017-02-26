#ifndef _SHELLSTATE_HH_
#define _SHELLSTATE_HH_
#include <vector>

struct ShellState
{
  double radius;
  double gamma;
  double mass;
  double time;
  int indx;
  
};

typedef std::vector<ShellState*> SSRow;

#endif
