#ifndef CONTROLLER_H
#define CONTROLLER_H

// CPP
#include <iostream>

// LOCAL
#include "game.h"
#include "view.h"

class Controller
{
public:
	Controller();
	~Controller();
	
  /* Setters */
  void SetAI(bool ai);
  void SetQuit();
	void SetMoves(int move);
	void SetDimension(int dimension);
	
	void Play();
	int GetColor(int row, int col);
  void UpdateMessage(const std::string& message);
	void Notify(int row, int column, unsigned int state);	
  
private:
  int moves_;
  int gridDimension;
  bool quit_;
  bool ai_;
  
  View *display_;
  Game game_;
};

#endif
