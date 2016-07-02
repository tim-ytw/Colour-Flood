#ifndef CONTROLLER_H
#define CONTROLLER_H

// CPP
#include <iostream>
#include <queue>

// LOCAL
#include "game.h"
#include "view.h"

class FloodAI;
class MouseInput;

class Controller
{
  friend class FloodAI;
  
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
  void AddInput(int color);
  void UpdateMessage();
  void UpdateMessage(const std::string& message);
	void Notify(int row, int column, unsigned int state);	
  
private:
  int GetInput();
  
  bool quit_;
  bool use_ai_;
  int moves_;
  int gridDimension;
  std::queue<int> inputs_;
  
  MouseInput* input_;
  View *display_;
  FloodAI* ai_;
  Game game_;
};

#endif
