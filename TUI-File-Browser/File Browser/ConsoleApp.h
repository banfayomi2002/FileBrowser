/** @file : ConsoleApp.h
Name : Fayomi Augustine
Purpose: Conains memeber and methods used for singleton implementation
History : The program is using console API and the sample code given by Garth Santors.
Date : 16/03/2016
version: 1.0
**/
#include <string>
#include <vector>


class App {
	using Args = std::vector<std::string>;
	Args _args;
	static App* thisApp;
	static int _main(int argc, char* argv[]);
	friend int main(int argc, char* argv[]);
protected:
	~App();
	App();
	Args const& GetArgs() const { return _args; }
	virtual int execute(int argc, char* argv[]);
};