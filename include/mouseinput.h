#ifndef MOUSE_INPUT_H
#define MOUSE_INPUT_H


// SDL2
#include "SDL2/SDL.h"

class Controller;

class MouseInput
{
public:
	MouseInput(Controller* controller = NULL);
  
	int getMove();
	
private:
  inline bool VerifyClick(int x, int y);
  
  Controller* controller;
};


#endif
