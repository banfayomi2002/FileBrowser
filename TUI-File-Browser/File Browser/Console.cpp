/** @file MyApp.cpp
Name : Fayomi Augustine
Purpose : Implementation file for the "thick" console wrapper.
History : The program is using console API and the sample code given by Garth Santors.
Date : 16/03/2016
**/

#include "Console.hpp"
#include "Event.h"

// -------- CONSTRUCTOR/DESTRUCTOR --------
Console::Console() {
	// Get and hold the original state of the console prior to making calls to the API.
	state_ = console_.GetState();
}
Console::~Console() {
	// Set the state back to the original one.
	console_.SetState(state_);
}

// -------- OPERATIONS --------

//Sets the title of the console by calling the ConsoleAPI wrapper function.

Console& Console::SetTitle(std::string title) {
	console_.SetTitle(title);
	return *this;
}

// Sets the size of the console window by calling the ConsoleAPI wrapper function.

Console& Console::SetSize(WORD const width, WORD const height) {
	console_.SetSize(width, height);
	return *this;
}

// Sets the cursor position within the console window and its visibility by 
// calling SetCursorPosition and SetCursorVisibility functions from ConsoleAPI wrapper.


Console& Console::SetCursor(SHORT const x, SHORT const y, bool visible) {
	console_.SetCursorPosition(x, y);
	console_.SetCursorVisibility(visible);
	return *this;
}

// Passes in the ENABLE_PROCESSED_INPUT (Ctrl+C event), ENABLE_MOUSE_INPUT and ENABLE_WINDOW_INPUT

Console& Console::EnableKeyboardAndMouse() {
	console_.SetConsoleInput(ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);
	return *this;
}

// Enables a handler to look for Ctrl key events using the ConsoleAPI wrapper function.

Console& Console::EnableCtrlHandler(PHANDLER_ROUTINE routine) {
	console_.SetCtrlHandler(routine);
	return *this;
}

// Clears the background screen to blank, and then paints with the specified colour using
// the ConsoleAPI wrapper function.

Console& Console::SetBackgroundColour(BackgroundColour background) {
	console_.SetBackgroundColour(background);
	return *this;
}

// Fills a portion of the console at the specified location, across the length of the console using
// the ConsoleAPI wrapper function.

Console& Console::Fill(WORD const startLoc, WORD const layoutSize, ForegroundColour foreground, BackgroundColour background) {
	console_.Fill(startLoc, layoutSize, foreground, background);
	return *this;
}

// Draws a control at x, y coord for a specified length by calling the ConsoleAPI wrapper
// function and passing in these arguments.

Console& Console::Draw(WORD const x, WORD const y, WORD const controlLength, ForegroundColour foreground, BackgroundColour background) {
	console_.Draw(x, y, controlLength, foreground, background);
	return *this;
}

// Used to write text or content to the console at a specific coordinate by calling the ConsoleAPI wrapper
// function and passing in these arguments.

Console& Console::Write(WORD const x, WORD const y, std::string content, ForegroundColour foreground, BackgroundColour background) {
	console_.Write(x, y, content, foreground, background);
	return *this;
}

// Clears the screen and sets a background color by calling the ConsoleAPI wrapper function.

Console& Console::Clear(BackgroundColour background) {
	console_.Clear(background);
	return *this;
}

// Returns an event from the console's read in input by calling the ConsoleAPI wrapper function.



Console& Console::GetEvent(std::vector<INPUT_RECORD>& buffer, DWORD& num)
  {
	  buffer = std::vector<INPUT_RECORD>(128);
	
	  console_.ThinReadConsoleInput(buffer.data(), buffer.size(), &num);
	 
	  return *this;
  }


// Gets the current working directory that the executable of this program is located in 
// by calling the ConsoleAPI wrapper function.

std::string Console::GetCurrentDir() {
	std::string dir = console_.GetCurrentDir();
	return dir;
}