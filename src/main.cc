// LOCAL
#include "graphicdisplay.h"
#include "controller.h"

// CPP
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main (int argc, char* argv[])
{
  int moves = 100, grid = 20;;
	Controller c;
	
	if(argc > 1)
  {
		istringstream iss(argv[1]);
		iss >> moves;
		c.SetMoves(moves);
	}
	
	if(argc > 2)
  {
		istringstream iss(argv[2]);
		iss >> grid;
		c.SetDimension(grid);
	}
  if (moves < 2 * grid) cerr << "Gonna be hard : )" << endl;
    
	c.Play();
  return 0;
}

