/*************************************************
Copyright:
Author:qqlqql，QQ 631011506
Date:2016-12-18
Description:质蕴含表类
**************************************************/

#ifndef _ZTABLE_H_
#define _ZTABLE_H_

#include "Minterm.h"

class Ztable
{
public:
	Ztable(Minterm *,int *,int);
	~Ztable();

	//void setN(int,int);
	//int getN(int,int);
	int getnotec();
	//int getnote(int);

	void print();

	bool Exist(int *arry,int len,int in);
	void findessential();
	void cutrow();
	void cutcol();
	bool emptyM();
    void setFinal(int *);
private:
	int **M;
	int col;
	int row;

	int *note;
	int notec;

	int  *tee;
	int  tlen;
};

#endif
