#ifndef GAME_H
#define GAME_H

// CPP
#include <stdlib.h>
#include <string>

const int COLOURTYPES = 5;

class Controller;
class Cell;

class Game
{
public:
	Game(Controller* gn = NULL);
	~Game();
  
	bool IsWon() const;
  bool Change(const int& state);
  std::string GetGameStatus() const;
  void Init(int grid_size, int moves);
  void Notify(int row, int column, unsigned int oldState, unsigned int newState);

private:
  Controller *controller_;
  Cell **grids_;
  unsigned int colours_[COLOURTYPES+1];
  int grid_size_;
  int moves_left_;
};

#endif
