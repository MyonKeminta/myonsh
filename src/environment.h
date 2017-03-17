#ifndef ENVIRONMENT_MANAGER
#define ENVIRONMENT_MANAGER

#include <string>
#include <stack>
#include <vector>
//#include <cstdlib>
//#include <unordered_map>
#include "utils.h"


class Environment
{
public:

	static Environment *getInstance();

	static void initWithArgs(const std::vector<std::string> &args);


	// Environmental variables

	std::string getEnv(const std::string &name) const 
	{
		return getEnv(name.c_str());
	}

	std::string getEnv(const char *name) const;

	bool hasEnv(const std::string &name) const
	{
		return hasEnv(name.c_str());
	}

	bool hasEnv(const char *name) const;

	bool tryGetEnv(const std::string &name, std::string &dest) const
	{
		return tryGetEnv(name.c_str(), dest);
	}

	bool tryGetEnv(const char *name, std::string &dest) const;

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
	// Moved into class Context, which is not singleton
	//const StringMap &getGlobalVariableDictionary() const;
	
	//StringMap &getGlobalVariableDictionary();


	
	// Path

	StringList getSystemPath() const;


	// Pwd

	int chdir(const std::string &path) { return chdir(path.c_str()); }

	int chdir(const char *path);


	// Prompt
	// Moved into class Context, which is not singleton
	//std::string getPrompt() const;


	// UID

	int getUid() const;

	int getEuid() const;

	std::string getPwd() const;

	std::string getHostName() const;

	std::string getUserName() const;

	std::string getHome() const;


	bool isInputRedirected() const;


	const std::vector<std::string> &getArgs() const;



private:
	static Environment *instance;

//	Environment();
	explicit Environment(const std::vector<std::string> &args);

	//StringMap globalDict;

	std::vector<std::string> args;
	
};


#endif //ENVIRONMENT_MANAGER
