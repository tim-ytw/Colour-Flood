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
#include "game_param.h"

using namespace std;

extern const int COLOURTYPES;
const int DefaultMoves = 100;
const int DefaultGridDimension = 20;


Controller::Controller()
{
  use_ai_ = false;
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
  game_ = Game(this);
  input_ = new MouseInput(this);
  display_ = new GraphicDisplay(gridDimension);
  
  game_.Init(gridDimension, moves_);
  ai_ = new FloodAI(game_.GetGrids(), gridDimension, moves_, this);
  
  status_ = IN_PROGRESS;
  quit_ = false;
  while (!quit_ && status_ == IN_PROGRESS)
  {
    int move = GetInput();
    if (move != 0)
    {
      game_.Change(move);
      UpdateMessage();
    }
  }
  
  if (!quit_) std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  
  delete ai_;
  delete display_;
  display_ = NULL;
}


void Controller::UpdateMessage()
{
  string game_status = "";
  status_ = game_.GetGameStatus(game_status);
  if (status_ == IN_PROGRESS)
  {
    game_status += "\n";
    if (use_ai_)
    {
      game_status = "[AI is playing]\n"+game_status;
    }
    else
    {
      int color_suggested = ai_->Recommend(game_.GetGrids(), gridDimension);
      game_status = game_status + "[AI suggests "+kColorNames[color_suggested]+"]";
    }
  }
  display_->UpdateMessage(game_status);
}


void Controller::AddInput(int color)
{
  if (!use_ai_)
  {
    inputs_.push(color);
  }
}


int Controller::GetInput()
{
  if (use_ai_)
  {
    /* Flushes inputs to prevent window hang */
    input_->Flush();
    /* Prevents GUI from being rendered too frequently */
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }
  
  /* Existing inputs may come from either AI or the user */
  if (!inputs_.empty())
  {
    int input = inputs_.front();
    inputs_.pop();
    return input;
  }
  
  /* Otherwise, wait for user input */
  return input_->GetMove();
}


void Controller::SetQuit()
{
  quit_ = true;
}

void Controller::SetAI(bool ai)
{
  use_ai_ = ai;
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



