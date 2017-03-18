#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include <string>


//The main application delegate
class Application
{
public:
	//Get the singleton instance
	static Application *getInstance();

	//Run the main application
	int run(const std::vector<std::string> &args);

	bool isInteractive() const;

private:
	//Singleton instance handler
	static Application *instance;

//	//Initialize environment:
//	//Trap SIGINT signal if interactive
//	//Initialize interpreter
//	//Initialize text provider
//	void initialize() {}

//	//Untrap signals
//	//Unload what have to unload
//	//Prepare for exec
//	void retire();

	bool interactive;

//	int execute(const std::string &scriptPath, const std::vector<std::string> &args);



};


#endif //APPLICATION_H
