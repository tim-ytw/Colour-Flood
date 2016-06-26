// LOCAL
#include "graphicdisplay.h"
#include "cell.h"
#include "game.h"

using namespace std;

extern const int MAXNEIGHBOURS;
extern const int kBackgroundColor;

Cell::Cell()
{
  row_ = 0;
  column_ = 0;
	state_ = kBackgroundColor;
	prev_state_ = kBackgroundColor;
	numNeighbours_ = 0;
	for (int i = 0; i < MAXNEIGHBOURS; i++)
	{
		neighbours_[i] = NULL;
	}
}


Cell::~Cell()
{
	for (int i = 0; i < MAXNEIGHBOURS; i++)
	{
    neighbours_[i] = NULL;
	}
	game_ = NULL;
}


int Cell::GetState()const
{
	return state_;
}


void Cell::SetState(const int& change)
{
	prev_state_ = state_;
	state_ = change;
}

void Cell::setCoords(const int row, const int column)
{
  row_ = row;
	column_ = column;
}

void Cell::SetGame(Game* game)
{
  game_ = game;
}

void Cell::AddNeighbour(Cell* neighbour)
{
	if (numNeighbours_ < MAXNEIGHBOURS)
	{
    neighbours_[numNeighbours_++] = neighbour;
	}
}


// This should ONLY be called by GAME to notify colour change.
void Cell::Notify(const int &change)
{
	SetState(change);
  Notify();
}


// A Neighbour may call this method to change its state.
void Cell::Notify(const int& current, const int& previous)
{
	if (previous == state_ && current != state_)
	{
		SetState(current);
    Notify();
	}
}


void Cell::Notify() const
{
  /* Render Current Cell first */
  game_->Notify(row_, column_, prev_state_, state_);
  
  /* Then render its neighbours */
  for (int i = 0; i < numNeighbours_; i++)
  {
    neighbours_[i]->Notify(state_, prev_state_);
  }
}
