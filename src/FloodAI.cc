//
//  FloodAI.cpp
//  ColourFlood
//
//  Created by Tim on 2016-07-01.
//  Copyright © 2016 Tim. All rights reserved.
//


// CPP
#include <iostream>
#include <vector>

// LOCAL
#include "graphicdisplay.h"
#include "FloodAI.h"



FloodAI::FloodAI(int** arr, int size, int moves)
{
  grids_ = arr;
  grid_size_ = size;
  colours_ = new int[COLOURTYPES+1];
  visited_ = new bool*[grid_size_];
  for (int r = 0; r < grid_size_; r++)
  {
    visited_[r] = new bool[grid_size_];
    for (int c = 0; c < grid_size_; c++)
    {
      visited_[r][c] = false;
    }
  }
}


FloodAI::~FloodAI()
{
  delete colours_;
  for (int r = 0; r < grid_size_; r++)
  {
    delete[] visited_[r];
  }
  delete[] visited_;
}


int FloodAI::GetAIMove()
{
  int input = ai_inputs.back();
  ai_inputs.pop_back();
  return input;
}




Moves FloodAI::FindMinMoves(int** arr, int size, int moves_allowed)
{
  if (IsComplete(arr, size)) return Moves();
  
  int origin_color = arr[0][0];
  
  int counts[COLOURTYPES+1];
  int** copies[COLOURTYPES+1];
  for (int color = 0; color <= COLOURTYPES; color++)
  {
    counts[color] = 0;
    copies[color] = NULL;
  }
  
  for (int color = 1; color <= COLOURTYPES; color++)
  {
    /* Pick a different colour */
    if (color == origin_color || colours_[color] == 0) continue;
    
    /* Try to do one flood */
    int** copy = CopyArray(arr, size);
    Floodit(copy, size, 0, 0, origin_color, color);
    ResetVisited();
    int count = Count(copy, size, 0, 0, copy[0][0]);
    ResetVisited();
    
    /* Record its result */
    counts[color] = count;
    copies[color] = copy;
    if (count == size * size) break;
  }
  
  /* Compare the results of flooding with different colours */
  int max_color = 0;
  for (int color = 0; color <= COLOURTYPES; color++)
  {
    if (counts[color] > counts[max_color]) max_color = color;
  }
  
  /* Pick the best try and recurse */
  Moves result = FindMinMoves(copies[max_color], grid_size_, moves_allowed - 1);
  result.push_back(max_color);
  
  for (int color = 0; color <= COLOURTYPES; color++)
  {
    delete[] copies[color];
  }
  return result;
}



void FloodAI::RunAI()
{
  
  int** grids = CopyArray(grids_, grid_size_);
  Count(grids, grid_size_, 0, 0, grids[0][0]);
  Moves result = FindMinMoves(grids, grid_size_, moves_left_);
  
  for (int c = 0; c < grid_size_; c++)
  {
    delete grids[c];
  }
  delete grids;
  
  ai_inputs = result;
}



void FloodAI::Floodit(int** grids, int size, int row, int col, int prev_state, int new_state)
{
  if (visited_[row][col]) return;
  visited_[row][col] = true;
  if (grids[row][col] == prev_state)
  {
    grids[row][col] = new_state;
    if(row != 0) Floodit(grids, size, row-1, col, prev_state, new_state);
    if(col != 0) Floodit(grids, size, row, col-1, prev_state, new_state);
    if(row != size-1) Floodit(grids, size, row+1, col, prev_state, new_state);
    if(col != size-1) Floodit(grids, size, row, col+1, prev_state, new_state);
  }
}


int FloodAI::Count(int** grids, int size, int row, int col, int colour)
{
  if (visited_[row][col]) return 0;
  visited_[row][col] = true;
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


inline bool FloodAI::IsComplete(int** arr, int size)
{
  for (int r = 0; r <= COLOURTYPES; r++)
  {
    colours_[r] = 0;
  }
  for (int r = 0; r < size; r++)
  {
    for (int c = 0; c < size; c++)
    {
      colours_[arr[r][c]] ++;
    }
  }
  for (int i = 0; i <= COLOURTYPES; i++)
  {
    int num = colours_[i];
    if (num != 0 && num != size * size)
    {
      return false;
    }
  }
  return true;
}


inline int** FloodAI::CopyArray(int** arr, int size)
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


void FloodAI::ResetVisited()
{
  for (int r = 0; r < grid_size_; r++)
  {
    for (int c = 0; c < grid_size_; c++)
    {
      visited_[r][c] = false;
    }
  }
}


void FloodAI::Show(int** arr, int size)
{
  for (int r = 0; r < size; r++)
  {
    for (int c = 0; c < size; c++)
    {
      std::cout << arr[r][c];
    }
    std::cout << std::endl;
  }
}


void FloodAI::Report()
{
   std::cout << "AI Suggests:" << std::endl;
   for (Moves::iterator it = ai_inputs.begin(); it != ai_inputs.end(); it++)
   {
     std::cout << kColorNames[*it] << std::endl;
   }
   std::cout << ai_inputs.size() << " steps in total" << std::endl;
}
