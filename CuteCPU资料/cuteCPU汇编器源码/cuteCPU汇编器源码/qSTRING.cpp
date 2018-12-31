#include "qSTRING.h"

/***************************************************************/
/***************************************************************/
/*
*ȥ���ַ�����β�� \x20 \r \n �ַ�
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
    //�����������ַ�����β�� ������ǰ
    --p;
    ++start;
    if(*start == 0)
    {
        //�Ѿ����ַ�����ĩβ��
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
	return s.substr(0,length);//����pos��ʼ��n���ַ���ɵ��ַ���
}

/***************************************************************/
/***************************************************************/

string qSTRING::right(string s, int length)
{
    if (s.empty())
    {
        return s;
    }
	return s.substr(s.length()-length,length);//����pos��ʼ��n���ַ���ɵ��ַ���
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

	return trim(s.substr(0,pos));//����pos��ʼ��n���ַ���ɵ��ַ���
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
	return trim(s.substr(pos+1,s.length()-pos-1));//����pos��ʼ��n���ַ���ɵ��ַ���
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

	return trim(s.substr(0,pos));//����pos��ʼ��n���ַ���ɵ��ַ���
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

//ע�⣺���ַ���Ϊ��ʱ��Ҳ�᷵��һ�����ַ���
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
    /*���ͣ�
        sin>>t��ʾ��sinת����double�ı�������ʵ����int��float�͵Ķ�����գ������ת���ɹ�����ֵΪ��0�����ת�����ɹ��ͷ���Ϊ0
    */
           return false;
    if(sin >> p)
    /*���ͣ��˲������ڼ����������У����ּ��ַ�����������ʽ�����磺34.f����������ĵĲ��֣�sin>>t���Ѿ����ղ�ת������������ֲ��֣���stringstream����ӦҲ�����һ���ָ�����������ʱ�����ַ��������ּ��ַ�����������ʽ����˲��ֿ���ʶ�𲢽����ַ����֣�����������˵�ģ����յ���.f�ⲿ�֣�������������������false;���ʣ�µĲ��ֲ����ַ�����ô��sin>>p��Ϊ0,����е���һ��else����
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
