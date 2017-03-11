#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include <string>


class Application
{
public:
	static Application *getInstance();

	int run(const std::vector<std::string> &args);

private:
	static Application *instance;
};


#endif //APPLICATION_H
