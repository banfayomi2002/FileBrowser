/**
	@file: Console.hpp
	@author: Aaron Gray
	@author_email: a_gray5@fanshaweonline.ca
	@date: 2015-04-11
	@brief: Header file for the thick wrapper to interact with the console wrapper.
**/

#ifndef __CONSOLE_GUARD__
#define __CONSOLE_GUARD__

#include "ConsoleAPI.hpp"

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
		/**
		* Sets the title of the console.
		*
		* @param title - A string containing the title for the console.
		*/
		Console& SetTitle(std::string title);
		/**
		* Sets the size of the console window.
		*
		* @param width - The width to set the console to.
		* @param height - The height to set the console to.
		*/
		Console& SetSize(WORD const width, WORD const height);
		/**
		* Sets the cursor position within the console window and its visibility.
		*
		* @param x - The x coordinate of where the cursor will be placed on the x-axis.
		* @param y - The y coordinate of where the cursor will be placed on the y-axis.
		* @param visible - Whether or not the cursor will be displayed at the location passed in.
		*/
		Console& SetCursor(SHORT const x, SHORT const y, bool visible);
		/**
		 * Used to pass in the input flags for keyboard and mouse down to the API to
		 * allow the console to accept keyboard and mouse events.
		 */
		Console& EnableKeyboardAndMouse();
		/**
		* Enables a handler to look for Ctrl key events.
		*
		* @param routine - A pointer to the handler routine for the event.
		*/
		Console& EnableCtrlHandler(PHANDLER_ROUTINE routine);
		/**
		* Clears the background screen to blank, and then paints with the specified colour.
		*
		* @param background - The colour of to paint the cleared console screen.
		*/
		Console& SetBackgroundColour(ConsoleAPI::BackgroundColour background);
		/**
		* Fills a portion of the console at the specified location, across the length of the console.
		*
		* @param startLoc - the y coordinate to start filling at
		* @param layoutSize - the height at which the fill we take place up to
		* @param foreground - the colour of any text within the filled area.
		* @param background - the colour of the fill.
		*/
		Console& Fill(WORD const startLoc, WORD const layoutSize, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background);
		/**
		* Draws a control at x, y coord for a specified length.
		*
		* @param x - The x coordinate to start the drawing at.
		* @param y - The y coordinate to start the drawing at.
		* @param controlLength - The length of the control to draw.
		* @param foreground - The colour of any content within the drawn area.
		* @param background - The colour of the control.
		*/
		Console& Draw(WORD const x, WORD const y, WORD const controlLength, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background);
		/**
		* Used to write text or content to the console at a specific coordinate.
		*
		* @param x - The x coordinate to start writing content at.
		* @param y - The y coordinate to start writing content at.
		* @param content - The content/text to write at specified location.
		* @param foreground - colour of the text to write.
		* @param background - colour of the background that the content will be written overtop.
		*/
		Console& Write(WORD const x, WORD const y, std::string content, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background);
		/**
		* Clears the screen and sets a background color. Similar to SetBackgroundColour.
		*
		* @param background - the colour of the console background.
		*/
		Console& Clear(ConsoleAPI::BackgroundColour background);
		/**
		* Returns an event from the console's read in input.
		*
		* @note - this can be used to derive mouse or keyboard events.
		*/
		ConsoleAPI::Event GetEvent();
		/**
		* Gets the current working directory that the executable of this program is located in.
		*
		* @note - This will be used as the default "root-folder" search for the program.
		*/
		std::string GetCurrentDir();
};
#endif