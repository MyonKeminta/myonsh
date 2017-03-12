#include "application.h"
#include <vector>
#include <string>

int main(int argc, const char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);
	return Application::getInstance()->run(args);
}

