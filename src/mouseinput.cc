// LOCAL
#include "game_param.h"
#include "mouseinput.h"
#include "controller.h"

// CPP
#include <iostream>

using namespace std;


MouseInput::MouseInput(Controller* controller)
	:controller(controller) {}


int MouseInput::GetMove()
{
	int x = 0, y = 0;
	
	SDL_Event e;
  while (SDL_PollEvent(&e) != 0)
  {
    if (e.type == SDL_QUIT)
    {
      std::cout << "Quit button clicked" << std::endl;
      controller->SetQuit();
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN &&
        SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT) &&
        VerifyClick(x, y))
    {
      int row = y / GRIDHEIGHT;
      int col = x / GRIDWIDTH;
      controller->AddInput(controller->GetColor(row, col)); 
    }
  }
	return -1;
}



inline bool MouseInput::VerifyClick(int x, int y)
{
  return (x > 0 && y > 0 &&
          x < GAME_PANEL_WIDTH && y < GAME_PANEL_HEIGHT);
}


void MouseInput::Flush()
{
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0)
  {
    if (e.type == SDL_QUIT)
    {
      std::cout << "Quit button clicked" << std::endl;
      controller->SetQuit();
    }
  }
  // SDL_FlushEvents()
}

