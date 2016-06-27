
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
  
  cout << "Suggest: " << endl;
  while (!ai_inputs.empty())
  {
    cout << ai_inputs.top() << endl;
    ai_inputs.pop();
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

int color_left[COLOURTYPES];
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


int already = 0;


int Floodit(int** grids, int size, int row, int col, int prev_state, int new_state)
{
  if (visited[row][col]) return 0;
  visited[row][col] = true;
  if (grids[row][col] == prev_state)
  {
    int count = 1;
    grids[row][col] = new_state;
    if(row != 0) count += Floodit(grids, size, row-1, col, prev_state, new_state);
    if(col != 0) count += Floodit(grids, size, row, col-1, prev_state, new_state);
    if(row != size-1) count += Floodit(grids, size, row+1, col, prev_state, new_state);
    if(col != size-1) count += Floodit(grids, size, row, col+1, prev_state, new_state);
    return count;
  }
  return (grids[row][col] == new_state)? 1 : 0;
}


std::stack<int> FindMinMoves(int** arr, int size, int moves_allowed)
{
  if (IsComplete(arr, size)) return std::stack<int>();
  
  int origin_color = arr[0][0];
  std::vector<std::stack<int>> results;
  
  for (int i = 1; i <= COLOURTYPES; i++)
  {
    if (i == arr[0][0] || colours[i] == 0) continue;
    
    int** copy = CopyArray(arr, size);
    Show(copy, size);
    cout << "Flood colour: " << i << endl;
    int count = Floodit(copy, size, 0, 0, origin_color, i);
    Show(copy, size);
    cout << "Count: " << count << endl;
    reset();
    std::stack<int> result;
    if (count > already)
    {
      already = count;
      if (count != size * size)
      {
        result = FindMinMoves(copy, grid_size, moves_allowed - 1);
        cout << "Get result: " << result.size() << endl;
      }
      result.push(i);
      results.push_back(result);
    }
    delete copy;
    if (count == size * size || result.size() == 1) break;
  }
  
  int min_index = 0;
  for (int i = 0; i < results.size(); i++)
  {
    if (results[i].size() < results[min_index].size())
    {
      min_index = i;
    }
  }
  
  cout << "Current min: " << results[min_index].size() << endl;
  return results.empty()? std::stack<int>() : results[min_index];
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
  for (int i = 0; i <= COLOURTYPES; i++)
  {
    color_left[i] = colours_[i];
  }
  
  int** grids = CopyArray(grids_, grid_size_);
  std::stack<int> result = FindMinMoves(grids, grid_size_, moves_left_);
  cout << "[RunAI] result: " << result.size() << endl;
  
  for (int c = 0; c < grid_size_; c++)
  {
    delete grids[c];
  }
  delete grids;

  for (int r = 0; r < grid_size_; r++)
  {
    delete visited[r];
  }
  delete visited;
  
  return result;
}


