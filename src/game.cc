
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

int grid_size;

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
  grid_size = n;
  
	int rows = n, columns = n;
	grids_ = new int*[rows];
  visited_ = new bool*[rows];
	for (int r = 0; r < rows; r++)
	{
		grids_[r] = new int[columns];
    visited_[r] = new bool[columns];
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
      visited_[r][c] = false;
    }
	}
  
  ai_inputs = RunAI();
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
		controller_->UpdateMessage("Please click on a different colour : )");
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


inline int** CopyArray(int** arr, int size)
{
  int** new_arr = new int*[size];
  for (int r = 0; r < size; r++)
  {
    new_arr[r] = new int[size];
    for (int c = 0; c < size; c++)
    {
      new_arr[r][c] = arr[r][c];
    }
  }
  return new_arr;
}


int colours[COLOURTYPES+1];
int already = 1;
inline bool IsComplete(int** arr, int size)
{
  for (int r = 0; r <= COLOURTYPES; r++)
  {
    colours[r] = 0;
  }
  for (int r = 0; r < size; r++)
  {
    for (int c = 0; c < size; c++)
    {
      colours[arr[r][c]] ++;
    }
  }
  for (int i = 0; i <= COLOURTYPES; i++)
  {
    int num = colours[i];
    if (num != 0 && num != size * size)
    {
      return false;
    }
  }
  return true;
}



int Game::GetAIMove()
{
  int input = ai_inputs.top();
  ai_inputs.pop();
  return input;
}


void Show(int** arr, int size)
{
  for (int r = 0; r < size; r++)
  {
    for (int c = 0; c < size; c++)
    {
      cout << arr[r][c];
    }
    cout << endl;
  }
}



bool** visited;

void reset()
{
  for (int r = 0; r < grid_size; r++)
  {
    for (int c = 0; c < grid_size; c++)
    {
      visited[r][c] = false;
    }
  }
}


void Floodit(int** grids, int size, int row, int col, int prev_state, int new_state)
{
  if (visited[row][col]) return;
  visited[row][col] = true;
  if (grids[row][col] == prev_state)
  {
    grids[row][col] = new_state;
    if(row != 0) Floodit(grids, size, row-1, col, prev_state, new_state);
    if(col != 0) Floodit(grids, size, row, col-1, prev_state, new_state);
    if(row != size-1) Floodit(grids, size, row+1, col, prev_state, new_state);
    if(col != size-1) Floodit(grids, size, row, col+1, prev_state, new_state);
  }
}



int Count(int** grids, int size, int row, int col, int colour)
{
  if (visited[row][col]) return 0;
  visited[row][col] = true;
  int count = 0;
  if (grids[row][col] == colour)
  {
    count ++;
    if(row != 0) count += Count(grids, size, row-1, col, colour);
    if(col != 0) count += Count(grids, size, row, col-1, colour);
    if(row != size-1) count += Count(grids, size, row+1, col, colour);
    if(col != size-1) count += Count(grids, size, row, col+1, colour);
  }
  return count;
}




std::stack<int> FindMinMoves(int** arr, int size, int moves_allowed)
{
  if (IsComplete(arr, size)) return std::stack<int>();
  
  int origin_color = arr[0][0];
  
  int counts[COLOURTYPES+1];
  int** copies[COLOURTYPES+1];
  for (int i = 0; i <= COLOURTYPES; i++)
  {
    counts[i] = 0;
    copies[i] = NULL;
  }
  
  for (int i = 1; i <= COLOURTYPES; i++)
  {
    /* Pick a different colour */
    if (i == arr[0][0] || colours[i] == 0) continue;
    
    /* Try to do one flood */
    int** copy = CopyArray(arr, size);
    Floodit(copy, size, 0, 0, origin_color, i);
    reset();
    int count = Count(copy, size, 0, 0, copy[0][0]);
    reset();
    
    counts[i] = count;
    copies[i] = copy;
    if (count == size * size) break;
  }
  
  int max_color = 0;
  for (int i = 0; i <= COLOURTYPES; i++)
  {
    if (counts[i] > counts[max_color]) max_color = i;
  }
  std::stack<int> result = FindMinMoves(copies[max_color], grid_size, moves_allowed - 1);
  result.push(max_color);
  
  for (int i = 0; i <= COLOURTYPES; i++)
  {
    delete[] copies[i];
  }
  return result;
}


std::stack<int> Game::RunAI()
{
  visited = new bool*[grid_size_];
  for (int r = 0; r < grid_size_; r++)
  {
    visited[r] = new bool[grid_size_];
    for (int c = 0; c < grid_size_; c++)
    {
      visited[r][c] = false;
    }
  }
  
  int** grids = CopyArray(grids_, grid_size_);
  already = Count(grids, grid_size, 0, 0, grids[0][0]);
  std::stack<int> result = FindMinMoves(grids, grid_size_, moves_left_);
  
  vector<int> temp;
  cout << "AI Suggests:" << endl;
  while (!result.empty())
  {
    int color = result.top();
    cout << kColorNames[color] << endl;
    result.pop();
    temp.push_back(color);
  }
  while (!temp.empty())
  {
    result.push(temp.back());
    temp.pop_back();
  }
  cout << result.size() << " steps in total" << endl;
  
  for (int c = 0; c < grid_size_; c++)
  {
    delete grids[c];
  }
  delete grids;
  reset();
  
  return result;
}


