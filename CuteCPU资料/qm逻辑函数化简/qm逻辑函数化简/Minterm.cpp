/*************************************************
Copyright:
Author:qqlqql，QQ 631011506
Date:2016-12-18
Description:最小项类
**************************************************/

#include <iostream>
#include "Minterm.h"

using namespace std;

/*
Minterm::Minterm(char c0, char c1, char c2, char c3, char c4, char c5,char c6,char c7, char c8, char c9)
{
	a=new char[10];
	a[0]=c0;a[1]=c1;a[2]=c2;a[3]=c3;a[4]=c4;a[5]=c5;a[6]=c6;a[7]=c7;a[8]=c8;a[9]=c9;
	b=new int[1024];
	setnumber();
}
*/

Minterm::Minterm()
{
	bcover = false;
	own = new int[ownc];
	for(int i=0;i<ownc;i++)
	{
		own[i]=0;
	}
	a=new char[10];
	for(int i=0;i<10;i++)
	{
		a[i]='-';
	}
	b=new int[1024];
	setnumber();
}

Minterm::~Minterm()
{
	delete []a;
	delete []b;
	delete []own;
}

void Minterm::setdefault()
{
	bcover = false;
	a[0]='-';a[1]='-';a[2]='-';a[3]='-';a[4]='-';a[5]='-';a[6]='-';a[7]='-';a[8]='-';a[9]='-';
	setnumber();
	for(int i=0;i<ownc;i++)
	{
		own[i]=0;
	}
}

bool Minterm::operator ==(Minterm &A)//找出只有一处不同，是就返回真
{
	int i,t,index;
	t=0;

	for(i=0;i<10;i++)
	{
		if(A.a[i]!=a[i])
		{
			t=t+1;
			index = i;
		}
	}
	if(t!=1)
		return false;

	if((A.a[index]=='1'||A.a[index]=='0')&&(a[index]=='1'||a[index]=='0'))
		return true;
	else
		return false;
}

void Minterm::operator=(int num)
{
	bcover = false;
	if(num>511){a[0]='1'; num=num-512;}
	else  a[0]='0';
	if(num>255){a[1]='1'; num=num-256;}
	else  a[1]='0';
	if(num>127){a[2]='1'; num=num-128;}
	else  a[2]='0';
	if(num>63){a[3]='1'; num=num-64;}
	else  a[3]='0';
	if(num>31){a[4]='1'; num=num-32;}
	else  a[4]='0';
	if(num>15){a[5]='1'; num=num-16;}
	else  a[5]='0';
	if(num>7){a[6]='1'; num=num-8;}
	else  a[6]='0';
	if(num>3){a[7]='1'; num=num-4;}
	else  a[7]='0';
	if(num>1){a[8]='1'; num=num-2;}
	else  a[8]='0';
	if(num>0){a[9]='1';}
	else  a[9]='0';
	setnumber();
	for(int i=0;i<ownc;i++)
	{
		own[i]=0;
	}
}

void Minterm::operator=(Minterm &A)
{
	int i;
	bcover = A.bcover;
	for(i=0;i<10;i++)
		a[i]=A.a[i];
	setnumber();
	for(i=0;i<ownc;i++)
	{
		own[i]=A.own[i];
	}
}

void Minterm::setvariable(int i, char ch)
{
	a[i]=ch;
	setnumber();
}

void Minterm::print()
{
	int i;

	for(i=0;i<10;i++)
	{
		cout<<a[i];
	}
	//cout<<" "<<*b<<endl;
	cout<<" = ";
	for(i=0;i<bleng();i++)
		cout<<b[i]<<",";
}

char Minterm::getvariable(int i)
{
	return a[i];
}

int Minterm::length(Minterm *A)
{
	int i,k;
	char ch[10];

	k=0;
	do{
		for(i=0;i<10;i++)
			ch[i]=A[k].getvariable(i);
		if(ch[0]=='-'&&ch[1]=='-'&&ch[2]=='-'&&ch[3]=='-'&&ch[4]=='-'&&ch[5]=='-'&&ch[6]=='-'&&ch[7]=='-'&&ch[8]=='-'&&ch[9]=='-')
			break;
		else
			k++;
	}while(k <= 2000);

	return k;
}

int Minterm::mutiple2(int y)
{
	int i,s=1;
	for(i=0;i<y;i++)
		s=s*2;
	return s;
}

int Minterm::bleng()
{
	int i,s,y=0;

	for(i=0;i<10;i++)
		if(a[i]=='-') y++;//计算有多少个-

	s = mutiple2(y);//2的y次方
	return s;
}

void Minterm::setnumber()
{
	int N=bleng();int x=1;
	for(int i=0;i<N;i++)
		b[i]=0;
	for(int t=0;t<10;t++)
	{
		if(a[t]=='1')
			for(int l=0;l<N;l++)
				b[l]=b[l]+mutiple2(9-t);
		if(a[t]=='-')
		{
			for(int n=0;n<N;n++)
			{
				int m=mutiple2(x)*n;
				if(m<N)
				{
					for(int w=0;w<mutiple2(x-1);w++)
						b[m+w]=b[m+w]+mutiple2(9-t);
				}
			}
			x++;
		}
	}
}

int Minterm::getnumber(int i)
{
	return b[i];
}

void Minterm::cover()
{
	bcover = true;
}

bool Minterm::iscover()
{
	return  bcover;
}

int  Minterm::getonecount()
{
	int i,y=0;
	for(i=0;i<10;i++)
		if(a[i]=='1') y++;//计算有多少个1
	return y;
}
//两个最小项的own数组有相同点则返回true，并取得相同点数组
bool Minterm::isown(Minterm &A,Minterm &B,int *arry)
{
	int i;
	bool flg=false;

	for(i=0;i<ownc;i++)
	{
		arry[i]=0;
	}

	for(i=0;i<ownc;i++)
	{
		if((A.own[i]==B.own[i])&&(A.own[i]==1))
		{
			flg=true;
			arry[i]=1;
		}
	}

	return  flg;
}
//比较两个数组相同
bool Minterm::arryequ(int *arry1,int *arry2,int siz)
{
	int i;
	for(i=0;i<siz;i++)
	{
		if(arry1[i]!=arry2[i])
			return false;
	}
	return  true;
}
