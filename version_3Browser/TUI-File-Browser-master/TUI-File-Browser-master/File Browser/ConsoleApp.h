
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