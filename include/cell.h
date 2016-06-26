#ifndef CELL_H
#define CELL_H


#include <iostream>


class Game;

const int MAXNEIGHBOURS = 4;
extern const int kBackgroundColor;


class Cell
{
public:
	Cell();
	~Cell();

	/* Accessors */
	int GetState() const ;
	
	/* Setters */
	void SetState(const int& change);
	void setCoords(const int row, const int column);
	void SetGame(Game * game);

	void AddNeighbour(Cell *neighbour);
	void Notify( const int & change);
	void Notify( const int & current, const int & previous); 

private:
  void Notify() const;
  
  int row_;
  int column_;
	int state_;
	int prev_state_;
	int numNeighbours_;
	
	Cell *neighbours_[MAXNEIGHBOURS];
	Game * game_;
};


#endif
