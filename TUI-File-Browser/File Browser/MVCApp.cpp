/** @file : MVCApp.cpp
Name : Fayomi Augustine
Purpose: Implementation file for FileBrowser application.
History : The program is using console API and the sample code given by Garth Santors.
Date : 16/03/2016
version: 1.0
**/

#include "ConsoleApp.h"
#include "FileBrowser.hpp"
#include <vector>
#include <sstream>
#include <regex>
#define _CRTDBG_MAP_ALLOC //Memory leak detection
#include <stdlib.h>
#include <crtdbg.h>
#include "Event.h"


using namespace std;
Framework mvc = Framework();

void ProcessEvents(FileView& view, FileModel& model) {
	while (!view.GetQuitState()) {
	
		auto e = mvc.GetEvent();
		switch (e.GetType())
		{
		case Event::EventType::KEY: view.ProcessKeyEvent(e.GetKeyboardEvent(), model); break;
			case Event::EventType::MOUSE: view.ProcessMouseEvent(e.GetMouseEvent(), model); break;
		}
	}
}

//App a;	// can't, App is protected

class MyApp : public App {
	//Framework frame = Framework();
	int execute(int argc, char* argv[]) override {
		//cout << "Hello, sub-class" << endl;



		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);  //detects memory leak
		// Create variables to hold the arguments passed in.
		bool recursive = false;

		string startPath = mvc.GetCurrentDir();
		string regexFilter(".*");

		// Convert args to a more C++ friendly variety.
		vector<string> args;
		for (int i = 0; i < argc; ++i)
			args.push_back(argv[i]);

		// Parse the arguments and assign to the approprite variables.
		for (unsigned i = 0; i < args.size(); ++i)
		{
			if (i != 0)
			{
				if (args[i] == "-r" && recursive == false)
					recursive = true;
				else if (regex_search(args[i], regex("^[a-z]|[A-Z]")))
					startPath = args[i];
				else
					regexFilter = args[i];
			}

			try
			{
				// Create application.
				FileView view(startPath, regexFilter, recursive);
				FileModel model(startPath, regexFilter, recursive);
				FileController controller(model, view);

				// Attach.
				model.Attach(&controller);
				view.Attach(&controller);

				// Notify the controller that the state has changed to start the application.
				model.Notify();

				// Start looking for processing events.
				ProcessEvents(view, controller.GetModel());
			}
			catch (ConsoleAPI::XError& e)
			{
				MessageBoxA(NULL, e.GetFile(), "Runtime Error", MB_OK);
			}
		}

		return EXIT_SUCCESS;
	}
} app;