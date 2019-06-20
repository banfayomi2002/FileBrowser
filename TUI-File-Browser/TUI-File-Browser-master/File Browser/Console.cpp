/**
	@file: Console.cpp
	@author: Aaron Gray
	@author_email: a_gray5@fanshaweonline.ca
	@date: 2015-04-11
	@brief: Implementation file for the "thick" console wrapper.
**/

#include "Console.hpp"

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
/**
* Sets the title of the console by calling the ConsoleAPI wrapper function.
*
* @param title - A string containing the title for the console.
*/
Console& Console::SetTitle(std::string title) {
	console_.SetTitle(title);
	return *this;
}
/**
* Sets the size of the console window by calling the ConsoleAPI wrapper function.
*
* @param width - The width to set the console to.
* @param height - The height to set the console to.
*/
Console& Console::SetSize(WORD const width, WORD const height) {
	console_.SetSize(width, height);
	return *this;
}
/**
* Sets the cursor position within the console window and its visibility by 
* calling a combination of the SetCursorPosition and SetCursorVisibility functions
* within the ConsoleAPI wrapper.
*
* @param x - The x coordinate of where the cursor will be placed on the x-axis.
* @param y - The y coordinate of where the cursor will be placed on the y-axis.
* @param visible - Whether or not the cursor will be displayed at the location passed in.
*/
Console& Console::SetCursor(SHORT const x, SHORT const y, bool visible) {
	console_.SetCursorPosition(x, y);
	console_.SetCursorVisibility(visible);
	return *this;
}
/**
* Passes in the ENABLE_PROCESSED_INPUT (Ctrl+C event), ENABLE_MOUSE_INPUT and ENABLE_WINDOW_INPUT
* input mode flags bitwise OR'd as the mode parameter in the ConsoleAPI wrapper SetConsoleInput method.
*/
Console& Console::EnableKeyboardAndMouse() {
	console_.SetConsoleInput(ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);
	return *this;
}
/**
* Enables a handler to look for Ctrl key events using the ConsoleAPI wrapper function.
*
* @param routine - A pointer to the handler routine for the event.
*/
Console& Console::EnableCtrlHandler(PHANDLER_ROUTINE routine) {
	console_.SetCtrlHandler(routine);
	return *this;
}
/**
* Clears the background screen to blank, and then paints with the specified colour using
* the ConsoleAPI wrapper function.
*
* @param background - The colour of to paint the cleared console screen.
*/
Console& Console::SetBackgroundColour(ConsoleAPI::BackgroundColour background) {
	console_.SetBackgroundColour(background);
	return *this;
}
/**
* Fills a portion of the console at the specified location, across the length of the console using
* the ConsoleAPI wrapper function.
*
* @param startLoc - the y coordinate to start filling at
* @param layoutSize - the height at which the fill we take place up to
* @param foreground - the colour of any text within the filled area.
* @param background - the colour of the fill.
*/
Console& Console::Fill(WORD const startLoc, WORD const layoutSize, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background) {
	console_.Fill(startLoc, layoutSize, foreground, background);
	return *this;
}
/**
* Draws a control at x, y coord for a specified length by calling the ConsoleAPI wrapper
* function and passing in these arguments.
*
* @param x - The x coordinate to start the drawing at.
* @param y - The y coordinate to start the drawing at.
* @param controlLength - The length of the control to draw.
* @param foreground - The colour of any content within the drawn area.
* @param background - The colour of the control.
*/
Console& Console::Draw(WORD const x, WORD const y, WORD const controlLength, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background) {
	console_.Draw(x, y, controlLength, foreground, background);
	return *this;
}
/**
* Used to write text or content to the console at a specific coordinate by calling the ConsoleAPI wrapper
* function and passing in these arguments.
*
* @param x - The x coordinate to start writing content at.
* @param y - The y coordinate to start writing content at.
* @param content - The content/text to write at specified location.
* @param foreground - colour of the text to write.
* @param background - colour of the background that the content will be written overtop.
*/
Console& Console::Write(WORD const x, WORD const y, std::string content, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background) {
	console_.Write(x, y, content, foreground, background);
	return *this;
}
/**
* Clears the screen and sets a background color by calling the ConsoleAPI wrapper
* function.
*
* @param background - the colour of the console background.
*/
Console& Console::Clear(ConsoleAPI::BackgroundColour background) {
	console_.Clear(background);
	return *this;
}
/**
* Returns an event from the console's read in input by calling the ConsoleAPI wrapper
* function.
*
* @note - this can be used to derive mouse or keyboard events.
*/
ConsoleAPI::Event Console::GetEvent() {
	ConsoleAPI::Event e = console_.GetEvent();
	return e;
}
/**
* Gets the current working directory that the executable of this program is located in 
* by calling the ConsoleAPI wrapper function.
*
* @note - This will be used as the default "root-folder" search for the program.
*/
std::string Console::GetCurrentDir() {
	std::string dir = console_.GetCurrentDir();
	return dir;
}