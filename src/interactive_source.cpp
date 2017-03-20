#include "interactive_source.h"
#include "context.h"
#include <string>
#include <iostream>
#include <exception>
#include "history.h"
#include <cctype>


// // ...Nothing here

StringList History::list;

int History::maxCount = 2000;

bool InteractiveSource::getChar(char &c)
{
	if (cursor <  buffer.size())
	{
		c = buffer[cursor++];
		return true;
	}

	bool isConverted = false;

	while (true)
	{

		printPrompt();
		buffer.clear();

		std::string input;

		if (!std::getline(std::cin, input))
		{
			c = '\0';
			return false;
		}

		unsigned i = 0;

		isConverted = false;

		try
		{

			while (i < input.size())
			{
				if (input[i] == '!')
				{
					++i;
					if (i >= input.size())
					{
						buffer.push_back('!');
					}
					else if (input[i] == '!')
					{
						isConverted = true;
						if (History::getCurrentCount())
							buffer.append(History::getLast());
						else
							throw std::logic_error("!!: event not found");
					}
					else if (isdigit(input[i]))
					{
						isConverted = true;
						int index = input[i] - '0';
						++i;
						while (i < input.size() && isdigit(input[i]))
							index = index * 10 + (input[i++] - '0');
						--i;
						if (index >= History::getCurrentCount())
							throw std::logic_error("!" + std::to_string(index) + ": event not found");
						buffer.append(History::getNthItem(index));
					}
					else if (input[i] == '-' && i + 1 < input.size() && isdigit(input[i+1]))
					{
						isConverted = true;
						++i;
						int index = 0;
						while (i < input.size() && isdigit(input[i]))
							index = 10 * index + (input[i] - '0');
						--i;
						if (index >= History::getCurrentCount())
							throw std::logic_error("!-" + std::to_string(index) + ": event not found");
						buffer.append(History::getNthItemCountedFromBack(index));
					}
					else
					{
						buffer.push_back('!');
						buffer.push_back(input[i]);
					}
				}
				else
				{
					buffer.push_back(input[i]);
				}

				++i;
			}

		} catch (const std::logic_error &ex)
		{
			Log::LogError(ex.what());
			continue;
		}

		break;
	}

	if (isConverted)
		std::cout << buffer << std::endl;

	if (!History::getCurrentCount() || buffer != History::getLast())
		History::append(buffer);

	buffer.push_back('\n');
	c = buffer[0];
	cursor = 1;
	return true;
}
