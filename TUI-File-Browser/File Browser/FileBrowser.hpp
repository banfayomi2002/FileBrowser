/** @file : Controller.h
Name : Fayomi Augustine
Purpose: Header file for FileBrowser application.
History : The program is using console API and the sample code given by Garth Santors.
Date : 16/03/2016
version: 1.0
**/


#ifndef __FILEBROWSER_APP_GUARD__
#define __FILEBROWSER_APP_GUARD__

#include "Console.hpp"

#include <set>
#include <map>
#include <regex>
#include <filesystem>
#include "Event.h"
#include "Color.h"


//  Observer Pattern

class IObserver
{
	public:
		virtual void Update() = 0;
};
class AbstractSubject
{
	
	private:
		std::set<IObserver*> observers_;

	// Methods
	public:
		void Attach(IObserver* p) { observers_.insert(p); }
		void Detach(IObserver* p) { observers_.erase(p); }

		void Notify()
		{
			for (auto o : observers_)
				o->Update();
		}
};

  
//  CONTROLLER/MODEL/VIEW CLASS IMPLEMENTATIONS SECTION

//Framework class that contains all the classes used for displaying all element on console
class Framework
{
	//Layout class contains method and attribute for displaying the console
	public:
		class Layout
		{
		//member
			public:
				WORD startLoc_;
				WORD size_;

				ForegroundColour foreground_;
				BackgroundColour background_;

				std::string layoutID_;

		
			public:
				Layout(std::string id, WORD const layoutStart, WORD const size, ForegroundColour foreground, BackgroundColour background) : layoutID_(id), startLoc_(layoutStart), size_(size), foreground_(foreground), background_(background) { };
		};
		class Control
		{
			// member
			public:
				WORD xPos_;
				WORD yPos_;
				WORD length_;
				WORD height_;

				ForegroundColour foreground_;
				BackgroundColour background_;

				std::string content_;
				std::string controlID_;
				std::string::size_type cursorPos_;

				decltype(cursorPos_) aperature_;

				bool controlHit_;
				bool state_;

		
			public:
				virtual ~Control() { };

			//Inherited consol control classes
			public:
				class Checkbox;
				class Label;
				class TextBox;
				class InputTextBox;
				class FileViewer;

			// methods
			public:
				
				 // Updates the variables within the particular control, saving its state.
				
				void Update(Framework::Control& control);
		};

		class Control::Checkbox : public Control
		{
			// -------- CONSTRUCTOR --------
			public:
				Checkbox(std::string id, COORD origin, WORD const length, ForegroundColour foreground, BackgroundColour background, bool state, std::string content);
				Checkbox(Control const& c);

			// -------- OPERATIONS --------
			public:
				
				 // Updates the recursive toggle on the view to indicate whether or not
				 // the search is a recursive or non-recursive.
				 
				void UpdateCheckState(Framework::Control::Checkbox const& c);
		};
		class Control::Label : public Control
		{
			// -------- CONSTRUCTOR --------
			public:
				Label(std::string id, COORD origin, std::string content, ForegroundColour foreground, BackgroundColour background);
		};
		class Control::TextBox : public Control
		{
			// -------- CONSTRUCTOR --------
			public:
				TextBox(std::string id, COORD origin, WORD const length, ForegroundColour foreground, BackgroundColour background, std::string content);
				TextBox(Control const& c);

			// -------- OPERATIONS --------
			public:
				
				 // Updates the file search stats on the bottom of the console with the content after
				 // each new model scan.
				 
				void UpdateContent(Framework::Control::TextBox const& tbx);
		};
		class Control::InputTextBox : public Control
		{
			// -------- CONSTRUCTOR --------
			public:
				InputTextBox(std::string id, COORD origin, WORD const length, std::string content, ForegroundColour foreground, BackgroundColour background);
				InputTextBox(Control const& c);

			// -------- OPERATIONS --------
			public:
				
				// Updates the folder or filter input with the content from the console arguments passed in at program
				// start, or with the new user's input.
				
				void UpdateInputContent(Framework::Control::InputTextBox const& itb);
		};
		class Control::FileViewer : public Control
		{
			// -------- CONSTRUCTOR --------
			public:
				FileViewer(std::string id, WORD const x, WORD const height, ForegroundColour foreground, BackgroundColour background);
				FileViewer(Control const& c);

			// -------- OPERATIONS --------
			public:
				
				 // Writes new files/content to the view when a model completes a new scan.
				 
				void UpdateFileView(Framework::Control::FileViewer const& fv);
				
				 // Clears the contents of the fileview area.
				 
				void ClearFileView();
		};

	// -------- CLASS MEMBERS --------
	private:
		std::map<std::string, Framework::Control> controls_;
		Console console_;

	// -------- CONSTRUCTOR --------
	private:
		//Framework() {};

		//// Add these so instances cannot be created this way.
		//Framework(Framework const&) = delete;
		//void operator=(Framework const&) = delete;

	public:
		/*static Framework& GetInstance() {
			static Framework instance_;
			return instance_;
		}*/
		Framework() {};
	// -------- OPERATIONS --------
	public:
		
		 // Adds a layout section to the console to indicate areas of the console that will have 
		 // certain controls grouped together.
		
		void AddLayoutToConsole(Layout const& layout);
		
		 // Adds a control to the console to allow for display or interaction with the user, for
		 // the program to update the model/view on different inputs.
		
		void AddControlToConsole(Control const& control);
		
		 // Adds a control to the console to allow for text display only. Used for labels only.
		 
		void AddTextToConsole(Control const& control);
		
		// Used to write text or content to the console at a specific coordinate.
		
		void Write(WORD const x, WORD const y, std::string content, ForegroundColour foreground, BackgroundColour background);
		
		// Sets the cursor position within the console window and its visibility.
		
		void ResetCursorPosition(SHORT const x, SHORT const y, bool visible);
		
		// Enables a handler to look for Ctrl key events.
		
		void EnableCtrlHandler(PHANDLER_ROUTINE routine);
		
		 // Used to call specific console functions to set size, title and other functions to create the eventual look
		 // and feel of the file browser.
		 
		void SetupConsole();
		
		// Returns an event from the console's read in input.
		
		Event GetEvent();
		
		// Gets the current working directory that the executable of this program is located in.
		
		std::string GetCurrentDir();

		std::map<std::string, Framework::Control> &GetControls() { return controls_; }
};

class FileModel : public AbstractSubject
{
	// -------- CONSTRUCTORS --------
	public:
		FileModel() { };
		FileModel(std::string f, std::string r, bool recurse) : folder_(f), regex_(r), recursion_(recurse) { };

	// -------- CLASS MEMBERS --------
	private:
		std::vector<std::string> files_;

		unsigned long long	sFiles_;
		unsigned long long	mFiles_;
		double long		fSize_;
		
		std::string folder_;
		std::string regex_;
		bool		recursion_;

	public:
		unsigned long long	fPos_;
		unsigned int startRow_;

	// -------- OPERATIONS --------
	public:
		
		 // A method that will scan a folder for files.
		
		void Scan(std::tr2::sys::path const& f, std::regex r, bool recurse);

	// -------- ACCESSORS --------
	public:
		bool IsRecursive() const { return recursion_; }

		std::string GetSearchFolder() const { return folder_; }
		std::string GetSearchFilter() const { return regex_; }

		unsigned long long GetSearchedFiles() const { return sFiles_; }
		unsigned long long GetMatchedFiles() const { return mFiles_; }
		double long GetSizeOfFiles() const { return fSize_; }

		std::vector<std::string> GetFiles() const { return files_; }
};
class FileView : public AbstractSubject
{
	
	private:
		static bool done;

	
	public:
		FileView() { };
		FileView(std::string folder, std::string filter, bool rSearch);

	// methods
	public:
		
		 // Used to construct the look of the console including controls and layouts.
		
		FileView& CreateTUI(std::string folder, std::string filter, bool rSearch);

	
		 // A handler to handle CTRL + events. This will be used to capture break events.
		
		static BOOL CtrlHandler(DWORD ctrlType);

	// -------- EVENT PROCESSING -------- 
	public:
		
		 // Used to process various key events passed into the console.
		
		void ProcessKeyEvent(Event::Keyboard const& ke, FileModel& model);
		
		// Used to process various mouse events passed into the console.
		
		void ProcessMouseEvent(Event::Mouse const& me, FileModel& model);

	
	public:
		bool GetQuitState() const { return done; }
};
class FileController : public IObserver
{	
	// Data Members
	private:
		FileModel	model_;
		FileView	view_;

	
	public:
		FileController(FileModel const& fm, FileView const& fb) : model_(fm), view_(fb) { };

	// Methods
	public:
		
		 // Used when the FileView/FileModel notifies the controller that it needs to update the both of them.
		 
		void Update() override;

		
		 // Updates the view with the new data from the model's current state after a new scan.
		 
		void UpdateView();
		
		 // Updates the model with the data from the view's user input.
		 
		void UpdateModel();

	// -------- ACCESSORS --------
	public:
		FileModel& GetModel() { return model_; }
};

#endif