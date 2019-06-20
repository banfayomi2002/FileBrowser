/**
@file: main.cpp
/** @file : ConsoleApp.cpp
Name : Fayomi Augustine
Purpose: Implementation of singleton for MVCApp.
History : The program is using console API and the sample code given by Garth Santors.
Date : 16/03/2016
version: 1.0
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