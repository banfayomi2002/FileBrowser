/**
	@file: ConsoleAPI.hpp
	@author: Aaron Gray
	@author_email: a_gray5@fanshaweonline.ca
	@date: 2015-04-11
	@brief: Header file for the wrapper to interact directly with the OS API as a 1-to-1 analog.
**/

#ifndef __CONSOLE_API_GUARD__
#define __CONSOLE_API_GUARD__

#include <Windows.h>
#include <string>
#include <vector>

class ConsoleAPI
{
	// -------- CLASS MEMBERS --------
	private:
		
		HANDLE hStdOut_;

	// -------- CONSTRUCTOR --------
	public:
		ConsoleAPI();
		HANDLE hStdIn_;
	// -------- DEPENDENCY CLASSES --------
	public:
		enum class ForegroundColour
		{
			BLACK = 0,
			DARKBLUE = FOREGROUND_BLUE,
			DARKGREEN = FOREGROUND_GREEN,
			DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
			DARKRED = FOREGROUND_RED,
			DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
			DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
			GRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
			DARKGRAY = FOREGROUND_INTENSITY,
			BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
			GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
			CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
			RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
			MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
			YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
			WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		};
		enum class BackgroundColour
		{
			BLACK = 0,
			DARKBLUE = BACKGROUND_BLUE,
			DARKGREEN = BACKGROUND_GREEN,
			DARKCYAN = BACKGROUND_GREEN | BACKGROUND_BLUE,
			DARKRED = BACKGROUND_RED,
			DARKMAGENTA = BACKGROUND_RED | BACKGROUND_BLUE,
			DARKYELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
			GREY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
			DARKGREY = BACKGROUND_INTENSITY,
			BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
			GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
			CYAN = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
			RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
			MAGENTA = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
			YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
			WHITE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
		};

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

				/**
				 * Gets a string describing the message of the exception.
				 *
				 * @note XErrors are all caught/called in API code.
				 */
				std::string GetFormattedMessage() const;
		};
		//class Event
		//{
		//	// -------- CLASS MEMBERS --------
		//	private:
		//		INPUT_RECORD	ir_;

		//	// -------- CONSTRUCTOR --------
		//	public:
		//		Event(INPUT_RECORD const ir) : ir_(ir) { };

		//	// -------- DEPENDENCY CLASSES --------
		//	public:
		//		enum class EventType
		//		{
		//			KEY = KEY_EVENT,
		//			MOUSE = MOUSE_EVENT
		//		};

		//		class Keyboard
		//		{
		//			// -------- FRIEND STATUS --------
		//			friend Event;

		//			// -------- CLASS MEMBERS --------
		//			private:
		//				KEY_EVENT_RECORD const&	ker_;

		//			// -------- CONSTRUCTOR --------
		//			private:
		//				Keyboard(INPUT_RECORD const& ir) : ker_(ir.Event.KeyEvent) { };

		//			// -------- OPERATIONS --------
		//			public:
		//				bool KeyDown() const { return ker_.bKeyDown ? true : false; }
		//				char AsciiChar() const { return ker_.uChar.AsciiChar; }
		//				unsigned short VirtualKeyCode() const { return ker_.wVirtualKeyCode; }
		//		};
		//		class Mouse
		//		{
		//			// -------- FRIEND STATUS --------
		//			friend Event;

		//			// --------- DEPENDENCY CLASS --------
		//			public:
		//				enum class MouseType
		//				{
		//					WHEELED = MOUSE_WHEELED,
		//					BUTTON = 0
		//				};

		//			// -------- CLASS MEMBERS --------
		//			private:
		//				MOUSE_EVENT_RECORD const&	mer_;

		//			// -------- CONSTRUCTOR --------
		//			private:
		//				Mouse(INPUT_RECORD const& ir) : mer_(ir.Event.MouseEvent) { };

		//			// -------- OPERATIONS --------
		//			public:
		//				bool LeftPressed() const { return mer_.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED ? true : false; }
		//				bool MouseWheelDown() const {
		//					short state = HIWORD(mer_.dwButtonState);
		//					return state < 0;
		//				}
		//				bool MouseWheelUp() const {
		//					short state = HIWORD(mer_.dwButtonState);
		//					return state > 0;
		//				}

		//				COORD MousePosition() const { return mer_.dwMousePosition; }

		//				MouseType GetType() const { return MouseType(mer_.dwEventFlags); }
		//		};

		//	// -------- ACCESSORS --------
		//	public:
		//		EventType GetType() const { return EventType(ir_.EventType); }

		//		Keyboard GetKeyboardEvent() const { return Keyboard(ir_); }
		//		Mouse GetMouseEvent() const { return Mouse(ir_); }
		//};

	// -------- OPERATIONS --------
	public:
		/**
		 * Gets the state of the console.
		 *
		 * @note Used in the thick constructor to set the state variable to the
		 *		 console state prior to making changes.
		 */
		State GetState() const;
		/**
		 * Sets the state of the console back to the contents of the state.
		 *
		 * @param state - State object that will have values pulled from it and
	     *        used to reset the console to the passed in state.
		 */
		ConsoleAPI& SetState(State const& state);

		/**
		 * Sets the title of the console.
		 *
		 * @param title - A string containing the title for the console.
		 */
		ConsoleAPI& SetTitle(std::string title);
		/**
		 * Sets the size of the console window.
		 *
		 * @param width - The width to set the console to.
		 * @param height - The height to set the console to.
		 */
		ConsoleAPI& SetSize(WORD const width, WORD const height);
		/**
		 * Sets the cursor position within the console window.
		 *
		 * @param x - The x coordinate of where the cursor will be placed on the x-axis.
		 * @param y - The y coordinate of where the cursor will be placed on the y-axis.
		 */
		ConsoleAPI& SetCursorPosition(SHORT const x, SHORT const y);
		/**
		 * Determines whether the cursor will be visible at its current location.
		 *
		 * @param visibility - The flag for whether to set the cursor to be visible.
		 */
		ConsoleAPI& SetCursorVisibility(bool visibility);
		/**
		 * Sets the consoles mode which allows for different input types.
		 *
		 * @param mode - The singular input type or bitwise OR'd modes.
		 */
		ConsoleAPI& SetConsoleInput(DWORD mode);
		/**
		 * Enables a handler to look for Ctrl key events.
		 *
		 * @param routine - A pointer to the handler routine for the event.
		 */
		ConsoleAPI& SetCtrlHandler(PHANDLER_ROUTINE routine);
		/**
		 * Clears the background screen to blank, and then paints with the specified colour.
		 *
		 * @param background - The colour of to paint the cleared console screen.
		 */
		ConsoleAPI& SetBackgroundColour(BackgroundColour background);

		/**
		 * Fills a portion of the console at the specified location, across the length of the console.
		 *
		 * @param startLoc - the y coordinate to start filling at
		 * @param layoutSize - the height at which the fill we take place up to
		 * @param foreground - the colour of any text within the filled area.
		 * @param background - the colour of the fill.
		 */
		ConsoleAPI& Fill(WORD const startLoc, WORD const layoutSize, ForegroundColour foreground, BackgroundColour background);
		/**
		 * Draws a control at x, y coord for a specified length.
		 *
		 * @param x - The x coordinate to start the drawing at.
		 * @param y - The y coordinate to start the drawing at.
		 * @param controlLength - The length of the control to draw.
		 * @param foreground - The colour of any content within the drawn area.
		 * @param background - The colour of the control.
		 */
		ConsoleAPI& Draw(WORD const x, WORD const y, WORD const controlLength, ForegroundColour foreground, BackgroundColour background);
		/**
		 * Used to write text or content to the console at a specific coordinate.
		 *
		 * @param x - The x coordinate to start writing content at.
		 * @param y - The y coordinate to start writing content at.
		 * @param content - The content/text to write at specified location.
		 * @param foreground - colour of the text to write.
		 * @param background - colour of the background that the content will be written overtop.
		 */
		ConsoleAPI& Write(WORD const x, WORD const y, std::string content, ForegroundColour foreground, BackgroundColour background);
		/**
		 * Clears the screen and sets a background color. Similar to SetBackgroundColour.
		 *
		 * @param background - the colour of the console background.
		 */
		ConsoleAPI& Clear(BackgroundColour background);

		/**
		 * Returns an event from the console's read in input.
		 * 
		 * @note - this can be used to derive mouse or keyboard events.
		 */
		//void GetEvent(std::vector<INPUT_RECORD>& buffer);
		/**
		 * Gets the current working directory that the executable of this program is located in.
		 *
		 * @note - This will be used as the default "root-folder" search for the program.
		 */
		std::string GetCurrentDir();

		void ThinReadConsoleInput(PINPUT_RECORD lpBuffer, unsigned long nLength, LPDWORD lpNumberOfEventsRead);


	private:
		/**
		 * Closes the standard input and output handle hooks into the console.
		 *
		 * @note - Used when setting the state back to original in the destructor.
		 */
		ConsoleAPI& CloseConsoleHandle(HANDLE handle);
};

// -------- HELPER MACROS --------
#define THROW_IF_CONSOLE_ERROR(res) if(!res) throw XError(__FILE__, __LINE__)
#define THROW_CONSOLE_ERROR() throw XError(__FILE__, __LINE__)

#endif