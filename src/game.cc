
// LOCAL
#include "game.h"
#include "cell.h"
#include "controller.h"

// CPP
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

extern const int kBackgroundColor;
extern const int COLOURTYPES;


Game::Game(Controller* gn)
{
  grids_ = NULL;
  grid_size_ = 0;
  controller_ = gn;
  moves_left_ = 0;
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
	grids_ = new Cell*[rows];
	for (int r = 0; r < rows; r++)
	{
		grids_[r] = new Cell[columns];
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
			
			Cell* current = &(grids_[r][c]);
			current->SetGame(this);
			current->setCoords(r, c);
			current->SetState(color);
			
			controller_->Notify(r, c, color);
			
			if(r != rows-1) grids_[r+1][c].AddNeighbour(current);
			if(c != columns-1) grids_[r][c+1].AddNeighbour(current);
			if(r != 0) grids_[r-1][c].AddNeighbour(current);
			if(c != 0) grids_[r][c-1].AddNeighbour(current);
		}
	}
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
	if (state == grids_[0][0].GetState())
	{
		controller_->UpdateMessage("Please click on a different colour :)");
		return false;
	}
  moves_left_ --;
	grids_[0][0].Notify(state);
	return true;
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
    return "You Win";
  }
  else if (moves_left_ == 0)
  {
    return "You Lost";
  }
  else
  {
    ostringstream sout;
    sout << moves_left_ << " move(s) left";
    return sout.str();
  }
}

