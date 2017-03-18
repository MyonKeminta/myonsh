#include "preprocessor.h"
#include "utils.h"
#include <string>
#include <deque>
#include "cassert"
#include "exceptions.h"
#include <regex>
#include <cctype>


Preprocessor::Preprocessor(Context &context, bool expandAlias)
	: context(&context), expandAlias(expandAlias)
{

}

bool Preprocessor::getProcessedCmd(ScriptSource &source, StringList &dest)
{
	this->source = &source;
	std::string cmd;
	if (getLine(cmd))
	{
		dest = process(cmd);
		return true;
	}
	else
	{
		dest.clear();
		return false;
	}
}

StringList Preprocessor::process(const std::string &_cmd)
{
	std::string cmd = _cmd;
	StringList result;
	unsigned i = 0;
	while (isspace(cmd[i]))
		++i;

	// Try to expand alias
	if (expandAlias && (isalpha(cmd[i]) || cmd[i] == '_'))
	{
		bool isValid = true;
		unsigned end;
		for (end = i + 1; cmd[end] != ' ' && end < cmd.size(); ++end)
		{
			if (!isalnum(cmd[end]) && cmd[end] != '_')
			{
				isValid = false;
				break;
			}
		}
		if (isValid)
		{
			std::string name(cmd.begin() + i, cmd.begin() + end);
			assert(name.front() != ' ');
			assert(name.back() != ' ');
			auto it = context->getAliasDictionary().find(name);
			if (it != context->getAliasDictionary().end())
			{
//				result.push_back(it->second);
				std::string temp(cmd.begin() + end, cmd.end());
				cmd = it->second + temp;
				i = 0;
			}
		}
	}

	bool isInStr = false;

	while (i < cmd.size())
	{
		// Skip spaces
		if (isspace(cmd[i]))
		{
			++i;
			continue;
		}

		std::string current;

		while (i < cmd.size())
		{
			if (cmd[i] == '\'')
			{
				++i;
				while (cmd[i] != '\'')
				{
					current.push_back(cmd[i++]);
				}
				assert(i < cmd.size() && cmd[i] == '\'');
				++i;
			}
			else if (cmd[i] == '\\')
			{
				++i;
				if (i < cmd.size())
					current.push_back(cmd[i]);
				else
					current.push_back('\\');
				++i;
			}
			else if (cmd[i] == '$')
			{
				// Expand macro
				++i;
				if (i < cmd.size())
				{
					if (isdigit(cmd[i])) //Args
					{
						//unsigned index = i - '0';
						//TODO: expand args
						++i;
					}
					else if (cmd[i] == '?')
					{
						current.append(std::to_string(context->getLastExitCode()));
						++i;
					}
					else if (isalpha(cmd[i]) || cmd[i] == '_')
					{
						std::string name{ cmd[i++] };
						while (i < cmd.size() && (isalnum(cmd[i]) || cmd[i] == '_'))
							name.push_back(cmd[i++]);
						current.append(context->getVar(name));
					}
					else if (cmd[i] == '{')
					{
						std::string name;
						++i;
						while (cmd[i] != '}')
							name.push_back(cmd[i]);
						assert(i < cmd.size() && cmd[i] == '}');
						current.append(context->getVar(name));
						++i;
					}
					else
					{
						current.push_back('$');
						current.push_back(cmd[i]);
						++i;
					}
				}
				else
				{
					current.push_back('$');
				}
			}
			else if (cmd[i] == '"')
				isInStr = ! isInStr;
			else if (isspace(cmd[i]))
			{
				if (isInStr)
				{
					current.push_back(cmd[i]);
					++i;
				}
				else
				{
					++i;
					break;
				}
			}
			else if (cmd[i] == ';' && !isInStr)
			{
				result.push_back(current);
				result.push_back(";");
				++i;
			}
			else if (cmd[i] == '&' && !isInStr)
			{
				result.push_back(current);
				result.push_back("&");
				++i;
			}

			// TODO: Finish this.
//			else if (cmd[i] == '|')
//			{
//				//...
//			}
//			else if (cmd[i] == '>')
//			{
//				//...
//			}
//			else if (cmd[i] == '<')
//			{
//				//...
//			}

//			else if (cmd[i]== '~' && !isInStr)
//			{
//				++i;
//				if (i >= cmd.size() || cmd[i] == '/')

//			}
			else
			{
				current.push_back(cmd[i]);
				++i;
			}
		}

		result.push_back(current);
	}
	return result;
}

bool Preprocessor::getLine(std::string &str)
{
	std::string result;

	bool isInStr = false;

	bool hasInput = false;

	while (true)
	{
		assert(context->getStatus() == Context::Ready || isInStr);
		char c;
		if (!getChar(c) /*|| c == '\n'*/)
			break;
		else
			hasInput = true;

		//Comments
		if (c == '#' && !isInStr)
		{
			while (getChar(c) && c != '\n')
				continue;
			continue;
		}

		if (c == '\n' && !isInStr)
			break;

		result.push_back(c);


		if (c == '"')
		{
			if (isInStr)
			{
				isInStr = false;
				context->setStatus(Context::Ready);
			}
			else
			{
				isInStr = true;
				context->setStatus(Context::Hanging);
			}
		}
		else if (c == '\'')
		{
			if (!isInStr)
			{
				context->setStatus(Context::Hanging);
				do
				{
					if (!getChar(c))
						throw SyntaxError("Unexpected EOF encountered");
					result.push_back(c);
				} while(c != '\'');
				context->setStatus(Context::Ready);
			}
		}
		else if (c == '\\')
		{
			if (!isInStr)
				context->setStatus(Context::Hanging);
			if (getChar(c))
			{
				if (c == '\n')
					result.pop_back();
				else
					result.push_back(c);
			}
			if (!isInStr)
				context->setStatus(Context::Ready);
		}
		else if (c == '$')
		{
			if (!isInStr)
				context->setStatus(Context::Hanging);

			if (getChar(c))
			{
				result.push_back(c);
				if (c == '{')
				{
					do
					{
						if (!getChar(c))
							throw SyntaxError("Unexpected EOF encountered");
						result.push_back(c);
					} while (c != '}');
				}
			}

			if (!isInStr)
				context->setStatus(Context::Ready);
		}
	}

	str = result;
	return hasInput;
}






bool Preprocessor::getChar(char &c)
{
	if (puttedBack.empty())
		return source->getChar(c);
	c = puttedBack.top();
	puttedBack.pop();
	return true;
}

void Preprocessor::putBack(char c)
{
	puttedBack.push(c);
}

