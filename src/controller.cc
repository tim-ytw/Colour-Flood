// CPP
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>

// LOCAL
#include "controller.h"
#include "game.h"
#include "graphicdisplay.h"
#include "mouseinput.h"

using namespace std;

extern const int COLOURTYPES;
const int DefaultMoves = 100;
const int DefaultGridDimension = 20;


Controller::Controller()
{
  quit_ = false;
  display_ = NULL;
  moves_ = DefaultMoves;
  gridDimension = DefaultGridDimension;
}


Controller::~Controller()
{
  delete display_;
  display_ = NULL;
  game_ = NULL;
}


void Controller::Play()
{
  quit_ = false;
  display_ = new GraphicDisplay(gridDimension);
  
  game_ = Game(this);
  MouseInput input(this);
  game_.Init(gridDimension, moves_);
  
  while (!quit_ && !game_.IsWon())
  {
    UpdateMessage(game_.GetGameStatus());
    if (!game_.Change(input.getMove()))
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  
  UpdateMessage(game_.GetGameStatus());
  if (!quit_) std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  
  delete display_;
  display_ = NULL;
}


void Controller::SetQuit()
{
  quit_ = true;
}


void Controller::SetMoves(int move)
{
  if (move <= 0 || move >= 1000)
  {
    cerr <<  move << " moves may be invalid" << endl;
  }
  else
  {
    moves_ = move;
  }
}

void Controller::SetDimension(int dimension)
{
  if (dimension <= 0)
  {
    cerr << "Dimension " << dimension << " may be invalid" << endl;
  }
  else if (dimension > 50)
  {
    cerr << "Dimension " << dimension << " does not look good" << endl;
  }
  else
  {
    gridDimension = dimension;
  }
}

void Controller::UpdateMessage(const std::string& message)
{
	display_->UpdateMessage(message);
}

void Controller::Notify(int row, int column, unsigned int state)
{
  display_->Notify(row, column, state);
}


int Controller::GetColor(int row, int col)
{
  return display_->GetColor(row, col);
}



