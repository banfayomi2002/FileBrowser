/**
	@file: ConsoleAPI.cpp
	@author: Aaron Gray
	@author_email: a_gray5@fanshaweonline.ca
	@date: 2015-04-11
	@brief: Implementation file for the thin console API wrapper.
**/

#include "ConsoleAPI.hpp"

#include <memory>
#include <stdio.h>
#include <direct.h>
#include <sstream>

// -------- CONSTRUCTOR --------
ConsoleAPI::ConsoleAPI() {
	// Set up the handles to the console.
	hStdIn_ = GetStdHandle(STD_INPUT_HANDLE);
	hStdOut_ = GetStdHandle(STD_OUTPUT_HANDLE);
}

// -------- OPERATIONS --------
/**
* Constructs a state object and proceeds to get the current console's window and buffer size,
* the desktop info, the cursor info, mode and title and store them within the state object's
* appropriately named variables. The state object is then returned.
*
* @note Used in the thick constructor to set the state variable to the
*		 console state prior to making changes.
*/
ConsoleAPI::State ConsoleAPI::GetState() const {
	// Pull in data from current console state and store in state object.
	ConsoleAPI::State state;

	try
	{
		// Get window and buffer size.
		THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(hStdOut_, &state.csbi_));

		// Get console desktop.
		state.bufferCoord_ = COORD{ 0 };
		state.buffer_ = std::vector<CHAR_INFO>(state.csbi_.dwSize.X * state.csbi_.dwSize.Y);
		THROW_IF_CONSOLE_ERROR(ReadConsoleOutput(hStdOut_, state.buffer_.data(), state.csbi_.dwSize, state.bufferCoord_, &state.csbi_.srWindow));

		// Get console cursor info.
		THROW_IF_CONSOLE_ERROR(GetConsoleCursorInfo(hStdOut_, &state.ccl_));

		// Get console mode.
		THROW_IF_CONSOLE_ERROR(GetConsoleMode(hStdIn_, &state.mode_));

		// Get console title.
		DWORD length = GetConsoleTitleA((LPSTR)state.title_.c_str(), state.title_.size());
		THROW_IF_CONSOLE_ERROR(length == 0); // Return value of title is length of title, if fails length is zero and GetLastError() contains error code.
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return state;
}
/**
* Uses the passed in state object's variables that have been assigned at construction
* of the Console instance to restore window, console buffer, desktop data, cursor position
* and info, console mode and console title.
*
* @param state - State object that will have values pulled from it and
*        used to reset the console to the passed in state.
*/
ConsoleAPI& ConsoleAPI::SetState(State const& state) {
	try
	{
		// Set window, console buffer, and desktop data.
		SMALL_RECT sr{ 0 };

		THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hStdOut_, TRUE, &sr));
		THROW_IF_CONSOLE_ERROR(SetConsoleScreenBufferSize(hStdOut_, state.csbi_.dwSize));
		THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hStdOut_, TRUE, &state.csbi_.srWindow));

		sr = state.csbi_.srWindow;
		THROW_IF_CONSOLE_ERROR(WriteConsoleOutput(hStdOut_, state.buffer_.data(), state.csbi_.dwSize, state.bufferCoord_, &sr));

		// Set cursor position and info.
		THROW_IF_CONSOLE_ERROR(SetConsoleCursorInfo(hStdOut_, &state.ccl_));
		THROW_IF_CONSOLE_ERROR(SetConsoleCursorPosition(hStdOut_, state.csbi_.dwCursorPosition));

		// Set console mode.
		THROW_IF_CONSOLE_ERROR(SetConsoleMode(hStdIn_, state.mode_));

		// Set console title.
		THROW_IF_CONSOLE_ERROR(SetConsoleTitleA(state.title_.c_str()));

		// Close the console handles.
		CloseConsoleHandle(hStdIn_);
		CloseConsoleHandle(hStdOut_);
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}
/**
* Uses the OS API call SetConsoleTitleA to set the content of "title" to an 
* ASCII version of content.
*
* @param title - A string containing the title for the console.
*/
ConsoleAPI& ConsoleAPI::SetTitle(std::string title) {
	try
	{
		THROW_IF_CONSOLE_ERROR(SetConsoleTitleA(title.c_str()));
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}
/**
* Sets the size of the console widow info to a zero value, then sets the buffer.
* This ensures the window will always be one less width/height value then the buffer.
*
* @param width - The width to set the console to.
* @param height - The height to set the console to.
*/
ConsoleAPI& ConsoleAPI::SetSize(WORD const width, WORD const height) {
	try
	{
		SMALL_RECT sr{ 0 };
		COORD size;
		size.X = width;
		size.Y = height;

		// Set initial window size to zero.
		THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hStdOut_, TRUE, &sr));

		// Set buffer size.
		THROW_IF_CONSOLE_ERROR(SetConsoleScreenBufferSize(hStdOut_, size));

		// Reset window size to actual size we want.
		sr.Right = width - 1;
		sr.Bottom = height - 1;

		THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hStdOut_, TRUE, &sr));
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}
/**
* Calls the API SetConsoleCursorPosition and constructs a COORD object using the passed in x,y values.
*
* @param x - The x coordinate of where the cursor will be placed on the x-axis.
* @param y - The y coordinate of where the cursor will be placed on the y-axis.
*/
ConsoleAPI& ConsoleAPI::SetCursorPosition(SHORT const x, SHORT const y) {
	try
	{
		// Set the console cursor position.
		COORD pos;
		pos.X = x;
		pos.Y = y;

		THROW_IF_CONSOLE_ERROR(SetConsoleCursorPosition(hStdOut_, pos));
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}
/**
* Calls API SetConsoleCursorInfo and constructs a cursor info variable and sets its
* visibility to the passed in value.
*
* @param visibility - The flag for whether to set the cursor to be visible.
*/
ConsoleAPI& ConsoleAPI::SetCursorVisibility(bool visibility) {
	try
	{
		// Set the cursor visibility.
		CONSOLE_CURSOR_INFO ccl;
		ccl.bVisible = visibility;
		ccl.dwSize = 1;

		THROW_IF_CONSOLE_ERROR(SetConsoleCursorInfo(hStdOut_, &ccl));
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}
/**
* Calls API call for SetConsoleMode and passes in the input handle and mode variable.
*
* @param mode - The singular input type or bitwise OR'd modes.
*/
ConsoleAPI& ConsoleAPI::SetConsoleInput(DWORD mode) {
	try
	{
		// Set the console mode.
		THROW_IF_CONSOLE_ERROR(SetConsoleMode(hStdIn_, mode));
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}
/**
* Calls API to SetConsoleCtrlHandler to the passed in routine.
*
* @param routine - A pointer to the handler routine for the event.
*/
ConsoleAPI& ConsoleAPI::SetCtrlHandler(PHANDLER_ROUTINE routine) {
	try
	{
		THROW_IF_CONSOLE_ERROR(SetConsoleCtrlHandler(routine, TRUE));
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}
/**
* Gets the console screen buffer info and sets the entire buffer to blank characters in order
* to "clear" it and then sets these blank characters to have the background colour specified.
*
* @param background - The colour of to paint the cleared console screen.
*/
ConsoleAPI& ConsoleAPI::SetBackgroundColour(ConsoleAPI::BackgroundColour background) {
	try
	{
		// Get console screen buffer info.
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(hStdOut_, &csbi));

		// Fill the entire screen area according to background colour.
		DWORD nCharsWritten;
		DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
		COORD pos{ 0, 0 };

		THROW_IF_CONSOLE_ERROR(FillConsoleOutputCharacterA(hStdOut_, ' ', size, pos, &nCharsWritten));
		THROW_IF_CONSOLE_ERROR(FillConsoleOutputAttribute(hStdOut_, (WORD)background, size, pos, &nCharsWritten));
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}
/**
* Works similarily to SetBackgroundColour, but the size of the fill is determined by layoutSize and it occurs at the y value passed in.
* It will fill blanks at the info using the background and foreground specified.
*
* @param startLoc - the y coordinate to start filling at
* @param layoutSize - the height at which the fill we take place up to
* @param foreground - the colour of any text within the filled area.
* @param background - the colour of the fill.
*/
ConsoleAPI& ConsoleAPI::Fill(WORD const startLoc, WORD const layoutSize, ForegroundColour foreground, BackgroundColour background) {
	try
	{
		// Get the console screen buffer info.
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(hStdOut_, &csbi));

		// Set the layout to fill to the size.
		DWORD size = csbi.dwSize.X * layoutSize;
		DWORD nCharsWritten;
		COORD pos{ 0, startLoc };

		THROW_IF_CONSOLE_ERROR(FillConsoleOutputCharacterA(hStdOut_, ' ', size, pos, &nCharsWritten));
		THROW_IF_CONSOLE_ERROR(FillConsoleOutputAttribute(hStdOut_, (WORD)foreground | (WORD)background, size, pos, &nCharsWritten));
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}
/**
* Gets the console screen buffer and constructs a COORD based off passed in x, y and uses
* FillConsoleOutputAttribute to draw a control at that location for controlLength width for the
* two passed in colours.
*
* @param x - The x coordinate to start the drawing at.
* @param y - The y coordinate to start the drawing at.
* @param controlLength - The length of the control to draw.
* @param foreground - The colour of any content within the drawn area.
* @param background - The colour of the control.
*/
ConsoleAPI& ConsoleAPI::Draw(WORD const x, WORD const y, WORD const controlLength, ForegroundColour foreground, BackgroundColour background) {
	try
	{
		// Get the console screen buffer info.
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(hStdOut_, &csbi));

		// Set the control to fill at coords and the length needed.
		DWORD length = controlLength;
		DWORD nCharsWritten;
		COORD pos{ x, y };

		THROW_IF_CONSOLE_ERROR(FillConsoleOutputAttribute(hStdOut_, (WORD)foreground | (WORD)background, length, pos, &nCharsWritten));
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}
/**
* Constructs a COORD for the location of the text and determines what the minimum text it will need to write. It uses
* WriteConsoleOutputCharacterA to output ASCII content and the colours are written using WriteConsoleOutputAttribute.
*
* @param x - The x coordinate to start writing content at.
* @param y - The y coordinate to start writing content at.
* @param content - The content/text to write at specified location.
* @param foreground - colour of the text to write.
* @param background - colour of the background that the content will be written overtop.
*/
ConsoleAPI& ConsoleAPI::Write(WORD const x, WORD const y, std::string content, ForegroundColour foreground, BackgroundColour background) {
	try
	{
		// Get current location, length and number of characters to write and write the data plus any color/attributes.
		COORD loc{ x, y };
		DWORD nCharsWritten;
		DWORD nToWrite = min(content.size(), unsigned(0 - x));
		WORD attributes = (WORD)foreground | (WORD)background;

		std::vector<WORD> attr = std::vector<WORD>(content.size(), attributes);

		THROW_IF_CONSOLE_ERROR(WriteConsoleOutputCharacterA(hStdOut_, content.c_str(), nToWrite, loc, &nCharsWritten));
		THROW_IF_CONSOLE_ERROR(WriteConsoleOutputAttribute(hStdOut_, attr.data(), nToWrite, loc, &nCharsWritten));
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}
/**
* Works the very same as SetBackgroundColour but has the added bonus of setting the cursor position to the top left.
*
* @param background - the colour of the console background.
*/
ConsoleAPI& ConsoleAPI::Clear(ConsoleAPI::BackgroundColour background) {
	try
	{
		// Get the console screen buffer info.
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(hStdOut_, &csbi));

		// Fill buffer with blanks to clear.
		DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
		DWORD nCharsWritten;
		COORD pos{ 0, 0 };

		THROW_IF_CONSOLE_ERROR(FillConsoleOutputCharacter(hStdOut_, ' ', size, pos, &nCharsWritten));
		THROW_IF_CONSOLE_ERROR(FillConsoleOutputAttribute(hStdOut_, (WORD)background, size, pos, &nCharsWritten));
		THROW_IF_CONSOLE_ERROR(SetConsoleCursorPosition(hStdOut_, pos));
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}

/**
* Constructs a vector of event records and reads the most recent input according to allowed modes in the console.
* It will then return the first element of the vector's input records.
*
* @note - this can be used to derive mouse or keyboard events.
*/
ConsoleAPI::Event ConsoleAPI::GetEvent() {
	std::vector<INPUT_RECORD> events(128);
	DWORD nEvents;

	THROW_IF_CONSOLE_ERROR(ReadConsoleInput(hStdIn_, events.data(), events.size(), &nEvents));
	ConsoleAPI::Event e = ConsoleAPI::Event(events[0]);

	return e;
}
/**
* Constructs a char array set to the max size of filename. Using the _getcwd (Get CurrentWorkingDirectory) function,
* it passes the executable's root folder directory into the array and then is used in the construction a string to be 
* returned.
*
* @note - This will be used as the default "root-folder" search for the program.
*/
std::string ConsoleAPI::GetCurrentDir() {
	std::string currentDir = "";
	
	try
	{
		// Get the current path using _getcwd.
		char currentPath[FILENAME_MAX];
		_getcwd(currentPath, sizeof(currentPath));
		currentDir = std::string(currentPath);
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return currentDir;
}
/**
* Closes the passed in handle by calling the CloseHandle function.
*
* @note - Used when setting the state back to original in the destructor.
*/
ConsoleAPI& ConsoleAPI::CloseConsoleHandle(HANDLE handle) {
	try
	{
		THROW_IF_CONSOLE_ERROR(CloseHandle(handle));
	}
	catch (ConsoleAPI::XError& e)
	{
		MessageBoxA(NULL, e.GetFormattedMessage().c_str(), "Runtime Error", MB_OK);
	}

	return *this;
}

// -------- XERROR OPERATION --------
/**
* Constructs a LPSTR buffer and sets the size/msg using the FormatMessageA function.
* Constructs the error message using the XError exception's GetFile and GetLine to return 
* the error code, name, and the line it occurred on.
*
* @note XErrors are all caught/called in API code.
*/
std::string ConsoleAPI::XError::GetFormattedMessage() const {
	// Format the message using windows functions.
	LPSTR buffer = nullptr;
	std::size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, 0, NULL);
	std::string msg(buffer, size);

	// Free buffer.
	LocalFree(buffer);

	// Construct end error message.
	std::string errmsg = this->GetFile();
	std::size_t fnd = errmsg.find_last_of(':');
	errmsg = errmsg.substr(0, fnd);

	std::string line;
	std::stringstream ss;
	ss << this->GetLine();
	ss >> line;

	std::string e = "ERROR: " + msg + " occured on line " + line + "\n";
	return e;
}