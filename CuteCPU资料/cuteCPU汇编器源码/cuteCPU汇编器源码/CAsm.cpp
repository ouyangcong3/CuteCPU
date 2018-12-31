#include "CAsm.h"
#include <stdio.h>
/***************************************************************/
/***************************************************************/

CAsm::CAsm()
{
	op_desc[0]="ADD";
	op_desc[1]="RM";
	op_desc[2]="ADDC";
	op_desc[3]="WM";
	op_desc[4]="SUB";
	op_desc[5]="OUT";
	op_desc[6]="<";
	op_desc[7]="=";
	op_desc[8]="GOTO";
	op_desc[9]="QCLR";
	op_desc[10]="QMOV";
	op_desc[11]="QADD";
	op_desc[12]="QSUB";
	op_desc[13]="QGOTO";
	op_desc[14]="QJMP";
	op_desc[15]="NOP";
	op_desc[16]="0";

	m_code = new unsigned char[4096];
}

/***************************************************************/
/***************************************************************/

CAsm::~CAsm()
{
	delete []m_code;
	i_mem.clear();
	lab_mem.clear();
	goto_mem.clear();
	qgoto_mem.clear();
}

/***************************************************************/
/***************************************************************/

/* get op from its string desc */
unsigned int CAsm::get_op( string s )
{
	unsigned int i;
	for(i=0; op_desc[i] != "0"; ++i )
	{
		if(op_desc[i] == s)
		{
			return i;
		}
	}
	return opInvalid;
}

/* get the op code arg(operand) count */
int CAsm::get_operand_count( unsigned int op )
{
	int ret;
	switch( op )
	{
		case opAdd:
		case opRM:
		case opAddc:
		case opWM:
		case opSub:
		case opOut:
		case opLt:
		case opEq:
		case opQMov:
		case opQAdd:
		case opQSub:
			ret = 2;
			break;
		case opGoto:
		case opQClr:
		case opQGoto:
		case opQJmp:
			ret = 1;
			break;
		default:
			ret = 0;
	}
	return ret;
}

/***************************************************************/
/***************************************************************/

string CAsm::GetLabel(string str)
{
	if(str.find(':') != -1)
		return qSTRING::leftc(str,':');
	else
		return "";
}

string CAsm::GetOther(string str)
{
	if(str.find(':') != -1)
		return qSTRING::rightc(str,':');
	else
		return "";
}

string CAsm::GetOps(string str)
{
	if(str.find(' ') != -1)
		return qSTRING::leftc(str,' ');
	else
		return str;
}

string CAsm::GetArgs(string str)
{
	if(str.find(' ') != -1)
		return qSTRING::rightc(str,' ');
	else
		return "";
}

string CAsm::GetArg1(string str)
{
	if(str.find(',') != -1)
		return qSTRING::leftc(str,',');
	else
		return "";
}

string CAsm::GetArg2(string str)
{
	if(str.find(',') != -1)
		return qSTRING::rightc(str,',');
	else
		return "";
}

/***************************************************************/
/***************************************************************/

unsigned char CAsm::Reg2int(string reg)
{
    reg = qSTRING::trim(reg);

	if(reg == "R0")  return 0x00;
	if(reg == "R1")  return 0x01;
	if(reg == "R2")  return 0x02;
	if(reg == "R3")  return 0x03;

	else  return 0x00;
}

/***************************************************************/
/***************************************************************/

unsigned char CAsm::DoArgs(string args)
{
	unsigned char code;
	string arg1,arg2;
	int tmp;

	code = 0x00;
	arg1 = GetArg1(args);
	arg2 = GetArg2(args);

	//OTRACE("%s ",args.c_str());
	//OTRACE("%s ",arg1.c_str());
	//OTRACE("%s\n",arg2.c_str());

	if(qSTRING::isnum(arg1))
	{
		code = 0x10;
		tmp = qSTRING::s2int(arg1);
		tmp = tmp%4;
		code = code + tmp*4;
	}
	else
	{
		code = Reg2int(arg1)*4;
	}

	tmp = Reg2int(arg2);
	code = code | tmp;

	return code;
}

/***************************************************************/
/***************************************************************/

void CAsm::DoGoto(unsigned int iAddr,unsigned int addr)
{
	m_code[iAddr] = addr;
}

/***************************************************************/
/***************************************************************/

void CAsm::DoQFunc(unsigned char op,vector<Instruction_t>::iterator it)
{
	unsigned char code;
	string arg1,arg2;
	unsigned char reg;
	int val,loopc,rest,i;

	code = 0x00;
	arg1 = GetArg1(it->arg);
	arg2 = GetArg2(it->arg);

	//OTRACE("%s ",args.c_str());
	//OTRACE("%s ",arg1.c_str());
	//OTRACE("%s\n",arg2.c_str());
	reg = Reg2int(arg2);

	if(qSTRING::isnum(arg1))
	{
		code = 0x10;
		val = qSTRING::s2int(arg1);

		if(val == 0)
		{
			code = op|0x10;
			code |= reg;
			m_code[m_insc] = code;
			m_insc++;
			return;
		}

		loopc = val/3;
		rest  = val%3;

		for(i=0;i<loopc;i++)
		{
			code = op|0x1C;
			code |= reg;
			m_code[m_insc] = code;
			m_insc++;
		}
		if(rest>0)
		{
			code = op|0x10;
			code |= rest*4|reg;
			m_code[m_insc] = code;
			m_insc++;
		}
	}
	else
	{
		code = op|Reg2int(arg1)*4;
		code |= reg;
		m_code[m_insc] = code;
		m_insc++;
	}
}

/***************************************************************/
/***************************************************************/

void CAsm::DoQGoto(unsigned int iAddr,unsigned int addr)
{
	unsigned char codebk;

	iAddr--;
	codebk = m_code[iAddr];//保存goto的上一条判断指令
	m_code[iAddr] = 0x8f;
	iAddr++;
///////////////////////////////////////
	unsigned int addr1,addr2;
	unsigned char code;
	unsigned char reg;
	int loopc,rest,i,c;

	reg = 3;
	addr1 = (addr>>8)&0x0f;
	loopc = addr1/3;
	rest  = addr1%3;
	c = 0;
	for(i=0;i<loopc;i++)
	{
		code = 0x1C;
		code |= reg;
		m_code[iAddr] = code;
		iAddr++;
		c++;
	}
	if(rest>0)
	{
		code = 0x10;
		code |= rest*4|reg;
		m_code[iAddr] = code;
		iAddr++;
		c++;
	}
	for(;c<5;c++)
	{
		m_code[iAddr] = 0x13;
		iAddr++;
	}
///////////////////////////////////////
	m_code[iAddr] = codebk;
	iAddr++;
	addr2 = addr&0xff;
	m_code[iAddr] = addr2;
	//iAddr++;
}

/***************************************************************/
/***************************************************************/

void CAsm::SaveLabel(vector<Instruction_t>::iterator it)
{
	Label_t  label;

	if(it->labelflg == 1)
	{
		label.label = it->label;
		label.addr = m_insc;
		lab_mem.push_back(label);
	}
}

/***************************************************************/
/***************************************************************/

void CAsm::oAddfunc(vector<Instruction_t>::iterator it)
{
	//OTRACE("Addfunc");
	unsigned char code;
	code = 0x00;
	SaveLabel(it);
	m_code[m_insc] = code | DoArgs(it->arg);
	m_insc++;
}
void CAsm::oRMfunc(vector<Instruction_t>::iterator it)
{
	//OTRACE("RMfunc");
	unsigned char code;
	code = 0x20;
	SaveLabel(it);
	m_code[m_insc] = code | DoArgs(it->arg);
	m_insc++;
}
void CAsm::oAddcfunc(vector<Instruction_t>::iterator it)
{
	//OTRACE("Addcfunc");
	unsigned char code;
	code = 0x40;
	SaveLabel(it);
	m_code[m_insc] = code | DoArgs(it->arg);
	m_insc++;
}
void CAsm::oWMfunc(vector<Instruction_t>::iterator it)
{
	//OTRACE("WMfunc");
	unsigned char code;
	code = 0x60;
	SaveLabel(it);
	m_code[m_insc] = code | DoArgs(it->arg);
	m_insc++;
}
void CAsm::oSubfunc(vector<Instruction_t>::iterator it)
{
	//OTRACE("Subfunc");
	unsigned char code;
	code = 0x80;
	SaveLabel(it);
	m_code[m_insc] = code | DoArgs(it->arg);
	m_insc++;
}
void CAsm::oOutfunc(vector<Instruction_t>::iterator it)
{
	//OTRACE("Outfunc");
	unsigned char code;
	code = 0xa0;
	SaveLabel(it);
	m_code[m_insc] = code | DoArgs(it->arg);
	m_insc++;
}
void CAsm::oLtfunc(vector<Instruction_t>::iterator it)
{
	//OTRACE("Ltfunc");
	unsigned char code;
	code = 0xc0;
	SaveLabel(it);
	m_code[m_insc] = code | DoArgs(it->arg);
	m_insc++;
}

void CAsm::oEqfunc(vector<Instruction_t>::iterator it)
{
	//OTRACE("Eqfunc");
	unsigned char code;
	code = 0xe0;
	SaveLabel(it);
	m_code[m_insc] = code | DoArgs(it->arg);
	m_insc++;
}

/***************************************************************/
/***************************************************************/

void CAsm::oGotofunc(vector<Instruction_t>::iterator it)
{
	//OTRACE("Gotofunc");
	string arg;
	unsigned int addr;
	Label_t gotov;

	SaveLabel(it);

	arg = it->arg;
	if(qSTRING::isnum(arg))
	{
		addr = qSTRING::s2int(arg);
		if(addr > 0xff)
		{
			//出错，提示地址超出255,
			m_code[0] = 0;
		}
		else
		{
			m_code[m_insc] = addr;
		}
	}
	else
	{
		gotov.label = arg;
		gotov.addr  = m_insc;//对应代码的地址
		goto_mem.push_back(gotov);
		m_code[m_insc] = 0;
	}
	/************************************/
	m_insc++;
}

/************************************************************************/
/* 自定义指令                                                           */
/************************************************************************/

void CAsm::qClrfunc(vector<Instruction_t>::iterator it)
{
	unsigned char code;
	unsigned char reg;

	code = 0x80;
	reg = Reg2int(it->arg);
	code |= reg*4;
	code |= reg;
	SaveLabel(it);
	m_code[m_insc] = code;
	m_insc++;
}

/***************************************************************/
/***************************************************************/

void CAsm::qMovfunc(vector<Instruction_t>::iterator it)
{
	unsigned char code;
	unsigned char reg;

	code = 0x80;
	reg = Reg2int(GetArg2(it->arg));
	code |= reg*4;
	code |= reg;

	SaveLabel(it);
	m_code[m_insc] = code;
	m_insc++;
	DoQFunc(0x00,it);
}

/***************************************************************/
/***************************************************************/

void CAsm::qAddfunc(vector<Instruction_t>::iterator it)
{
	SaveLabel(it);
	DoQFunc(0x00,it);
}

/***************************************************************/
/***************************************************************/

void CAsm::qSubfunc(vector<Instruction_t>::iterator it)
{
	SaveLabel(it);
	DoQFunc(0x80,it);
}

/***************************************************************/
/***************************************************************/

void CAsm::qGotofunc(vector<Instruction_t>::iterator it)
{
	//OTRACE("qGotofunc\n");
	string arg;
	unsigned int addr;
	Label_t gotov;

	SaveLabel(it);

	arg = it->arg;
	if(qSTRING::isnum(arg))
	{
		addr = qSTRING::s2int(arg);
		DoQGoto(m_insc,addr);
	}
	else
	{
		//OTRACE("push_back\n");
		gotov.label = arg;
		gotov.addr  = m_insc;//对应代码的地址
		qgoto_mem.push_back(gotov);
		m_code[m_insc] = 0;
	}
	m_insc += 7;
}

/***************************************************************/
/***************************************************************/

void CAsm::qJmpfunc(vector<Instruction_t>::iterator it)
{
	//OTRACE("qJmpfunc\n");
	string arg;
	unsigned int addr;
	Label_t gotov;

	SaveLabel(it);
	//加一条指令: < R0,R0，使JMP变成绝对跳转指令
	m_code[m_insc] = 0XC0;
	m_insc++;

	arg = it->arg;
	if(qSTRING::isnum(arg))
	{
		addr = qSTRING::s2int(arg);
		DoQGoto(m_insc,addr);
	}
	else
	{
		//OTRACE("push_back\n");
		gotov.label = arg;
		gotov.addr  = m_insc;//对应代码的地址
		qgoto_mem.push_back(gotov);
		m_code[m_insc] = 0;
	}
	m_insc += 7;
}

void CAsm::Nopfunc(vector<Instruction_t>::iterator it)
{
	//OTRACE("Nopfunc\n");
	///////////////////////////////////////
	SaveLabel(it);
	m_code[m_insc] = 0x10;
	m_insc++;
}

/***************************************************************/
/***************************************************************/
/***************************************************************/
/***************************************************************/

void CAsm::ReadLine(string line)
{
	string  strline,label;
	unsigned int op;
	int arg_c;
	bool flg;

	strline = qSTRING::trim(line);

	//if( line.c_str()[0] == '\0')
	if( line == "")
		return;//continue;
	if( line.c_str()[0] == '#')
		return;//continue;
	if((line.length() >= 2)&&(line.c_str()[0] == '/')&&(line.c_str()[1] == '/'))
		return;//continue;

	//qqlqql OTRACE("%s\n",line.c_str());

	if(line.find("//") != -1)
		line = qSTRING::lefts(line,"//");

	strline = qSTRING::toupper(line);
	label = GetLabel(strline);

	if(label != "")
	{
		flg = false;
		vector<Instruction_t>::iterator it;
		for(it=i_mem.begin();it!=i_mem.end();it++)
		{
			if(it->label == label) {flg = true; break;}
		}
		if(!flg)
		{
			itmp.labelflg = 1;
			itmp.label = label;
		}else{
			//标号已经存在，出错
		}

		strline = GetOther(strline);

		if(strline == "")
			return;//continue;
	}

	op = get_op( GetOps(strline) );
	arg_c = get_operand_count( op );

	if( arg_c > 0 )
	{
		strline = GetArgs(strline);
	}
	else
	{
		strline = "";
	}

	itmp.op = op;
	itmp.arg = strline;
	i_mem.push_back(itmp);

	itmp.labelflg = 0;
	itmp.label = "";
}

/***************************************************************/
/***************************************************************/

bool CAsm::ReadFile(string ff)
{
    //char f[]="./sourcefile.a";
    //fp_in = fopen( f, "r" );
    fp_in = fopen( ff.c_str(), "r" );
    if( fp_in == 0 )
    {
        cout << "Open file failed!" << endl;
        return false;
    }

	char line[256];
	while( !feof( fp_in ) )
	{
		fgets( line, sizeof( line ) - 1, fp_in );
        ReadLine(line);
	}
	fclose( fp_in );
}

/***************************************************************/
/***************************************************************/

void CAsm::Compiling()
{
	unsigned int op;

	//typename std::  iterator
    vector<Instruction_t>::iterator it;
	for(it=i_mem.begin();it!=i_mem.end();it++)
	{
		op  = it->op;
		switch( op )
		{
			case opAdd:
				oAddfunc(it);break;
			case opRM:
				oRMfunc(it);break;
			case opAddc:
				oAddcfunc(it);break;
			case opWM:
				oWMfunc(it);break;
			case opSub:
				oSubfunc(it);break;
			case opOut:
				oOutfunc(it);break;
			case opLt:
				oLtfunc(it);break;
			case opEq:
				oEqfunc(it);break;
			case opGoto:
				oGotofunc(it);break;

			case opQClr:
				qClrfunc(it);break;
			case opQMov:
				qMovfunc(it);break;
			case opQAdd:
				qAddfunc(it);break;
			case opQSub:
				qSubfunc(it);break;
			case opQGoto:
				qGotofunc(it);break;
			case opQJmp:
				qJmpfunc(it);break;
			case opNop:
				Nopfunc(it);break;
			default:
				break;
		}
	}
}

/***************************************************************/
/***************************************************************/

void CAsm::Output_Hardware_BinFile(string ff)
{
	fp_out = fopen( ff.c_str(), "wb" );
	if( fp_out == 0 )
	{
		//fprintf( stderr, "Open %s failed\n", output );
		//qqlqql OTRACE( "Open output file failed\n");
		return;
	}

	for(int i=0;i<m_insc;i++)
	{
		fwrite( &m_code[i], sizeof( char ), 1, fp_out );
	}
	fclose( fp_out );
}

/***************************************************************/
/***************************************************************/

void CAsm::DoLabelAddr()
{
	bool find;
	vector<Label_t>::iterator itg,itl;

	for(itg=goto_mem.begin();itg!=goto_mem.end();itg++)
	{
		find = false;
		for(itl=lab_mem.begin();itl!=lab_mem.end();itl++)
		{
			//OTRACE(" ============遍历============\n");
			//OTRACE(" %d,%s  %d,%s\n",itg->addr,itg->label.c_str(),itl->addr,itl->label.c_str());
			if(itg->label == itl->label){
				find = true; break;
			}
		}
		if(find)
		{
			if(itl->addr > 255)
			{
				//出错，跳转的标号地址大于255
				//qqlqql OTRACE(" 出错，跳转的标号地址大于255\n");
			}else{
#if   PRINT_INFO == 1
				OTRACE(" ============找到============\n");
				OTRACE(" %d,%s  %d,%s\n",itg->addr,itg->label.c_str(),itl->addr,itl->label.c_str());
#endif
				DoGoto(itg->addr,itl->addr);
			}
		}else{
			//出错，跳转的标号找不到
		}
	}
	///////////////////////////////////////////////////////
	for(itg=qgoto_mem.begin();itg!=qgoto_mem.end();itg++)
	{
		find = false;
		for(itl=lab_mem.begin();itl!=lab_mem.end();itl++)
		{
			//OTRACE(" ============遍历============\n");
			//OTRACE(" %d, %d\n",itg->addr,itl->addr);
			if(itg->label == itl->label){
				find = true; break;
			}
		}
		if(find)
		{
#if   PRINT_INFO == 1
			OTRACE(" ============找到============\n");
			OTRACE(" %d,%s  %d,%s\n",itg->addr,itg->label.c_str(),itl->addr,itl->label.c_str());
#endif
			DoQGoto(itg->addr,itl->addr);
		}else{
			//出错，跳转的标号找不到
		}
	}
}

/***************************************************************/
/***************************************************************/

void CAsm::Init()
{
	m_insc = 0;
	i_mem.clear();
	lab_mem.clear();
	goto_mem.clear();
	qgoto_mem.clear();

	itmp.labelflg = 0;
	itmp.label = "";
}

/***************************************************************/
/***************************************************************/
#if   PRINT_INFO == 1
void CAsm::GetInfo()
{
	OTRACE("源码行数：itmp.size() = %d\n",i_mem.size());
	OTRACE("最终指令行数：m_insc = %d\n",m_insc);
	OTRACE("lab   = %d\n",lab_mem.size());
	OTRACE("goto  = %d\n",goto_mem.size());
	OTRACE("qgoto = %d\n",qgoto_mem.size());
}
#endif
/***************************************************************/
/***************************************************************/

void CAsm::compile(string ff)
{
	Init();
	ReadFile(ff);
	Compiling();
	DoLabelAddr();
#if   PRINT_INFO == 1
	GetInfo();
#endif
	int pos;
	pos = ff.rfind('.');
	ff = qSTRING::left(ff,pos);
	ff += ".bin";
	Output_Hardware_BinFile(ff);
}
