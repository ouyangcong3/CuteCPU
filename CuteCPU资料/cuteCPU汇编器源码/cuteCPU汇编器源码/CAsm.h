#ifndef _ASM_H_
#define _ASM_H_

#include<vector>
#include <iostream>
using namespace std;
#include "qSTRING.h"

#define  PRINT_INFO  1
#define  OTRACE      printf

typedef struct tab_Instruction
{
    int    labelflg;
    string label;
    unsigned int op;
    string arg;
}Instruction_t;

typedef struct tab_Label
{
    string label;
    unsigned int addr;
}Label_t;

enum op_type{ opAdd,opRM,opAddc,opWM,opSub,opOut,opLt,opEq,opGoto,opQClr,opQMov,opQAdd,opQSub,opQGoto,opQJmp,opNop,opInvalid };

class CAsm
{

private:
	/* map to op_type */
	string  op_desc[17];
	FILE *fp_in;
	FILE *fp_out;

	vector < Instruction_t > i_mem;
	vector < Label_t >       lab_mem;
	vector < Label_t >       goto_mem;
	vector < Label_t >       qgoto_mem;

	Instruction_t   itmp;
	int             m_insc;
	unsigned char  *m_code;

	string  GetLabel(string str);
	string  GetOther(string str);
	string  GetOps(string str);
	string  GetArgs(string str);
	string  GetArg1(string str);
	string  GetArg2(string str);
	unsigned char Reg2int(string reg);
	unsigned char DoArgs(string args);

	void SaveLabel(vector<Instruction_t>::iterator it);
	void oAddfunc(vector<Instruction_t>::iterator it);
	void oRMfunc(vector<Instruction_t>::iterator it);
	void oAddcfunc(vector<Instruction_t>::iterator it);
	void oWMfunc(vector<Instruction_t>::iterator it);
	void oSubfunc(vector<Instruction_t>::iterator it);
	void oOutfunc(vector<Instruction_t>::iterator it);
	void oLtfunc(vector<Instruction_t>::iterator it);
	void oEqfunc(vector<Instruction_t>::iterator it);
	void oGotofunc(vector<Instruction_t>::iterator it);

	void DoGoto(unsigned int iAddr,unsigned int addr);
	void DoQFunc(unsigned char op,vector<Instruction_t>::iterator it);
	void DoQGoto(unsigned int iAddr,unsigned int addr);
	void qClrfunc(vector<Instruction_t>::iterator it);
	void qMovfunc(vector<Instruction_t>::iterator it);
	void qAddfunc(vector<Instruction_t>::iterator it);
	void qSubfunc(vector<Instruction_t>::iterator it);
	void qGotofunc(vector<Instruction_t>::iterator it);
	void qJmpfunc(vector<Instruction_t>::iterator it);
	void Nopfunc(vector<Instruction_t>::iterator it);

	unsigned int get_op( string s );
	int  get_operand_count( unsigned int op );

	void Compiling();
	void DoLabelAddr();
#if   PRINT_INFO == 1
	void GetInfo();
#endif
	void Init();
	void ReadLine(string line);
	bool ReadFile(string ff);

	void Output_Hardware_BinFile(string ff);

public:
	CAsm();
	~CAsm();

	int  getinsc(void){return m_insc;}
	void compile(string ff);
};

/////////////////////////////////////////////////////////////////////////////
#endif
