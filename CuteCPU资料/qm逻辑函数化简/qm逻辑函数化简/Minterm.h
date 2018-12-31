/*************************************************
Copyright:
Author:qqlqql，QQ 631011506
Date:2016-12-18
Description:最小项类
**************************************************/

#ifndef _MINTERM_H_
#define _MINTERM_H_

//输出个数
#define  ownc  3
//变量数
#define  XC    4

class Minterm
{
public:
	//Minterm(char , char , char , char , char ,char, char , char , char ,char);
	Minterm();
	~Minterm();

	void setdefault();
	bool operator==(Minterm &);
	void operator=(int);
	void operator=(Minterm &);

	void setvariable(int ,char);
	char getvariable(int);
	void cover();
	bool iscover();
	void print();
	int  bleng();
	int  getonecount();
	int  getnumber(int);
	static int length(Minterm *);
	static bool isown(Minterm &,Minterm &,int *);
	static bool arryequ(int *,int *,int);
	int  *own;
private:
	int mutiple2(int);
	void setnumber();

private:
	char *a;
	int  *b;

	bool  bcover;
};

#endif
