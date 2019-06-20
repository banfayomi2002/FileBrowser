/**
	@file: FileBrowser.hpp
	@author: Aaron Gray
	@author_email: a_gray5@fanshaweonline.ca
	@date: 2015-04-12
	@brief: Header file for FileBrowser application.
**/

#ifndef __FILEBROWSER_APP_GUARD__
#define __FILEBROWSER_APP_GUARD__

#include "Console.hpp"

#include <set>
#include <map>
#include <regex>
#include <filesystem>

// ==========================================
// | OBSERVER PATTERN CLASS IMPLEMENTATIONS |
// ==========================================
class IObserver
{
	// -------- OPERATIONS --------
	public:
		virtual void Update() = 0;
};
class AbstractSubject
{
	// -------- CLASS MEMBER --------
	private:
		std::set<IObserver*> observers_;

	// -------- OPERATIONS --------
	public:
		void Attach(IObserver* p) { observers_.insert(p); }
		void Detach(IObserver* p) { observers_.erase(p); }

		void Notify()
		{
			for (auto o : observers_)
				o->Update();
		}
};

// ===============================================
// | CONTROLLER/MODEL/VIEW CLASS IMPLEMENTATIONS |
// ===============================================
class Framework
{
	// -------- CONTROL CLASSES --------
	public:
		class Layout
		{
			// -------- CLASS MEMBERS --------
			public:
				WORD startLoc_;
				WORD size_;

				ConsoleAPI::ForegroundColour foreground_;
				ConsoleAPI::BackgroundColour background_;

				std::string layoutID_;

			// -------- CONSTRUCTOR --------
			public:
				Layout(std::string id, WORD const layoutStart, WORD const size, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background) : layoutID_(id), startLoc_(layoutStart), size_(size), foreground_(foreground), background_(background) { };
		};
		class Control
		{
			// -------- CLASS MEMBERS --------
			public:
				WORD xPos_;
				WORD yPos_;
				WORD length_;
				WORD height_;

				ConsoleAPI::ForegroundColour foreground_;
				ConsoleAPI::BackgroundColour background_;

				std::string content_;
				std::string controlID_;
				std::string::size_type cursorPos_;

				decltype(cursorPos_) aperature_;

				bool controlHit_;
				bool state_;

			// -------- DESTRUCTOR (FOR POLYMORPHISM) --------
			public:
				virtual ~Control() { };

			// -------- INHERITANCE CLASSES --------
			public:
				class Checkbox;
				class Label;
				class TextBox;
				class InputTextBox;
				class FileViewer;

			// -------- OPERATIONS --------
			public:
				/**
				 * Updates the variables within the particular control, saving its state.
				 *
				 * @note - all controls that extend from this class have this method.
				 */
				void Update(Framework::Control& control);
		};

		class Control::Checkbox : public Control
		{
			// -------- CONSTRUCTOR --------
			public:
				Checkbox(std::string id, COORD origin, WORD const length, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background, bool state, std::string content);
				Checkbox(Control const& c);

			// -------- OPERATIONS --------
			public:
				/**
				 * Updates the recursive toggle on the view to indicate whether or not
				 * the search is a recursive or non-recursive.
				 */
				void UpdateCheckState(Framework::Control::Checkbox const& c);
		};
		class Control::Label : public Control
		{
			// -------- CONSTRUCTOR --------
			public:
				Label(std::string id, COORD origin, std::string content, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background);
		};
		class Control::TextBox : public Control
		{
			// -------- CONSTRUCTOR --------
			public:
				TextBox(std::string id, COORD origin, WORD const length, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background, std::string content);
				TextBox(Control const& c);

			// -------- OPERATIONS --------
			public:
				/**
				 * Updates the file search stats on the bottom of the console with the content after
				 * each new model scan.
				 */
				void UpdateContent(Framework::Control::TextBox const& tbx);
		};
		class Control::InputTextBox : public Control
		{
			// -------- CONSTRUCTOR --------
			public:
				InputTextBox(std::string id, COORD origin, WORD const length, std::string content, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background);
				InputTextBox(Control const& c);

			// -------- OPERATIONS --------
			public:
				/**
				* Updates the folder or filter input with the content from the console arguments passed in at program
				* start, or with the new user's input.
				*/
				void UpdateInputContent(Framework::Control::InputTextBox const& itb);
		};
		class Control::FileViewer : public Control
		{
			// -------- CONSTRUCTOR --------
			public:
				FileViewer(std::string id, WORD const x, WORD const height, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background);
				FileViewer(Control const& c);

			// -------- OPERATIONS --------
			public:
				/**
				 * Writes new files/content to the view when a model completes a new scan.
				 */
				void UpdateFileView(Framework::Control::FileViewer const& fv);
				/**
				 * Clears the contents of the fileview area.
				 */
				void ClearFileView();
		};

	// -------- CLASS MEMBERS --------
	private:
		std::map<std::string, Framework::Control> controls_;
		Console console_;

	// -------- CONSTRUCTOR --------
	private:
		Framework() {};

		// Add these so instances cannot be created this way.
		Framework(Framework const&) = delete;
		void operator=(Framework const&) = delete;

	public:
		static Framework& GetInstance() {
			static Framework instance_;
			return instance_;
		}

	// -------- OPERATIONS --------
	public:
		/**
		 * Adds a layout section to the console to indicate areas of the console that will have 
		 * certain controls grouped together.
		 *
		 * @param layout - a reference to the layout that is to be added to the console screen.
		 */
		void AddLayoutToConsole(Layout const& layout);
		/**
		 * Adds a control to the console to allow for display or interaction with the user, for
		 * the program to update the model/view on different inputs.
		 *
		 * @param control - a reference to the control that is to be added to the console screen.
		 */
		void AddControlToConsole(Control const& control);
		/**
		 * Adds a control to the console to allow for text display only. Used for labels only.
		 *
		 * @param control - a reference to the control that is to be added to the console screen.
		 */
		void AddTextToConsole(Control const& control);
		/**
		* Used to write text or content to the console at a specific coordinate.
		*
		* @param x - The x coordinate to start writing content at.
		* @param y - The y coordinate to start writing content at.
		* @param content - The content/text to write at specified location.
		* @param foreground - colour of the text to write.
		* @param background - colour of the background that the content will be written overtop.
		*/
		void Write(WORD const x, WORD const y, std::string content, ConsoleAPI::ForegroundColour foreground, ConsoleAPI::BackgroundColour background);
		/**
		* Sets the cursor position within the console window and its visibility.
		*
		* @param x - The x coordinate of where the cursor will be placed on the x-axis.
		* @param y - The y coordinate of where the cursor will be placed on the y-axis.
		* @param visible - Whether or not the cursor will be displayed at the location passed in.
		*/
		void ResetCursorPosition(SHORT const x, SHORT const y, bool visible);
		/**
		* Enables a handler to look for Ctrl key events.
		*
		* @param routine - A pointer to the handler routine for the event.
		*/
		void EnableCtrlHandler(PHANDLER_ROUTINE routine);
		/**
		 * Used to call specific console functions to set size, title and other functions to create the eventual look
		 * and feel of the file browser.
		 */
		void SetupConsole();
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
		/**
		 * A method that will scan a folder for files.
		 *
		 * @param f - a reference to the path of the folder.
		 * @param r - a regex to filter file extensions to match it.
		 * @param recurse - a flag to indicate whether the scan should start in the folder specified and
		                    go through its sub folders.
		 */
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
	// -------- CLASS MEMBERS --------
	private:
		static bool quit_;

	// -------- CONSTRUCTOR --------
	public:
		FileView() { };
		FileView(std::string folder, std::string filter, bool rSearch);

	// -------- OPERATIONS --------
	public:
		/**
		 * Used to construct the look of the console including controls and layouts.
		 *
		 * @param folder - the start folder for the first search on startup.
		 * @param filter - the start filter for the first search on startup.
		 * @param rSearch - whether or not the first search is recursive.
		 */
		FileView& CreateTUI(std::string folder, std::string filter, bool rSearch);

		/**
		 * A handler to handle CTRL + events. This will be used to capture break events.
		 *
		 * @param ctrlType - The CTRL event that is being captured.
		 */
		static BOOL CtrlHandler(DWORD ctrlType);

	// -------- EVENT PROCESSING -------- 
	public:
		/**
		 * Used to process various key events passed into the console.
		 * 
		 * @param ke - A reference to the key event
		 * @param model - A reference to the current model state.
		 */
		void ProcessKeyEvent(ConsoleAPI::Event::Keyboard const& ke, FileModel& model);
		/**
		* Used to process various mouse events passed into the console.
		*
		* @param me - A reference to the mouse event
		* @param model - A reference to the current model state.
		*/
		void ProcessMouseEvent(ConsoleAPI::Event::Mouse const& me, FileModel& model);

	// -------- ACCESSOR --------
	public:
		bool GetQuitState() const { return quit_; }
};
class FileController : public IObserver
{	
	// -------- CLASS MEMBERS --------
	private:
		FileModel	model_;
		FileView	view_;

	// -------- CONSTRUCTOR --------
	public:
		FileController(FileModel const& fm, FileView const& fb) : model_(fm), view_(fb) { };

	// -------- OPERATIONS --------
	public:
		/**
		 * Used when the FileView/FileModel notifies the controller that it needs to update the both of them.
		 */
		void Update() override;

		/**
		 * Updates the view with the new data from the model's current state after a new scan.
		 */
		void UpdateView();
		/**
		 * Updates the model with the data from the view's user input.
		 */
		void UpdateModel();

	// -------- ACCESSORS --------
	public:
		FileModel& GetModel() { return model_; }
};

#endif