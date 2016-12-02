#include "stringUtils.h"



bool stringStartsWith(std::string a, std::string with, bool ignoreWhiteSpaces)
{
	if (a.size() < with.size())
		return false;
	if (with.size() == 0)
		return true;

	std::string result;
	if (ignoreWhiteSpaces)
	{
		for (size_t i = 0; i < a.size(); i++)
		{
			if (a[i] != ' ')
				result += a[i];
		}
	}
	else result = a;

	for (size_t i = 0; i < with.size(); i++)
	{
		if (result[i] != with[i])
		{
			return false;
		}
	}

	return true;
}
