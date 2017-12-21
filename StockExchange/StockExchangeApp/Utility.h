
#pragma once
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <vector>

/*
* TRACE CONFIG
* change TRACE_LEVEL according to the desired detailed level (1: important details only, 2: display more details)
*/
#define TRACE_LEVEL 1
#define TRACE(message, level) if (level <= TRACE_LEVEL) std::wcout << message << std::endl;


/*
* STRING UTILITY CLASS
* used accross the application to change from and to strings and wide strings
*/
static class Utility
{
public:
	Utility();
	~Utility();
	static std::wstring StringToWString(const std::string& s)
	{
		std::wstring temp(s.length(), L' ');
		std::copy(s.begin(), s.end(), temp.begin());
		return temp;
	}
	static std::string WStringToString(const std::wstring& s)
	{
		std::string temp(s.length(), ' ');
		std::copy(s.begin(), s.end(), temp.begin());
		return temp;
	}

private:

};
