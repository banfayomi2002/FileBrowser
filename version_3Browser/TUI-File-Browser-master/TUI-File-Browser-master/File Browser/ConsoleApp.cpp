/**
@file: main.cpp
@author: Aaron Gray
@author_email: a_gray5@fanshaweonline.ca
@date: 2015-04-12
@brief: Main method.
**/

#include "ConsoleApp.h"

#include <vector>
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) try {
	return App::_main(argc, argv);
}
catch (...) {
	cout << "Error: an exception has been thrown..." << endl;
}

App* App::thisApp = nullptr;

App::App() {
	if (thisApp)
		throw "Error: App already initialized";
	cout << "App: starting..." << endl;
	thisApp = this;
}


App::~App() {
	cout << "App: Goodbye" << endl;
}

int App::execute(int argc, char* argv[]) {
	cout << "Hello, world!" << endl;
	return EXIT_SUCCESS;
}


int App::_main(int argc, char* argv[]) {
	for (int i = 1; i < argc; ++i)
		thisApp->_args.push_back(argv[i]);

	return thisApp->execute(argc, argv);
}