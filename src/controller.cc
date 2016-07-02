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
#include "FloodAI.h"

using namespace std;

extern const int COLOURTYPES;
const int DefaultMoves = 100;
const int DefaultGridDimension = 20;


Controller::Controller()
{
  ai_ = false;
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
  
  FloodAI* ai = new FloodAI(game_.GetGrids(), gridDimension, moves_);
  ai->Init();
  
  int move = 0;
  string game_status;
  while (!quit_)
  {
    move = ai_? ai->GetMove() : input.GetMove();
    if (game_.Change(move))
    {
      UpdateMessage(game_.GetGameStatus());
      quit_ = game_.IsWon();
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  }
  
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  
  delete ai;
  delete display_;
  display_ = NULL;
}


void Controller::SetQuit()
{
  quit_ = true;
}

void Controller::SetAI(bool ai)
{
  ai_ = ai;
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



