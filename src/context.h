#ifndef CONTEXT_H
#define CONTEXT_H



#include "utils.h"
#include <string>
#include <stack>
#include "environment.h"



class LocalContext
{
public:

	const StringMap &getVariableDictionary() const;

	StringMap &getVariableDictionary();

private:

	StringMap variableDict;
};


class Context
{
public:

	enum Status { Ready, Hanging };

public:

	Context();

	const StringMap &getGlobalVariableDictionary() const;

	StringMap &getGlobalVariableDictionary();

	const StringMap &getFunctionDictionary() const;

	StringMap &getFunctionDictionary();

	const StringMap &getAliasDictionary() const;

	StringMap &getAliasDictionary();


	std::string getPrompt() const;


	std::string getVar(const std::string &name) const;

	bool hasVar(const std::string &name) const;

	void setVar(const std::string &name, const std::string &value);

	void unset(const std::string &name);

	void exportVar(const std::string &name);


	Status getStatus() const;

	void setStatus(const Status &value);


	void initInteractiveContext();

	int getLastExitCode() const { return lastExitCode; }

	void setLastExitCode(int value) { lastExitCode = value; }


protected:

	std::string parsePrompt(const std::string &ps) const;

private:

	std::stack<LocalContext> callStack;

	StringMap globalVariableDict;

	StringMap functionDict;

	StringMap aliasDictionary;

	bool interactive = false;

	Status status;

	Environment *environment;

	int lastExitCode = 0;
};



#endif //CONTEXT_H
