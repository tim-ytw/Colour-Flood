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


extern const int COLOURTYPES;

typedef std::vector<int> Moves;

class FloodAI
{
public:
  FloodAI(int** arr, int size, int moves);
  ~FloodAI();
  
  int GetMove();
  void Init();
  void Report();
  
private:
  void ResetVisited();
  void Show(int** arr, int size);
  inline int** CopyArray(int** arr, int size);
  inline bool IsComplete(int** arr, int size);
  Moves FindMinMoves(int** arr, int size, int moves_allowed);
  int Count(int** grids, int size, int row, int col, int colour);
  void Floodit(int** grids, int size, int row, int col, int prev_state, int new_state);
  
  Moves ai_inputs;
  int* colours_;
  int grid_size_;
  int ** grids_;
  bool **visited_;
  int moves_left_;
};



#endif /* FloodAI_h */
