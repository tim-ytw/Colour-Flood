#ifndef GAME_H
#define GAME_H

// CPP
#include <stack>
#include <stdlib.h>
#include <string>

const int COLOURTYPES = 5;

class Controller;

class Game
{
public:
	Game(Controller* gn = NULL);
	~Game();
  
	bool IsWon() const;
  bool Change(const int& state);
  int GetAIMove();
  std::string GetGameStatus() const;
  void Init(int grid_size, int moves);
  void Notify(int row, int column, unsigned int oldState, unsigned int newState);

private:
  std::stack<int> RunAI();
  void Flood(int row, int column, int prev_state, int new_state);
  
  std::stack<int> ai_inputs;
  Controller *controller_;
  unsigned int colours_[COLOURTYPES+1];
  int grid_size_;
  int moves_left_;
  int **grids_;
  bool **visited_;
};

#endif
