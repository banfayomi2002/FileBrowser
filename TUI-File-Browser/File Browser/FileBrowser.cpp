/** @file : Controller.cpp
Name : Fayomi Augustine
Purpose: Implementation file for FileBrowser application.
History : The program is using console API and the sample code given by Garth Santors.
Date : 16/03/2016
version: 1.0
**/

#include "FileBrowser.hpp"
#include <regex>
#include <sstream>
#include "Color.h"

//application status
bool FileView::done = false;
Framework frame = Framework();


//Gets the file and creates the console interface
FileView::FileView(std::string folder, std::string filter, bool rSearch) {
	// Set up the console.
	frame.SetupConsole();
	frame.EnableCtrlHandler((PHANDLER_ROUTINE)CtrlHandler);
	
	// Create the interface.
	CreateTUI(folder, filter, rSearch);
}

// -------- OPERATIONS --------

// Constructs the layouts, the labels, input controls and the textboxes to display, and retrieve
// user input in order to update the program's status and state.

FileView& FileView::CreateTUI(std::string folder, std::string filter, bool rSearch) {
	// Create the layout of the console.
	frame.AddLayoutToConsole(Framework::Layout("titleBar", 0, 5, ForegroundColour::WHITE, BackgroundColour::GREY));
	frame.AddLayoutToConsole(Framework::Layout("ribbonBar", 5, 7, ForegroundColour::WHITE, BackgroundColour::GREY));
	frame.AddLayoutToConsole(Framework::Layout("footerBar", 43, 7, ForegroundColour::WHITE, BackgroundColour::GREY));

	// Create labels on the console.
	frame.AddTextToConsole(Framework::Control::Label("titleLabel", COORD{ 60, 2 }, "TUI FILE BROWSER", ForegroundColour::WHITE, BackgroundColour::GREY));
	frame.AddTextToConsole(Framework::Control::Label("folderLabel", COORD{ 1, 6 }, "FOLDER:", ForegroundColour::WHITE, BackgroundColour::GREY));
	frame.AddTextToConsole(Framework::Control::Label("filterLabel", COORD{ 1, 8 }, "FILTER:", ForegroundColour::WHITE, BackgroundColour::GREY));
	frame.AddTextToConsole(Framework::Control::Label("recursiveLabel", COORD{ 1, 10 }, "RECURSIVE SEARCH?", ForegroundColour::WHITE, BackgroundColour::GREY));
	frame.AddTextToConsole(Framework::Control::Label("searchedLabel", COORD{ 1, 44 }, "TOTAL SEARCHED:", ForegroundColour::WHITE, BackgroundColour::GREY));
	frame.AddTextToConsole(Framework::Control::Label("matchingLabel", COORD{ 1, 46 }, "TOTAL MATCHED:", ForegroundColour::WHITE, BackgroundColour::GREY));
	frame.AddTextToConsole(Framework::Control::Label("fileSizeLabel", COORD{ 1, 48 }, "TOTAL FILESIZE:", ForegroundColour::WHITE, BackgroundColour::GREY));

	// Create input boxes for user to change model and view.
	frame.AddControlToConsole(Framework::Control::InputTextBox("folderInput", COORD{ 10, 6 }, 100, folder, ForegroundColour::BLACK, BackgroundColour::WHITE));
	frame.AddControlToConsole(Framework::Control::InputTextBox("filterInput", COORD{ 10, 8 }, 50, filter, ForegroundColour::BLACK, BackgroundColour::WHITE));
	frame.AddControlToConsole(Framework::Control::Checkbox("recursiveCheck", COORD{ 20, 10 }, 2, ForegroundColour::BLACK, BackgroundColour::WHITE, rSearch, rSearch ? "X" : " "));

	// Create textboxes we will use to display file stats.
	frame.AddControlToConsole(Framework::Control::TextBox("tbxSearched", COORD{ 17, 44 }, 35, ForegroundColour::BLACK, BackgroundColour::WHITE, ""));
	frame.AddControlToConsole(Framework::Control::TextBox("tbxMatched", COORD{ 17, 46 }, 35, ForegroundColour::BLACK, BackgroundColour::WHITE, ""));
	frame.AddControlToConsole(Framework::Control::TextBox("tbxFileSize", COORD{ 17, 48 }, 35, ForegroundColour::BLACK, BackgroundColour::WHITE, ""));

	// Create the file viewer that will display files.
	frame.AddControlToConsole(Framework::Control::FileViewer("fv", 12, 31, ForegroundColour::WHITE, BackgroundColour::BLACK));

	return *this;
}

// Checks to see if a key down event has been fired. If so, it checks if the cursor has been placed
// in one of the input controls using the controlHit_ flag. If not, the key events that will be captured
// will affect the file view scroll. The key event switches on the virtual key code and completes actions
// based on the key press.

void FileView::ProcessKeyEvent(Event::Keyboard const& ke, FileModel& model) {
	if (ke.KeyDown())
	{
		
		// Get the two controls we want to examine for key events within them.
		Framework::Control::InputTextBox itbFolder = frame.GetControls().find("folderInput")->second;
		Framework::Control::InputTextBox itbFilter = frame.GetControls().find("filterInput")->second;

		if (itbFolder.controlHit_)
		{
			// Flag for signifying enter key stroke.
			bool enterHit_ = false;

			switch (ke.VirtualKeyCode())
			{
				case VK_BACK:
				{
					if (0 < itbFolder.cursorPos_ && itbFolder.cursorPos_ <= itbFolder.content_.size())
					{
						// Back space to remove character at cursor location.
						--itbFolder.cursorPos_;
						itbFolder.content_.erase(itbFolder.cursorPos_, 1);
					}
				}
				break;

				case VK_DELETE:
				{
					if (0 <= itbFolder.cursorPos_ && itbFolder.cursorPos_ < itbFolder.content_.size())
						itbFolder.content_.erase(itbFolder.cursorPos_, 1);
				}
				break;

				case VK_LEFT:
				{
					if (itbFolder.cursorPos_ > 0)
						--itbFolder.cursorPos_;
				}
				break;

				case VK_RIGHT:
				{
					if (itbFolder.cursorPos_ < itbFolder.content_.size())
						++itbFolder.cursorPos_;
				}
				break;

				case VK_END: itbFolder.cursorPos_ = itbFolder.content_.size(); break;
				case VK_HOME: itbFolder.cursorPos_ = 0; break;
				case VK_RETURN:
				{
					itbFolder.cursorPos_ = 0;
					itbFolder.aperature_ = 0;
					enterHit_ = true;
					itbFolder.controlHit_ = false;

					itbFolder.Update(itbFolder);
					Notify();
				}
				break;
				
				default:
				{
					char ch = ke.AsciiChar();
					if (isprint(ch))
						itbFolder.content_.insert(itbFolder.cursorPos_++ + itbFolder.content_.begin(), ch);
				}
				break;
			}

			// Update as typing occurs.
			auto size = itbFolder.content_.size() + 1;
			while (itbFolder.cursorPos_ < itbFolder.aperature_)
				--itbFolder.aperature_;

			while (itbFolder.cursorPos_ - itbFolder.aperature_ >= itbFolder.length_)
				++itbFolder.aperature_;

			while (size - itbFolder.aperature_ < itbFolder.length_ && size > itbFolder.length_)
				--itbFolder.aperature_;

			auto s = itbFolder.content_.substr(itbFolder.aperature_, itbFolder.length_);
			s += std::string(itbFolder.length_ / 2, ' ');

			frame.Write(itbFolder.xPos_, itbFolder.yPos_, s, itbFolder.foreground_, itbFolder.background_);
			itbFolder.UpdateInputContent(itbFolder);

			// Replace cursor.
			COORD cLoc{ itbFolder.xPos_, itbFolder.yPos_ };
			cLoc.X += itbFolder.cursorPos_ - itbFolder.aperature_;
			frame.ResetCursorPosition(cLoc.X, cLoc.Y, enterHit_ ? false : true);

			// Update control.
			itbFolder.Update(itbFolder);
		}
		else if (itbFilter.controlHit_)
		{
			// Flag for signifying enter key stroke.
			bool enterHit_ = false;

			switch (ke.VirtualKeyCode())
			{
			case VK_BACK:
			{
				if (0 < itbFilter.cursorPos_ && itbFilter.cursorPos_ <= itbFilter.content_.size())
				{
					// Back space to remove character at cursor location.
					--itbFilter.cursorPos_;
					itbFilter.content_.erase(itbFilter.cursorPos_, 1);
				}
			}
			break;

			case VK_DELETE:
			{
				if (0 <= itbFilter.cursorPos_ && itbFilter.cursorPos_ < itbFilter.content_.size())
					itbFilter.content_.erase(itbFilter.cursorPos_, 1);
			}
			break;

			case VK_LEFT:
			{
				if (itbFilter.cursorPos_ > 0)
					--itbFilter.cursorPos_;
			}
			break;

			case VK_RIGHT:
			{
				if (itbFilter.cursorPos_ < itbFilter.content_.size())
					++itbFilter.cursorPos_;
			}
			break;

			case VK_END: itbFilter.cursorPos_ = itbFilter.content_.size(); break;
			case VK_HOME: itbFilter.cursorPos_ = 0; break;
			case VK_RETURN:
			{
				itbFilter.cursorPos_ = 0;
				itbFilter.aperature_ = 0;
				enterHit_ = true;
				itbFilter.controlHit_ = false;

				itbFilter.Update(itbFilter);
				Notify();
			}
			break;

			default:
			{
				char ch = ke.AsciiChar();
				if (isprint(ch))
					itbFilter.content_.insert(itbFilter.cursorPos_++ + itbFilter.content_.begin(), ch);
			}
			break;
			}

			// Update as typing occurs.
			auto size = itbFilter.content_.size() + 1;
			while (itbFilter.cursorPos_ < itbFilter.aperature_)
				--itbFilter.aperature_;

			while (itbFilter.cursorPos_ - itbFilter.aperature_ >= itbFilter.length_)
				++itbFilter.aperature_;

			while (size - itbFilter.aperature_ < itbFilter.length_ && size > itbFilter.length_)
				--itbFilter.aperature_;

			auto s = itbFilter.content_.substr(itbFilter.aperature_, itbFilter.length_);
			s += std::string(itbFilter.length_ / 2, ' ');

			frame.Write(itbFilter.xPos_, itbFilter.yPos_, s, itbFilter.foreground_, itbFilter.background_);
			itbFilter.UpdateInputContent(itbFilter);

			// Replace cursor.
			COORD cLoc{ itbFilter.xPos_, itbFilter.yPos_ };
			cLoc.X += itbFilter.cursorPos_ - itbFilter.aperature_;
			frame.ResetCursorPosition(cLoc.X, cLoc.Y, enterHit_ ? false : true);

			// Update control.
			itbFilter.Update(itbFilter);
		}
		else
		{
			// These events will change the file view, much like the scroll method.
			switch (ke.VirtualKeyCode())
			{
				case VK_PRIOR:
				case VK_OEM_PLUS:
				case VK_UP:
				{
					// Make sure we are not exiting bounds of the vector.
					if (model.fPos_ > 0)
						--model.fPos_;

					// Reset and output.
					int viewBound = 0;
					model.startRow_ = 13;

					for (auto i = model.fPos_; i < model.GetFiles().size(); ++i) {
						if (viewBound <= 28) {
							frame.Write(1, model.startRow_, std::string(200, ' '), ForegroundColour::WHITE, BackgroundColour::BLACK);
							frame.Write(1, model.startRow_++, model.GetFiles()[static_cast<unsigned int>(i)], ForegroundColour::WHITE, BackgroundColour::BLACK);
						}
						else
							break;

						viewBound++;
					}
				}
				break;

				case VK_NEXT:
				case VK_OEM_MINUS:
				case VK_DOWN:
				{
					// Make sure we are not exiting bounds of the vector.
					if (model.fPos_ + 28 < model.GetFiles().size())
						++model.fPos_;

					// Reset and output.
					int viewBound = 0;
					model.startRow_ = 13;

					for (auto i = model.fPos_; i < model.GetFiles().size(); ++i) {
						if (viewBound <= 28) {
							frame.Write(1, model.startRow_, std::string(200, ' '), ForegroundColour::WHITE, BackgroundColour::BLACK);
							frame.Write(1, model.startRow_++, model.GetFiles()[static_cast<unsigned int>(i)], ForegroundColour::WHITE, BackgroundColour::BLACK);
						}
						else
							break;

						viewBound++;
					}
				}
				break;
			}
		}
	}
}

// Checks for mouse clicks which will update the appropriate control's cursor position within its bounds,
// and set its hit flag to true. Otherwise, the mouse wheel up or down will be used to scroll the file viewer.

void FileView::ProcessMouseEvent(Event::Mouse const& me, FileModel& model) {
	switch (me.GetType())
	{
		// Scroll events for the file viewer.
		case Event::Mouse::MouseType::WHEELED:
		{
			if (me.MouseWheelUp())
			{
				// Make sure we are not exiting bounds of the vector.
				if (model.fPos_ > 0)
					--model.fPos_;

				// Reset and output.
				int viewBound = 0;
				model.startRow_ = 13;

				for (auto i = model.fPos_; i < model.GetFiles().size(); ++i) {
					if (viewBound <= 28) {
						frame.Write(1, model.startRow_, std::string(200, ' '), ForegroundColour::WHITE, BackgroundColour::BLACK);
						frame.Write(1, model.startRow_++, model.GetFiles()[static_cast<unsigned int>(i)], ForegroundColour::WHITE, BackgroundColour::BLACK);
					}
					else
						break;

					viewBound++;
				}
			}
			else if (me.MouseWheelDown())
			{
				// Make sure we are not exiting bounds of the vector.
				if (model.fPos_ + 28 < model.GetFiles().size())
					++model.fPos_;

				// Reset and output.
				int viewBound = 0;
				model.startRow_ = 13;

				for (auto i = model.fPos_; i < model.GetFiles().size(); ++i) {
					if (viewBound <= 28) {
						frame.Write(1, model.startRow_, std::string(200, ' '), ForegroundColour::WHITE, BackgroundColour::BLACK);
						frame.Write(1, model.startRow_++, model.GetFiles()[static_cast<unsigned int>(i)], ForegroundColour::WHITE, BackgroundColour::BLACK);
					}
					else
						break;

					viewBound++;
				}
			}
		}
		break;
		case Event::Mouse::MouseType::BUTTON:
		{
			// Get variables we will need.
			auto clickPos = me.MousePosition();
			
			Framework::Control::Checkbox cb = frame.GetControls().find("recursiveCheck")->second;
			Framework::Control::InputTextBox itbFolder = frame.GetControls().find("folderInput")->second;
			Framework::Control::InputTextBox itbFilter = frame.GetControls().find("filterInput")->second;

			// Test for change to recursion.
			cb.controlHit_ = clickPos.X == 20 && clickPos.Y == 10;
			if (cb.controlHit_ && me.LeftPressed())
			{
				// Change state and content according to the press.
				cb.state_ = !cb.state_;
				cb.content_ = cb.state_ ? "X" : " ";
				cb.controlHit_ = !cb.controlHit_;

				// Update control.
				cb.Update(cb);
				cb.UpdateCheckState(cb);

				// Notify controller we need to update model and view.
				Notify();
			}

			// Test for click on folder textbox.
			itbFolder.controlHit_ = clickPos.X >= 10 && clickPos.X <= 185 && clickPos.Y == 6;
			if (itbFolder.controlHit_ && me.LeftPressed())
			{
				// Show cursor at selection point.
				itbFolder.cursorPos_ = min(me.MousePosition().X - itbFolder.xPos_ + itbFolder.aperature_, itbFolder.content_.size());
				COORD mLoc{ itbFolder.cursorPos_ - itbFolder.aperature_ + itbFolder.xPos_, itbFolder.yPos_ };
				frame.ResetCursorPosition(mLoc.X, mLoc.Y, true);

				// Update control.
				itbFolder.Update(itbFolder);
				
				// Update other control to take off its controlHit status.
				itbFilter.controlHit_ = false;
				itbFilter.Update(itbFilter);
			}

			// Test for click on filter textbox.
			itbFilter.controlHit_ = clickPos.X >= 10 && clickPos.X <= 185 && clickPos.Y == 8;
			if (itbFilter.controlHit_ && me.LeftPressed())
			{
				// Show cursor at selection point.
				itbFilter.cursorPos_ = min(me.MousePosition().X - itbFilter.xPos_ + itbFilter.aperature_, itbFilter.content_.size());
				COORD mLoc{ itbFilter.cursorPos_ - itbFilter.aperature_ + itbFilter.xPos_, itbFilter.yPos_ };
				frame.ResetCursorPosition(mLoc.X, mLoc.Y, true);

				// Update control.
				itbFilter.Update(itbFilter);

				// Update other control to take off its controlHit status.
				itbFolder.controlHit_ = false;
				itbFolder.Update(itbFolder);
			}
		}
		break;
	}
}

// Switches the ctrl handled events. If the event is equal to CTRL + C, the application's
// quit flag will be set and the program will destruct.

BOOL FileView::CtrlHandler(DWORD ctrlType) {
	switch (ctrlType)
	{
	case CTRL_C_EVENT:
		done = true;
		return TRUE;
	}

	return FALSE;
}

//Implementation of Framework Class

//creates the checkbox and sets its value to default
Framework::Control::Checkbox::Checkbox(std::string id, COORD origin, WORD const length, ForegroundColour foreground,BackgroundColour background, bool state, std::string content) {
	// Set the internal members to defaults.
	xPos_ = origin.X;
	yPos_ = origin.Y;
	length_ = length;

	foreground_ = foreground;
	background_ = background;

	content_ = content;
	controlID_ = id;

	cursorPos_ = 0;
	aperature_ = 0;

	controlHit_ = false;
	state_ = state;
}
Framework::Control::Checkbox::Checkbox(Control const& c) {
	// Set internal members.
	xPos_ = c.xPos_;
	yPos_ = c.yPos_;
	length_ = c.length_;

	foreground_ = c.foreground_;
	background_ = c.background_;

	content_ = c.content_;
	controlID_ = c.controlID_;

	cursorPos_ = c.cursorPos_;
	aperature_ = c.aperature_;

	controlHit_ = c.controlHit_;
	state_ = c.state_;
}

//creates the label and sets its value to the given parameter
Framework::Control::Label::Label(std::string id, COORD origin, std::string content,ForegroundColour foreground, BackgroundColour background) {
	// Set internal members.
	xPos_ = origin.X;
	yPos_ = origin.Y;

	foreground_ = foreground;
	background_ = background;

	content_ = content;
	controlID_ = id;
}

Framework::Control::TextBox::TextBox(std::string id, COORD origin, WORD const length, ForegroundColour foreground, BackgroundColour background, std::string content) {
	// Set internal members.
	xPos_ = origin.X;
	yPos_ = origin.Y;
	length_ = length;

	foreground_ = foreground;
	background_ = background;

	controlID_ = id;
	content_ = content;
}
//Sets the Textbox using the given control
Framework::Control::TextBox::TextBox(Control const& c) {
	// Set internal members.
	xPos_ = c.xPos_;
	yPos_ = c.yPos_;
	length_ = c.length_;

	foreground_ = c.foreground_;
	background_ = c.background_;

	controlID_ = c.controlID_;
	content_ = c.content_;
}

//Sets the input textbox to the give parameter
Framework::Control::InputTextBox::InputTextBox(std::string id, COORD origin, WORD const length, std::string content, ForegroundColour foreground, BackgroundColour background) {
	// Set internal members.
	xPos_ = origin.X;
	yPos_ = origin.Y;
	length_ = length;

	foreground_ = foreground;
	background_ = background;

	controlID_ = id;
	content_ = content;

	cursorPos_ = 0;
	aperature_ = 0;

	controlHit_ = false;
}

Framework::Control::InputTextBox::InputTextBox(Control const& c) {
	// Set internal members.
	xPos_ = c.xPos_;
	yPos_ = c.yPos_;
	length_ = c.length_;

	foreground_ = c.foreground_;
	background_ = c.background_;

	controlID_ = c.controlID_;
	content_ = c.content_;

	cursorPos_ = c.cursorPos_;
	aperature_ = c.aperature_;

	controlHit_ = c.controlHit_;
}

Framework::Control::FileViewer::FileViewer(std::string id, WORD const x, WORD const height, ForegroundColour foreground, BackgroundColour background) {
	// Set internal members.
	xPos_ = x;
	height_ = height;

	foreground_ = foreground;
	background_ = background;

	controlID_ = id;
}
Framework::Control::FileViewer::FileViewer(Control const& c) {
	// Set internal members.
	xPos_ = c.xPos_;
	height_ = c.height_;

	foreground_ = c.foreground_;
	background_ = c.background_;

	controlID_ = c.controlID_;
}

//Methods
// Adds a layout section to the console to indicate areas of the console that will have
// certain controls grouped together using the Console thick wrapper function to fill.

void Framework::AddLayoutToConsole(Framework::Layout const& l) {
	// Make call to the console to fill out the layout section on the screen.
	console_.Fill(l.startLoc_, l.size_, l.foreground_, l.background_);
}

// Adds a control to the console to allow for display or interaction with the user, for
// the program to update the model/view on different inputs using the Console thick wrapper function.

void Framework::AddControlToConsole(Framework::Control const& control) {
	// Check to see what control we are adding.
	if (dynamic_cast<Framework::Control::FileViewer const*>(&control))
	{
		// Create a FileViewer control and paint to screen.
		Framework::Control::FileViewer fv = control;
		console_.Fill(fv.xPos_, fv.height_, fv.foreground_, fv.background_);
		controls_.insert(std::make_pair(fv.controlID_, control));
	}
	else
	{
		console_.Draw(control.xPos_, control.yPos_, control.length_, control.foreground_, control.background_);
		controls_.insert(std::make_pair(control.controlID_, control));
	}
}

// Adds a control to the console to allow for text display only using the Console thick wrapper function.

void Framework::AddTextToConsole(Framework::Control const& control) {
	// Write text at console using control location and console call.
	console_.Write(control.xPos_, control.yPos_, control.content_, control.foreground_, control.background_);
}

// Enables a handler to look for Ctrl key events using the Console thick wrapper function.

void Framework::EnableCtrlHandler(PHANDLER_ROUTINE routine) {
	console_.EnableCtrlHandler(routine);
}

// Sets the cursor position within the console window and its visibility using the Console thick wrapper function.

void Framework::ResetCursorPosition(SHORT const x, SHORT const y, bool visible) {
	console_.SetCursor(x, y, visible);
}

// Used to write text or content to the console at a specific coordinate using the Console thick wrapper function.

void Framework::Write(WORD const x, WORD const y, std::string content, ForegroundColour foreground, BackgroundColour background) {
	console_.Write(x, y, content, foreground, background);
}

// Used to call specific console functions to set size, title and other functions to create the eventual look
// and feel of the file browser using the Console thick wrapper functions.

void Framework::SetupConsole() {
	console_.SetTitle("TUI File Browser").SetSize(130, 50).SetCursor(0, 0, false).SetBackgroundColour(BackgroundColour::WHITE).EnableKeyboardAndMouse();
}


// Updates the recursive toggle on the view to indicate whether or not
// the search is a recursive or non-recursive by writing to the control's content
// using the Write function.

void Framework::Control::Checkbox::UpdateCheckState(Framework::Control::Checkbox const& c) {
	frame.Write(c.xPos_, c.yPos_, c.content_, c.foreground_, c.background_);
}

// Updates the folder or filter input with the content from the console arguments passed in at program
// start, or with the new user's input by writing to the control's content
// using the Write function.

void Framework::Control::InputTextBox::UpdateInputContent(Framework::Control::InputTextBox const& itb) {
	frame.Write(itb.xPos_, itb.yPos_, itb.content_, itb.foreground_, itb.background_);
}

// Updates the file search stats on the bottom of the console with the content after
// each new model scan by writing to the control's content, once to clear and once with the actual content,
// using the Write function.

void Framework::Control::TextBox::UpdateContent(Framework::Control::TextBox const& tbx) {
	frame.Write(tbx.xPos_, tbx.yPos_, std::string(tbx.length_, ' '), tbx.foreground_, tbx.background_);
	frame.Write(tbx.xPos_, tbx.yPos_, tbx.content_, tbx.foreground_, tbx.background_);
}
/**
* Writes new files/content to the view when a model completes a new scan using the Write function.
*/
void Framework::Control::FileViewer::UpdateFileView(Framework::Control::FileViewer const& fv) {
	frame.Write(fv.xPos_, fv.yPos_, fv.content_, fv.foreground_, fv.background_);
}

// Clears the contents of the fileview area by looping through the immediately viewable area,
// and using the Write function to write a blank string across the whole length of the control.

void Framework::Control::FileViewer::ClearFileView() {
	WORD row = 13;
	for (auto i = 0; i <= 28; i++) {
		frame.Write(1, row++, std::string(200, ' '), ForegroundColour::WHITE, BackgroundColour::BLACK);
	}
}

// Updates the variables within the particular control, saving its state by looping through the map,
// finding the matching key value, and setting the Control associated with the key to the control reference.

void Framework::Control::Update(Framework::Control& control) {
	for (std::map<std::string, Control>::iterator it = frame.controls_.begin(); it != frame.controls_.end(); ++it) {
		if (it->first == control.controlID_) {
			it->second = control;
			break;
		}
	}
}

// Returns an event from the console's read in input by using the Console thick wrapper function.

Event Framework::GetEvent() {
	std::vector<INPUT_RECORD> inBuffer;
	DWORD numEvent;

	console_.GetEvent(inBuffer, numEvent);
	auto e  = Event::Event(inBuffer[0]);
	return e;
}

// Gets the current working directory that the executable of this program is located in,
// by using the Console thick wrapper function.

std::string Framework::GetCurrentDir() {
	std::string dir = console_.GetCurrentDir();
	return dir;
}



//Method
// A method that will scan a folder for files by first setting the state of the model's variables to zero
// and then depending on the state of the recurse flag, it will loop through the directories using the appropriate
// iterator starting at the passed in path "f". It will return all file names that match the regex and place them into
// the model's file vector.

void FileModel::Scan(std::tr2::sys::path const& f, std::regex r, bool recurse) {
	// Zero out counters/file list before each scan.
	sFiles_ = 0;
	mFiles_ = 0;
	startRow_ = 0;
	fPos_ = 0;
	fSize_ = 0;
	files_.clear();

	// Used for reducing file size to MB.
	double const BYTES_TO_MB = 1048576;

	// Scan appropriately.
	if (recurse)
	{
		std::tr2::sys::recursive_directory_iterator d(f);
		std::tr2::sys::recursive_directory_iterator e;

		for (; d != e; d++)
		{
			if (!is_directory(d->status()))
			{
				// Increment search counter again as we have hit a file.
				sFiles_++;

				// Check to see if extension of file matches files we are looking for.
				if (std::regex_match(d->path().extension(), r))
				{
					// Increment counters.
					mFiles_++;
					fSize_ += file_size(d->path()) / BYTES_TO_MB;

					// Add to the file list.
					files_.push_back(d->path());
				}
			}
			else
				sFiles_++; // Increment counter here as this will be the level at which folders will be searched.
		}
	}
	else
	{
		std::tr2::sys::directory_iterator d(f);
		std::tr2::sys::directory_iterator e;

		for (; d != e; d++)
		{
			if (!is_directory(d->status()))
			{
				// Increment search counter again as we have hit a file.
				sFiles_++;

				// Check to see if extension of file matches files we are looking for.
				if (std::regex_match(d->path().extension(), r))
				{
					// Increment counters.
					mFiles_++;
					fSize_ += file_size(d->path()) / BYTES_TO_MB;

					// Add to the file list.
					files_.push_back(d->path());
				}
			}
			else
				sFiles_++; // Increment counter here as this will be the level at which folders will be searched.
		}
	}
}



// Methods

// Used when the FileView/FileModel notifies the controller that it needs to update the both of them. 
// Will then call the UpdateModel and UpdateView functions.

void FileController::Update() {
	UpdateModel();
	UpdateView();
}

// Updates the view with the new data from the model's current state after a new scan by getting the controls
// that will be affected by the model's new state. This includes the recursive toggle, folder and filter input, 
// the file stats and the file view. It will update the state of all these controls with the new data and then update
// their state to the view.

void FileController::UpdateView() {
	// Get all of the controls we will need to update.
	Framework::Control::Checkbox cb = frame.GetControls().find("recursiveCheck")->second;
	Framework::Control::InputTextBox itbFolder = frame.GetControls().find("folderInput")->second;
	Framework::Control::InputTextBox itbFilter = frame.GetControls().find("filterInput")->second;
	Framework::Control::TextBox tbxSearched = frame.GetControls().find("tbxSearched")->second;
	Framework::Control::TextBox tbxMatched = frame.GetControls().find("tbxMatched")->second;
	Framework::Control::TextBox tbxFileSize = frame.GetControls().find("tbxFileSize")->second;
	Framework::Control::FileViewer fv = frame.GetControls().find("fv")->second;

	// Initial update pre-scanning.
	cb.state_ = model_.IsRecursive();
	cb.content_ = cb.state_ ? "X" : " ";
	itbFolder.content_ = model_.GetSearchFolder();
	itbFilter.content_ = model_.GetSearchFilter();
	fv.yPos_ = 13;
	fv.xPos_ = 1;

	// Update controls.
	cb.Update(cb);
	cb.UpdateCheckState(cb);

	itbFolder.Update(itbFolder);
	itbFolder.UpdateInputContent(itbFolder);

	itbFilter.Update(itbFilter);
	itbFilter.UpdateInputContent(itbFilter);

	fv.Update(fv);
	fv.ClearFileView();
	fv.UpdateFileView(fv);

	// Output contents of files.
	int viewBound = 0;
	model_.startRow_ = fv.yPos_;
	model_.fPos_ = 0;

	for (auto i = model_.fPos_; i < model_.GetFiles().size(); ++i) {
		if (viewBound <= 28) {
			frame.Write(1, model_.startRow_++, model_.GetFiles()[static_cast<unsigned int>(i)], fv.foreground_, fv.background_);
		}
		else
			break;

		viewBound++;
	}

	model_.startRow_ = 0;

	// Output file stats.
	std::string sStat;
	std::string mStat;
	std::string fStat;

	std::stringstream s;
	s << model_.GetSearchedFiles();
	s >> sStat;
	s.clear();

	s << model_.GetMatchedFiles();
	s >> mStat;
	s.clear();

	s << model_.GetSizeOfFiles();
	s >> fStat;
	s.clear();

	tbxSearched.content_ = sStat;
	tbxMatched.content_ = mStat;
	tbxFileSize.content_ = fStat + "MB";

	// Update map and output stats.
	tbxSearched.Update(tbxSearched);
	tbxSearched.UpdateContent(tbxSearched);

	tbxMatched.Update(tbxMatched);
	tbxMatched.UpdateContent(tbxMatched);

	tbxFileSize.Update(tbxFileSize);
	tbxFileSize.UpdateContent(tbxFileSize);
}

// Updates the model with the data from the view's user input by retrieving the recursive toggle,
// the folder and filter input and the file view. Using the data that was set by the user, the model is reconstructed
// using the content of these controls. Then a scan is performed using this new data.

void FileController::UpdateModel() {

	// Get all of the controls we will need to get input from.
	Framework::Control::Checkbox cb = frame.GetControls().find("recursiveCheck")->second;
	Framework::Control::InputTextBox itbFolder = frame.GetControls().find("folderInput")->second;
	Framework::Control::InputTextBox itbFilter = frame.GetControls().find("filterInput")->second;
	Framework::Control::FileViewer fv = frame.GetControls().find("fv")->second;

	// Update the model.
	model_ = FileModel(itbFolder.content_, itbFilter.content_, cb.state_);

	// Indicate to user that a scan is in progress for recursive scans, in the case that the scan is a large drive.
	if (model_.IsRecursive()) {
		fv.yPos_ = 13;
		fv.xPos_ = 1;
		fv.content_ = "Scan in progress...";
		fv.ClearFileView();
		fv.Update(fv);
		fv.UpdateFileView(fv);
	}
	else
		fv.ClearFileView();

	// Populate the model's data with a new scan.
	std::regex r;
	try
	{
		r = model_.GetSearchFilter();
	}
	catch (std::exception)
	{
		throw ConsoleAPI::XError("Invalid regex.", 833);
	}
	
	model_.Scan(std::tr2::sys::path(model_.GetSearchFolder()), r, model_.IsRecursive());
}