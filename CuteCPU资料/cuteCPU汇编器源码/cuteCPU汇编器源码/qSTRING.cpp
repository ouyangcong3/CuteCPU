#include "qSTRING.h"

/***************************************************************/
/***************************************************************/
/*
*去掉字符串首尾的 \x20 \r \n 字符
*/
void qSTRING::charTrim(char* str)
{
	char *start = str - 1;
    char *end = str;
    char *p = str;
    while(*p)
    {
        switch(*p)
        {
        case ' ':
        case '\r':
        case '\n':
            {
                if(start + 1==p)
                    start = p;
            }
            break;
        default:
            break;
        }
        ++p;
    }
    //现在来到了字符串的尾部 反向向前
    --p;
    ++start;
    if(*start == 0)
    {
        //已经到字符串的末尾了
        *str = 0 ;
        return;
    }
    end = p + 1;
    while(p > start)
    {
        switch(*p)
        {
        case ' ':
        case '\r':
        case '\n':
            {
                if(end - 1 == p)
                    end = p;
            }
            break;
        default:
            break;
        }
        --p;
    }
    memmove(str,start,end-start);
    *(str + (int)end - (int)start) = 0;
}

/***************************************************************/
/***************************************************************/

string qSTRING::left(string s, int length)
{
    if (s.empty())
    {
        return s;
    }
	return s.substr(0,length);//返回pos开始的n个字符组成的字符串
}

/***************************************************************/
/***************************************************************/

string qSTRING::right(string s, int length)
{
    if (s.empty())
    {
        return s;
    }
	return s.substr(s.length()-length,length);//返回pos开始的n个字符组成的字符串
}

/***************************************************************/
/***************************************************************/

string qSTRING::leftc(string s, char c)
{
	int pos;
    if (s.empty())
    {
        return s;
    }
	pos = s.find(c);

	return trim(s.substr(0,pos));//返回pos开始的n个字符组成的字符串
}

/***************************************************************/
/***************************************************************/

string qSTRING::rightc(string s, char c)
{
	int pos;
    if (s.empty())
    {
        return s;
    }
	pos = s.find(c);
	return trim(s.substr(pos+1,s.length()-pos-1));//返回pos开始的n个字符组成的字符串
}

/***************************************************************/
/***************************************************************/

string qSTRING::lefts(string s, string c)
{
	int pos;
    if (s.empty())
    {
        return s;
    }
	pos = s.find(c.c_str());

	return trim(s.substr(0,pos));//返回pos开始的n个字符组成的字符串
}

/***************************************************************/
/***************************************************************/

string qSTRING::trim(string s)
{
    if (s.empty())
    {
        return s;
    }
    int len = s.size();

    if((s.c_str()[0] == '\n')||( s.c_str()[len-1] == '\n'))
        s.erase(s.find('\n'),1);

    if((s.c_str()[0] == '\r')||( s.c_str()[len-1] == '\r'))
        s.erase(s.find('\r'),1);

    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

/***************************************************************/
/***************************************************************/

string qSTRING::tolower(string s)
{
	string lower;
	lower.resize(s.size());
	transform(s.begin(), s.end(), lower.begin(), ::tolower);
	return lower;
}

/***************************************************************/
/***************************************************************/

string qSTRING::toupper(string s)
{
	string upper;
	upper.resize(s.size());
    transform(s.begin(), s.end(), upper.begin(), ::toupper);
	return upper;
}

/***************************************************************/
/***************************************************************/

//注意：当字符串为空时，也会返回一个空字符串
void qSTRING::split(string s,string delim,vector< string >* ret)
{
	size_t last = 0;
	size_t index=s.find_first_of(delim,last);
	while (index!=string::npos)
	{
		ret->push_back(s.substr(last,index-last));
		last=index+1;
		index=s.find_first_of(delim,last);
	}
	if (index-last>0)
	{
		ret->push_back(s.substr(last,index-last));
	}
}

/***************************************************************/
/***************************************************************/

bool qSTRING::isnum(string s)
{
    stringstream sin(s);
    double t;
    char p;
    if(!(sin >> t))
    /*解释：
        sin>>t表示把sin转换成double的变量（其实对于int和float型的都会接收），如果转换成功，则值为非0，如果转换不成功就返回为0
    */
           return false;
    if(sin >> p)
    /*解释：此部分用于检测错误输入中，数字加字符串的输入形式（例如：34.f），在上面的的部分（sin>>t）已经接收并转换了输入的数字部分，在stringstream中相应也会把那一部分给清除，如果此时传入字符串是数字加字符串的输入形式，则此部分可以识别并接收字符部分，例如上面所说的，接收的是.f这部分，所以条件成立，返回false;如果剩下的部分不是字符，那么则sin>>p就为0,则进行到下一步else里面
      */
            return false;
    else
            return true;
}

/***************************************************************/
/***************************************************************/

int qSTRING::s2int(string s)
{
    stringstream sin(s);
    int t;
    sin >> t;
	return t;
}

/***************************************************************/
/***************************************************************/

string qSTRING::int2s(int i)
{
    stringstream sm;
	string str;
    sm << i;
	sm>>str;
	return str;
}
