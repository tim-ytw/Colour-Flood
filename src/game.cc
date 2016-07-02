
// LOCAL
#include "game.h"
#include "controller.h"
#include "graphicdisplay.h"

// CPP
#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

extern const int kBackgroundColor;
extern const int COLOURTYPES;


Game::Game(Controller* gn)
{
  grid_size_ = 0;
  controller_ = gn;
  moves_left_ = 0;
  grids_ = NULL;
}


Game::~Game()
{
  int rows = grid_size_;
  for (int r = 0; r < rows; r++)
  {
    delete[] grids_[r];
  }
  delete[] grids_;
}



void Game::Init(int n, int _moves)
{
  moves_left_ = _moves;
  grid_size_ = n;
  
	int rows = n, columns = n;
	grids_ = new int*[rows];
	for (int r = 0; r < rows; r++)
	{
		grids_[r] = new int[columns];
	}
  for (int i = 0; i < COLOURTYPES; i++)
  {
    colours_[i] = 0;
  }
  
  srand((unsigned int)time(NULL));
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			int color = rand() % COLOURTYPES + 1;
			colours_[color] ++;
      grids_[r][c] = color;
			controller_->Notify(r, c, color);
    }
	}
  
  controller_->UpdateMessage(GetGameStatus());
}


bool Game::IsWon()const
{
  for (int i = 0; i < COLOURTYPES; i++)
  {
    int num = colours_[i];
    if (num != 0 && num != grid_size_ * grid_size_)
    {
      return false;
    }
  }
	return true;
}


bool Game::Change(const int& state)
{
  if (state < 0) return false;
	if (state == grids_[0][0])
	{
		controller_->UpdateMessage("Try a different colour");
		return false;
	}
  moves_left_ --;
  int prev_state = grids_[0][0];
  Flood(0, 0, prev_state, state);
	return true;
}



void Game::Flood(int row, int col, int prev_state, int new_state)
{
  if (grids_[row][col] == prev_state)
  {
    grids_[row][col] = new_state;
    Notify(row, col, prev_state, new_state);
    if(row != 0) Flood(row-1, col, prev_state, new_state);
    if(col != 0) Flood(row, col-1, prev_state, new_state);
    if(row != grid_size_-1) Flood(row+1, col, prev_state, new_state);
    if(col != grid_size_-1) Flood(row, col+1, prev_state, new_state);
  }
}


void Game::Notify(int row, int column, unsigned int old_state, unsigned int new_state)
{
  colours_[old_state] --;
  colours_[new_state] ++;
	controller_->Notify(row, column, new_state);
}



string Game::GetGameStatus() const
{
  if (IsWon() && moves_left_ > 0)
  {
    return "Won";
  }
  else if (moves_left_ <= 0)
  {
    controller_->SetQuit();
    return "Lost";
  }
  else
  {
    ostringstream sout;
    sout << moves_left_ << ((moves_left_ > 1)? " moves":"move") <<  " left";
    return sout.str();
  }
}



int** Game::GetGrids() const
{
  return grids_;
}


