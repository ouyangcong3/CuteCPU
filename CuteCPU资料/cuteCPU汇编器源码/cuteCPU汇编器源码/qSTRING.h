
#ifndef _CPPSTRING_H_
#define _CPPSTRING_H_

//#pragma warning(disable : 4786)

#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

#include <vector>
using namespace std;

#include <string.h>
class qSTRING
{
public:
	static void charTrim(char* str);
	static string left(string s, int length);
	static string right(string s, int length);
	static string leftc(string s, char c);
	static string rightc(string s, char c);
	static string lefts(string s, string c);

	static string trim(string s);
	static string tolower(string s);
	static string toupper(string s);

	static void  split(string s,string delim,vector< string >* ret);
	static bool  isnum(string s);
	static int   s2int(string s);
	static string  int2s(int i);

};

/////////////////////////////////////////////////////////////////////////////
#endif
