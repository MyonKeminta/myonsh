#ifndef CONTEXT_H
#define CONTEXT_H



#include "utils.h"
#include <string>
#include <stack>



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

	Context();

	const StringMap &getGlobalVariableDictionary() const;

	StringMap &getGlobalVariableDictionary();

	const StringMap &getFunctionDictionary() const;

	StringMap &getFunctionDictionary();

	const StringMap &getAliasDictionary() const;

	StringMap &getAliasDictionary();


	std::string getPrompt() const;


	std::string getVar(const std::string &name) const;

	void setVar(const std::string &name, const std::string &value);

	void unset(const std::string &name);

	void exportVar(const std::string &name);




private:

	std::stack<LocalContext> callStack;

	StringMap globalVariableDict;

	StringMap functionDict;

	StringMap aliasDictionary;


};



#endif //CONTEXT_H
