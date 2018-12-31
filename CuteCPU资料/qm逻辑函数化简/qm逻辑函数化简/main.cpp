/*************************************************
Copyright:
Author:qqlqql，QQ 631011506
Date:2016-12-18
Description:main主函数
**************************************************/

#include<iostream>
#include "Minterm.h"
#include "Ztable.h"
#include "time.h"
using namespace std;

Minterm *connectTwo(int *,int *,int **,int *,int);
void       compareSingle(Minterm *,Minterm *,Minterm *);
Minterm *compareMutiple(Minterm *);
Minterm *compareFinal(Minterm *);
bool       Exist(Minterm *,Minterm *);
void       cutdontcare(Minterm *,int *,int);


/**
函数作用：连接输入的最小项和无关项到 Minterm 数组
*/
Minterm *connectTwo(int *tmp,int *len,int **eem,int *lenary,int outlen)
{
	int i,j,n,k=0,alen=0;
	bool flg;

	for(i=0;i<outlen;i++)
        alen += lenary[i];
    alen++;
	Minterm *Two= new Minterm[alen];

	for(i=0;i<alen;i++)
	{
		tmp[i] = -1;
	}
	for(i=0;i<lenary[0];i++)
	{
		tmp[i]=eem[0][i];
		Two[k]=eem[0][i];//是一个运算符重载
		Two[k].own[0] = 1;
		k++;
	}

    for(n=1;n<outlen;n++)
	{
        for(i=0;i<lenary[n];i++)
        {
            flg = false;
            for(j=0;j<alen;j++)
            {
                if(tmp[j] == -1) break;
                if(tmp[j] == eem[n][i])
                {
                    flg = true; break;
                }
            }

            if(flg){
                Two[j].own[n] = 1;
            }else{
                tmp[j]=eem[n][i];
                Two[k]=eem[n][i];//是一个运算符重载
                Two[k].own[n] = 1;
                k++;
            }
        }
	}

	//ee = &tmp[0];//要记得清空ee
	*len = k;
/*===========================*/
	for(i=0;i<k;i++){Two[i].print();cout<<endl;}
	return Two;
}

/**
函数作用：合并只有一处区别的最小项到另一个 Minterm 数组
这个函数前已经保证不同之处都是1,0
*/
void compareSingle(Minterm *A,Minterm *B,Minterm *C)
{
	int i,index;
	char a,b;

	for(index=0;index<10;index++)
	{
		a=A->getvariable(index);
		b=B->getvariable(index);
		if(a!=b)
			break;
	}
	C->setvariable(index,'-');
	for(i=0;i<10;i++)
	{
		if(i != index)//不是t位置的才拷贝
		{
			C->setvariable(i,A->getvariable(i));
		}
	}

	int *own = new int[ownc];
	Minterm::isown(*A,*B,own);
	for(i=0;i<ownc;i++)
	{
		C->own[i]=own[i];
	}
	delete []own;
}

int **sortminterm(Minterm *A)
{
	int i,j,len,tmp;
	int **m = new int*[XC + 1];
    len = Minterm::length(A);

	for(i=0; i<=XC;i++)
	{
		m[i] = new int[len];
		for(j=0; j<len;j++)
		{
			m[i][j] = -1;
		}
	}

	for(i=0;i<len;i++)
	{
		tmp = A[i].getonecount();
		for(j=0; j<len;j++)
		{
			if(m[tmp][j] == -1)
			{
				m[tmp][j] = i;
				break;
			}
		}
	}

	return m;
}

/**
函数作用：比较一个 Minterm 数组，把结果复制到另外一个 Minterm 数组
*/
Minterm *compareMutiple(Minterm *A)
{
	int i,j,k,n1,n2,t=0;
	int len = Minterm::length(A);

    Minterm *N    = new Minterm[2*len];//应该不会超过2倍长度
	Minterm *Temp = new Minterm;

	cout<<endl;
	cout<<"-------------------------len= "<<len<<" -------------------------"<<endl;

	int **m = sortminterm(A);

	int *own = new int[ownc];

	for(i=0; i<XC;i++)//到倒数第二个就可以
	{
		if(m[i][0] == -1) continue;
		if(m[i+1][0] == -1) {i++; continue;}

		for(j=0;j<len;j++)
		{
			if(m[i][j]==-1) break;
			for(k=0;k<len;k++)
			{
				if(m[i+1][k]==-1) break;
				n1 = m[i][j];
				n2 = m[i+1][k];
				if((A[n1]==A[n2])&&(Minterm::isown(A[n1],A[n2],own)))
				{
					cout<<n1<<"+"<<n2<<" "<<endl;
					A[n1].print();cout<<endl;
					A[n2].print();cout<<endl;
					if(Minterm::arryequ(own,A[n1].own,ownc)) A[n1].cover();
					if(Minterm::arryequ(own,A[n2].own,ownc)) A[n2].cover();
					compareSingle(&A[n1],&A[n2],Temp);
					Temp->print();cout<<" --------合并到Temp"<<endl;
					if(!Exist(N,Temp))//检查是否结果已经存在，防止重复的结果
					{
						N[t] = *Temp;
						N[t].print();cout<<" --------拷贝到N["<<t<<"]"<<endl;
						t++;
					}
				}
			}
		}
	}

	for(i=0; i<=XC;i++)
	{
		delete []m[i];
	}
	delete []m;
	delete []own;
    delete Temp;
	if(Minterm::length(N) == 0)
	{
		delete []N;
		return NULL;
	}
	return N;
}


Minterm *compareFinal(Minterm *A)
{
	int i;
	Minterm **m = new Minterm*[XC + 1];
	for(i=0; i<=XC;i++)
	{
		m[i] = NULL;
	}

	i = 0;
	m[i] = A;
	Minterm *f;
	do{
		f = compareMutiple(A);
		if(f == NULL){
			break;
		}else{
			i++;
			if(i>XC) break;
			m[i] = f;
			A = f;
		}
	}while(1);
/////////////////////////////////////
	int j,len,t;
	t = 0;
	len = Minterm::length(m[0]);
	f   = new Minterm[(XC+1)*len];//不会超过5倍长度
	for(i=0; i<=XC&&m[i]!=NULL;i++)
	{
		len = Minterm::length(m[i]);
		for(j=0;j<len;j++)
		{
			if(m[i][j].iscover() == false)
			{
				f[t] = m[i][j];
				f[t].print();cout<<" --------拷贝到ffff["<<t<<"]"<<endl;
				t++;
			}
		}
	}

	for(i=0; i<=XC;i++)
	{
		delete []m[i];
	}
	delete m;
	return f;
}

bool Exist(Minterm *N,Minterm *A)
{
	int i,j,len;
	bool fg,flag;
	char ch1,ch2;

	len = Minterm::length(N);
	flag = false;
	for(i=0;i<len;i++)
	{
		fg = true;
		for(j=0;j<10;j++)
		{
			ch1 = N[i].getvariable(j);
			ch2 = A->getvariable(j);
			if(ch1!=ch2)
				fg=false;
			else{
				if(!Minterm::arryequ(N[i].own,A->own,ownc))
					fg=false;
			}
		}
		if(fg==true)
		{
			flag = true; break;
		}
	}
	return flag;
}


void cutdontcare(Minterm *N,int ee[],int Nofe)
{
	int  i,j,x,len;
	bool flag;

	//cout<<"length(N)="<<Minterm::length(N)<<" "<<endl;
	len = Minterm::length(N);

	for(i=0;i<len;i++)
	{
		flag=false;
		int bl=N[i].bleng();
		j=0;
		//cout<<"N["<<i<<"].bleng()="<<N[i].bleng()<<" "<<endl;

		while((j<bl)&&(flag==false))
		{
			for(int n=0;n<Nofe;n++)
				if(N[i].getnumber(j)==ee[n])
				{
					flag=true; break;
				}
			j++;
		}
		if(flag==false)//当前项是无关项
		{
			for(x=i;x<len-1;x++)
				N[x]=N[x+1];
			N[len-1].setdefault();
			i--;
		}
		len = Minterm::length(N);
	}
}


int main()
{
	//int x;
	//x = 4;

/*
	//变量数是4
	int lenary[]={8,4,8};//长度数组
	int ee1[]={0,8,4,12,14,9,13,11};//按照书本的顺序
	int ee2[]={12,10,9,11};//按照书本的顺序
	int ee3[]={0,8,4,12,9,11,7,15};//按照书本的顺序

	//变量数是4
	int lenary[]={7,4,7};//长度数组
	int ee1[]={10,14,1,9,5,13,11};//按照书本的顺序
	int ee2[]={8,14,13,15};//按照书本的顺序
	int ee3[]={8,6,14,1,9,5,13};//按照书本的顺序*/
	/*
	//变量数是4测试通过
	int lenary[]={7,4,7};//长度数组
	int ee1[]={5,7,8,9,10,11,13};//按照书本的顺序
	int ee2[]={1,7,11,15};//按照书本的顺序
	int ee3[]={1,6,7,8,9,10,11};//按照书本的顺序
*/
    /*
	//变量数是3测试通过
	int lenary[]={6,5,4};//长度数组
	int ee1[]={0,1,2,4,5,6};//按照书本的顺序
	int ee2[]={1,2,3,5,6};//按照书本的顺序
	int ee3[]={1,3,5,6};//按照书本的顺序
*/
	/**/
	//变量数是4测试通过
	int lenary[]={8,9,7};//长度数组
	int ee1[]={2,6,7,8,10,12,14,15};//按照书本的顺序
	int ee2[]={5,8,9,10,11,12,13,14,15};//按照书本的顺序
	int ee3[]={2,6,7,9,11,13,15};//按照书本的顺序

	//cout<<"=============================================="<<endl;

	clock_t start,finish;
	double  duration;
	start = clock();

	cout<<"======================begin========================"<<endl;

	int outlen =sizeof(lenary)/sizeof(lenary[0]);
    int **eem = new int*[outlen];
    eem[0] = ee1;
    eem[1] = ee2;
    eem[2] = ee3;

	Minterm *Two,*f;
	int  i,elen;
	elen = 0;
	for(i=0;i<outlen;i++)
        elen += lenary[i];
	int  *ee = new int[elen];
	int  len;
	Two = connectTwo(ee,&len,eem,lenary,outlen);
	cout<<"outlen : "<<outlen<<endl;
	cout<<"len : "<<len<<endl;
	for(int i=0;i<len;i++)
    {
		cout<<ee[i]<<endl;
    }
	f=compareFinal(Two);

    cout<<endl;
	cout<<"=================第一步得到的蕴含项================= "<<endl;
	for(int i=0;i<Minterm::length(f);i++)
    {
		f[i].print();
		cout<<endl;
    }
/*
	cout<<endl;
	cout<<"===================生成质蕴含表==================="<<endl;
	int ED[]={0,1,7,8,10,13,14,3,9,12,15};
	Ztable ttt(f,ED,NofE+NofD);
	ttt.print();
*/
	cout<<endl;
	cout<<"==================删除无关项===================== "<<endl;
	//cutdontcare(f,ee,len);
/*
	cout<<endl;
	cout<<"===================删除无关项后的质蕴含表==================="<<endl;
	Ztable ttt2(f,E,NofE);
	ttt2.print();
*/
	int *Mark = new int[100];
	cout<<"len : "<<len<<endl;

	Ztable table(f,ee,len);
	table.print();

	table.setFinal(Mark);
    int notec=table.getnotec();
/*
    cout<<endl;
	cout<<"=================第二步得到的蕴含项================= "<<endl;
	for(int i=0;i<notec;i++)
    {
		f[Mark[i]].print();
		cout<<endl;
    }

	cout<<endl;
	cout<<"===================生成质蕴含表==================="<<endl;
	table.print();
*/
    int  val,gate1,gate2,gate3;
    gate2 = notec;
    gate1=0;
    gate3=0;
    int mius[10];
    for(i=0;i<10;i++) mius[i]=0;

    cout<<endl;
	cout<<"================================================= "<<endl;
	cout<<"化简结果为 :  ";
    char a[10];
	for(int q=0;q<notec;q++)
	{
		for(i=0;i<10;i++)
			a[i]=f[Mark[q]].getvariable(i);
		//for(int j=x;j>0;j--)
			//cout<<a[10-j];
        val=0;
		for(int j=XC;j>0;j--)
        {
			if(a[10-j]=='1')
            {
                val++; gate1++; cout<<char('A'+XC-j);
            }
			else if(a[10-j]=='0')
            {
                mius[10-j]=1;
                val++; gate1++; cout<<char('A'+XC-j)<<"'";
            }
        }
        if(val==1) gate1--;

		if(q!=notec-1)  cout<<" + ";
	}
	cout<<endl;
    for(i=0;i<10;i++) gate3 += mius[i];

    cout<<"门成本："<<gate1<<"+"<<gate2<<"+"<<gate3<<"="<<gate1+gate1+gate1<<endl<<endl;

	delete []ee;
	//delete []D;
	//delete []Two;
	delete []f;
	delete []Mark;

	finish = clock();
	duration = (double)(finish-start)/CLOCKS_PER_SEC;
	cout<<duration<<endl;
	return 0;
}








