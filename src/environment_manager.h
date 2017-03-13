#ifndef ENVIRONMENT_MANAGER
#define ENVIRONMENT_MANAGER

#include "cinclude/cunistd.h"
#include <string>
#include <stack>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include "utils.h"


class Environment
{
public:
	static Environment getInstance();


	// Environmental variables

	std::string getEnv(const std::string &name) const 
	{
		return getEnv(name.c_str());
	}

	std::string getEnv(const char *name) const;

	void setEnv(const std::string &name, const std::string &value) 
	{
		setEnv(name.c_str(), value.c_str());
	}

	void setEnv(const char *name, const char *value);

	void unsetEnv(const std::string &name)
	{
		unsetEnv(name.c_str());
	}

	void unsetEnv(const char *name);


	// Global variables
	
	const StringMap &getGlobalVariableDictionary() const;
	
	StringMap &getGlobalVariableDictionary();


	
	// Path

	std::vector<std::string> getSystemPath() const;


	// Pwd

	void chdir(const std::string &path) { chdir(path.c_str()); }

	void chdir(const char *path);


	// Prompt

	std::string getPrompt() const;





private:
	static Environment *instance;

	StringMap globalDict;
	
};


#endif //ENVIRONMENT_MANAGER
