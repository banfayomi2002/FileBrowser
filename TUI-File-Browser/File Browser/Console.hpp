/** @file : Console.h
Name : Fayomi Augustine
Purpose:  Header file for the thick wrapper to interact with the thin wrapper.
History : The program is using console API and the sample code given by Garth Santors.
Date : 16/03/2016
**/


#ifndef __CONSOLE_GUARD__
#define __CONSOLE_GUARD__

#include "ConsoleAPI.hpp"
#include "Color.h"

class Console
{
	// -------- CLASS MEMBERS --------
	private:
		ConsoleAPI console_;
		ConsoleAPI::State state_;

	// -------- CONSTRUCTOR/DESTRUCTOR --------
	public:
		Console();
		~Console();

	// -------- OPERATIONS --------
	public:
		
		// Sets the title of the console.
		
		Console& SetTitle(std::string title);
		
		//Sets the size of the console window.
		
		Console& SetSize(WORD const width, WORD const height);
		
		 //Sets the cursor position within the console window and its visibility.
		
		Console& SetCursor(SHORT const x, SHORT const y, bool visible);
	
		 // Used to pass in the input flags for keyboard and mouse down to the API to
		 // allow the console to accept keyboard and mouse events.
		 
		Console& EnableKeyboardAndMouse();
		
		// Enables a handler to look for Ctrl key events.
		
		Console& EnableCtrlHandler(PHANDLER_ROUTINE routine);
		
		 //Clears the background screen to blank, and then paints with the specified colour.
		
		Console& SetBackgroundColour(BackgroundColour background);
		
		// Fills a portion of the console at the specified location, across the length of the console.
		
		Console& Fill(WORD const startLoc, WORD const layoutSize, ForegroundColour foreground, BackgroundColour background);
		
		// Draws a control at x, y coord for a specified length.
		
		Console& Draw(WORD const x, WORD const y, WORD const controlLength, ForegroundColour foreground, BackgroundColour background);
	
		// Used to write text or content to the console at a specific coordinate.
		
		Console& Write(WORD const x, WORD const y, std::string content, ForegroundColour foreground, BackgroundColour background);
	
		// Clears the screen and sets a background color. Similar to SetBackgroundColour.
		
		Console& Clear(BackgroundColour background);
	
		// Returns an event from the console's read in input.
		

		Console& GetEvent(std::vector<INPUT_RECORD>& buffer, DWORD& num);

		
		//Gets the current working directory that the executable of this program is located in.
		
		
		std::string GetCurrentDir();
};
#endif