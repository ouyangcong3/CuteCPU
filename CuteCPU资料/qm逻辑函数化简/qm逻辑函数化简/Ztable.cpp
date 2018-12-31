/*************************************************
Copyright:
Author:qqlqql，QQ 631011506
Date:2016-12-18
Description:质蕴含表类
**************************************************/

#include<iostream>
#include "Ztable.h"

using namespace std;

Ztable::Ztable(Minterm *N,int *ee,int len)
{
    int i,j,k;
	row = Minterm::length(N);
	col = len;

	tee = ee;
	tlen = len;
	note=new int[row];
	notec=0;

	M = new int*[ownc];
	for(k=0;k<ownc;k++)
        M[k]=new int[row*col];

	for(k=0;k<ownc;k++)
	for(i=0;i<row;i++)
    for(j=0;j<col;j++)
        M[k][i*col+j]=0;

	//cout<<"row = "<<row;
	//cout<<",  col = "<<col<<endl;
    int m,n,b,x;
    bool flag;
	for(m=0;m<row;m++)
	{
		//cout<<"N["<<m<<"].bleng() = "<<N[m].bleng()<<endl;
		for(n=0;n<(N[m].bleng());n++)
		{
			b=N[m].getnumber(n);
            flag=false;
			for(x=0;x<col;x++)
			{
				if(ee[x]==b){//存在于输入的最小项里面，b的作用在这里
					flag=true;
					break;
				}
			}
			if(flag==true)
            {
                for(k=0;k<ownc;k++)
                {
                    if(N[m].own[k] == 1)
                        M[k][m*col+x]=1;
                }
			}
		}
	}
}

Ztable::~Ztable()
{
	for(int i=0; i<ownc;i++)
	{
		delete []M[i];
	}
	delete []M;
	delete []note;
}

void Ztable::print()
{
	int i,j,k,n;

	for(k=0;k<ownc;k++)
	{
	for(n=0;n<tlen;n++)
    {
		cout<<tee[n]<<" ";
    }
    cout<<endl<<"-------------------"<<endl;
		for(i=0;i<row;i++)
		{
			for(j=0;j<col;j++)
			{
				cout<<M[k][i*col+j]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	}
}
/*
int Ztable::getN(int a,int b)
{
	if((a<row)&&(b<col))
		return M[a*col+b];
	else
		return -1;
}

void Ztable::setN(int a,int b)
{
	if((a<row)&&(b<col))
		M[a*col+b]=1;
}
*/
int Ztable::getnotec()
{
	return notec;
}
/*
int Ztable::getnote(int a)
{
	if(a<notec)
		return note[a];
	else
		return -1;
}
*/
bool Ztable::Exist(int *arry,int len,int in)
{
    int i;
	for(i=0;i<len;i++)
	{
        if(arry[i]==in)  return true;
	}
	return false;
}

void Ztable::findessential()
{
	int i,j,k,n,s,temp,m1,m2;

	for(k=0;k<ownc;k++)
	{
		for(i=0;i<col;i++)
		{
			s=0; temp=0;
			for(j=0;j<row;j++)
			{
				if(M[k][j*col+i]==1)
				{s++;temp=j;}
			}
			if(s==1)
			{
			    if(!Exist(note,notec,temp))
                {
                    cout<<"note[notec]=temp : "<<temp<<endl;
                    note[notec]=temp;
                    notec++;
                }

				for(m1=0;m1<col;m1++)
				{
					//if(s==1)
					for(n=0;n<ownc;n++)
					if(M[n][temp*col+m1]==1)
					{
						for(m2=0;m2<row;m2++)
							M[n][m2*col+m1]=0;//删除temp行上1所在的全部列
					}
				}
			}
		}
	}
}

void Ztable::cutrow()
{
	int k,m;
	bool flag[ownc];
    bool flg;

    for(int i=0;i<row-1;i++)
    {
        for(int j=i+1;j<row;j++)
        {
            for(k=0;k<ownc;k++) flag[k]=false;
            flg = false;
            for(m=0;m<col;m++)
            {
                for(k=0;k<ownc;k++)
                {
                    if((M[k][i*col+m]==1)&&(M[k][j*col+m]==0))
                    {flag[k]=true;flg=true;break;}
                }
                if(flg==true) break;
            }
            flg = false;
            for(k=0;k<ownc;k++)
            {
                if(flag[k]==true){flg = true;break;}
            }
            if(flg == false)
            {
                for(m=0;m<col;m++)
                {
                    for(k=0;k<ownc;k++)  M[k][i*col+m]=0;
                }
            }
        }
    }

	for(int i=row-1;i>0;i--)
	{
		for(int j=i-1;j>=0;j--)
		{
            for(k=0;k<ownc;k++) flag[k]=false;
            flg = false;
            for(m=0;m<col;m++)
            {
                for(k=0;k<ownc;k++)
                {
                    if((M[k][i*col+m]==1)&&(M[k][j*col+m]==0))
                    {flag[k]=true; flg=true;break;}
                }
                if(flg==true) break;
            }
            flg = false;
            for(k=0;k<ownc;k++)
            {
                if(flag[k]==true){flg = true;break;}
            }
            if(flg == false)
            {
                for(m=0;m<col;m++)
                {
                    for(k=0;k<ownc;k++)  M[k][i*col+m]=0;
                }
            }
		}
	}
}

void Ztable::cutcol()
{
	int i,j,k,m;
	bool flag[ownc];
	bool flag2[ownc];
    bool flg;

	for(i=0;i<col-1;i++)
	{
	    for(k=0;k<ownc;k++) flag2[k]=false;

        for(int t1=0;t1<ownc;t1++)
            for(int t2=0;t2<row;t2++)
                if(M[t1][t2*col+i]!=0)
                {flag2[t1]=true;break;}
		for(j=i+1;j<col;j++)
		{
			for(k=0;k<ownc;k++) flag[k]=false;

			for(k=0;k<ownc;k++)
            {
                if(flag2[k]==true)
                for(m=0;m<row;m++)
                {
                    if((M[k][m*col+i]==1)&&(M[k][m*col+j]==0))
                    {flag[k]=true;break;}
                }
            }

            flg = false;
            for(k=0;k<ownc;k++)
            {
                if(flag[k]==true){flg = true;break;}
            }

            for(k=0;k<ownc;k++)
            {
                if((flag2[k]==true)&&(flg==false))
                {
                    for(m=0;m<row;m++)
                        M[k][m*col+j]=0;
                }
            }
		}
	}

	for(i=col-1;i>0;i--)
	{
		for(k=0;k<ownc;k++) flag2[k]=false;

        for(int t1=0;t1<ownc;t1++)
            for(int t2=0;t2<row;t2++)
                if(M[t1][t2*col+i]!=0)
                {flag2[t1]=true;break;}

		for(j=i-1;j>=0;j--)
		{
			for(k=0;k<ownc;k++) flag[k]=false;

			for(k=0;k<ownc;k++)
            {
                if(flag2[k]==true)
                for(m=0;m<row;m++)
                {
                    if((M[k][m*col+i]==1)&&(M[k][m*col+j]==0))
                    {flag[k]=true;break;}
                }
            }

            flg = false;
            for(k=0;k<ownc;k++)
            {
                if(flag[k]==true){flg = true;break;}
            }

            for(k=0;k<ownc;k++)
            {
                if((flag2[k]==true)&&(flg==false))
                {
                    for(m=0;m<row;m++)
                        M[k][m*col+j]=0;
                }
            }
		}
	}
}

bool Ztable::emptyM()
{
    int i,j,k;

    for(k=0;k<ownc;k++)
    for(i=0;i<row;i++)
    for(j=0;j<col;j++)
        if(M[k][i*col+j]!=0)
        return false;

    return true;
}

void Ztable::setFinal(int *A)
{
	bool flag;

	do{
		//flag=true;
		cutrow();
		//cout<<"================cutrow================== "<<endl;
		//print();
		cutcol();
		//cout<<"==================cutcol================ "<<endl;
		//print();
		findessential();
		//cout<<"=================findessential================= "<<endl;
        flag = emptyM();
        //print();
        //cout<<"===================print=============== "<<endl;
	}while(!flag);

	for(int i=0;i<notec;i++)
		A[i]=note[i];
}
