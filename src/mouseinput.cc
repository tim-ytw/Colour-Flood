
// LOCAL
#include "game_param.h"
#include "mouseinput.h"
#include "controller.h"

// CPP
#include <iostream>

using namespace std;


MouseInput::MouseInput(Controller* controller)
	:controller(controller) {}


int MouseInput::getMove()
{
	int x = 0, y = 0;
	bool quit = false;
	
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			quit = (e.type == SDL_QUIT);
			if (e.type == SDL_MOUSEBUTTONDOWN &&
          SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT) &&
          VerifyClick(x, y))
      {
        int row = y / GRIDHEIGHT;
        int col = x / GRIDWIDTH;
        return controller->GetColor(row, col); 
      }
		}
	}
	cerr << "User just clicked quit" << endl;
	controller->SetQuit(); 
	return -1;
}



inline bool MouseInput::VerifyClick(int x, int y)
{
  return (x > 0 && y > 0 &&
          x < GAME_PANEL_WIDTH && y < GAME_PANEL_HEIGHT);
}


