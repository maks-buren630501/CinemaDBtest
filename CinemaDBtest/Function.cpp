#include"Function.h"


string bstr_to_str(BSTR source)
{
	_bstr_t wrapped_bstr = _bstr_t(source);
	int length = wrapped_bstr.length();
	char* char_array = new char[length];
	strcpy_s(char_array, length + 1, wrapped_bstr);
	return char_array;
}

int checkUser(const char* nickname, list<string> listOfUsers)
{
	std::list<string>::iterator iterator = std::find(listOfUsers.begin(), listOfUsers.end(), string(nickname));
	if (iterator == listOfUsers.end())
	{
		return -1;
	}
	else
	{
		return 1;
	}
}