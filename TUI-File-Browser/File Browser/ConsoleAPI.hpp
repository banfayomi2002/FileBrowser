/**
 @file : ConsoleAPI.h
    Name : Fayomi Augustine
	Purpose: Header file for the thin console API wrapper.
	Date :  16/03/2016
	History : The program is using console API and the sample code given by Garth Santors.
	Version : 2
	Modifications : Added more methods for TUI Brwoser implementation
**/


#ifndef __CONSOLE_API_GUARD__
#define __CONSOLE_API_GUARD__

#include <Windows.h>
#include <string>
#include <vector>
#include "Color.h"

class ConsoleAPI
{
	// -------- CLASS MEMBERS --------
	private:
		
		HANDLE hStdOut_;
		HANDLE hStdIn_;
	// -------- CONSTRUCTOR --------
	public:
		ConsoleAPI();
		
	//// -------- DEPENDENCY CLASSES --------
	
		class State
		{
			// -------- FRIEND STATUS --------
			friend ConsoleAPI;

			// -------- CLASS MEMBERS --------
			private:
				CONSOLE_SCREEN_BUFFER_INFO	csbi_;
				CONSOLE_CURSOR_INFO			ccl_;
				std::vector<CHAR_INFO>		buffer_;
				COORD						bufferCoord_;
				DWORD						mode_;
				std::string					title_;
			};
		class XError
		{
			// -------- CLASS MEMBERS --------
			private:
				unsigned long	line_;
				char const*		file_;

			// -------- CONSTRUCTOR --------
			public:
				XError(char const* file, unsigned long line) : file_(file), line_(line) { };

			// -------- ACCESSORS --------
			public:
				unsigned long GetLine() const { return line_; }
				char const* GetFile() const { return file_; }

				
				 // Gets a string describing the message of the exception.
				 
				std::string GetFormattedMessage() const;
		};
	

	// -------- OPERATIONS --------
	public:
	
		 // Gets the state of the console.
		
		State GetState() const;
		
		 // Sets the state of the console back to the contents of the state.
		
		ConsoleAPI& SetState(State const& state);

		
		 // Sets the title of the console.
		 
		
		ConsoleAPI& SetTitle(std::string title);
		
		 // Sets the size of the console window.
		
		ConsoleAPI& SetSize(WORD const width, WORD const height);
		
		 // Sets the cursor position within the console window.
		
		ConsoleAPI& SetCursorPosition(SHORT const x, SHORT const y);
		
		 // Determines whether the cursor will be visible at its current location.
		
		ConsoleAPI& SetCursorVisibility(bool visibility);
	
		 //Sets the consoles mode which allows for different input types.
		 
		
		ConsoleAPI& SetConsoleInput(DWORD mode);
		
		 // Enables a handler to look for Ctrl key events.
		
		ConsoleAPI& SetCtrlHandler(PHANDLER_ROUTINE routine);
	
		 // Clears the background screen to blank, and then paints with the specified colour.
		
		ConsoleAPI& SetBackgroundColour(BackgroundColour background);

	
		 // Fills a portion of the console at the specified location, across the length of the console.
		
		ConsoleAPI& Fill(WORD const startLoc, WORD const layoutSize, ForegroundColour foreground, BackgroundColour background);
	
		 // Draws a control at x, y coord for a specified length.
		
		ConsoleAPI& Draw(WORD const x, WORD const y, WORD const controlLength, ForegroundColour foreground, BackgroundColour background);
		
		 // Used to write text or content to the console at a specific coordinate.
		
		ConsoleAPI& Write(WORD const x, WORD const y, std::string content, ForegroundColour foreground, BackgroundColour background);
	
		 // Clears the screen and sets a background color. Similar to SetBackgroundColour.
		
		ConsoleAPI& Clear(BackgroundColour background);

		
		 // Returns an event from the console's read in input.
	
		std::string GetCurrentDir();

		void ThinReadConsoleInput(PINPUT_RECORD lpBuffer, unsigned long nLength, LPDWORD lpNumberOfEventsRead);


	private:
		
		 // Closes the standard input and output handle hooks into the console.
		
		ConsoleAPI& CloseConsoleHandle(HANDLE handle);
};

// -------- HELPER MACROS --------
#define THROW_IF_CONSOLE_ERROR(res) if(!res) throw XError(__FILE__, __LINE__)
#define THROW_CONSOLE_ERROR() throw XError(__FILE__, __LINE__)

#endif