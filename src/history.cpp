#include <string>
#include <utility>
#include "utils.h"
#include <fstream>
#include "cinclude/cunistd.h"
#include "history.h"



void History::tryLoad(const std::string &fileName)
{
	std::ifstream file(fileName);

	if (!file)
		return;

	list.clear();

	std::string line;

	while (std::getline(file, line))
		list.push_back(line);

	file.close();
}

void History::save(const std::string &fileName)
{
	std::ofstream file(fileName);

	if (!file)
		return;

	auto range = getRecent(maxCount);
	auto it = range.first;
	auto end = range.second;

	for (; it != end; ++it)
	{
		file << *it << std::endl;
	}

	file.close();
}

void History::append(const std::string &content)
{
	list.push_back(content);
}

History::ConstStringListRange History::getRecent(int count)
{
	auto end = list.end();
	auto begin = list.size() > count ? list.end() - count : list.begin();
	return std::make_pair(begin, end);
}

History::ConstStringListRange History::getAll()
{
	return std::make_pair(list.begin(), list.end());
}

const std::string &History::getLast() { return list.back(); }

const std::string &History::getNthItemCountedFromBack(int index)
{
	return list[list.size() - 1 - index];
}

const std::string &History::getNthItem(int index)
{
	return list[index];
}

void History::setMaxCount(int count) { maxCount = count; }

int History::getMaxCount() { return maxCount; }

int History::getCurrentCount() { return (int)list.size(); }
