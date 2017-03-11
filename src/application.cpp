#include "application.h"
#include <string>
#include <vector>
#include <iostream>

Application *Application::instance = nullptr;

Application *Application::getInstance()
{
	if(instance)
		return instance;
	return instance = new Application;
}

int Application::run(const std::vector<std::string> &args)
{
	for(const auto &arg : args)
		std::cout<<arg<<std::endl;
	
	return 0;
}
