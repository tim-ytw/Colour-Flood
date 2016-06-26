#ifndef DISPLAY_H
#define DISPLAY_H

// CPP
#include <iostream>
#include <sstream>

class View
{
public:
	View(int n);
	virtual ~View() = 0;
	
	virtual void Render() = 0;
	virtual int GetColor(int row, int col) = 0;
	virtual void UpdateMessage(const std::string& message) = 0;
	virtual void Notify(int row, int column, int ch) = 0;
	
protected:
	const int grid_size_;
};

#endif
