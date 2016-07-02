//
//  FloodAI.h
//  ColourFlood
//
//  Created by Tim on 2016-07-01.
//  Copyright © 2016 Tim. All rights reserved.
//

#ifndef FloodAI_h
#define FloodAI_h

// CPP
#include <stack>
#include <vector>

// LOCAL
#include "graphicdisplay.h"

class Controller;

extern const int COLOURTYPES;

typedef std::vector<int> Moves;

class FloodAI
{
public:
  FloodAI(int** arr, int size, int moves, Controller* ctl = NULL);
  ~FloodAI();
  
  void Init();
  void Report();
  int GetMove();
  int PeekMove();
  int Recommend(int** arr, int size);
  void SetController(Controller*);
  
private:
  void ResetVisited();
  void Show(int** arr, int size);
  inline int** CopyArray(int** arr, int size);
  inline bool IsComplete(int** arr, int size);
  Moves FindMinMoves(int** arr, int size, int moves_allowed);
  int Count(int** grids, int size);
  int Count(int** grids, int size, int row, int col, int colour);
  void Floodit(int** grids, int size, int new_state);
  void Floodit(int** grids, int size, int row, int col, int prev_state, int new_state);
  
  Controller* controller_;
  Moves ai_inputs_;
  int grid_size_;
  int ** grids_;
  bool **visited_;
  int moves_left_;
};



#endif /* FloodAI_h */
