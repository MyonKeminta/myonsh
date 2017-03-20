#ifndef HISTORY_H
#define HISTORY_H


#include <string>
#include <utility>
#include "utils.h"



class History
{
public:

	typedef std::pair<StringList::iterator, StringList::iterator> StringListRange;

	typedef std::pair<StringList::const_iterator, StringList::const_iterator>
		ConstStringListRange;


	static void tryLoad(const std::string &fileName);

	static void save(const std::string &fileName);

	static void append(const std::string &content);

	static ConstStringListRange getRecent(int count = 1);

	static ConstStringListRange  getAll();

	static const std::string &getLast();


	//Counted from 0

	static const std::string &getNthItemCountedFromBack(int index);

	static const std::string &getNthItem(int index);

	static void setMaxCount(int count);

	static int getMaxCount();

	static int getCurrentCount();

private:

	static StringList list;

	static int maxCount;

};



#endif //HISTORY_H
