/* Semantics.c
   Support and semantic action routines.
   
*/

#include <strings.h>
#include <stdlib.h>

#include "CodeGen.h"
#include "Semantics.h"
#include "SymTab.h"
#include "IOMngr.h"

extern struct SymTab *table;

/* Semantics support routines */

struct ExprRes *  doIntLit(char * digits)  { 

   struct ExprRes *res;
  
  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  res->Reg = AvailTmpReg();
  res->Instrs = GenInstr(NULL,"li",TmpRegName(res->Reg),digits,NULL);

  return res;
}

struct ExprRes *  doRval(char * name)  {

   struct ExprRes *res;
  
   if (!FindName(table, name)) {
		WriteIndicator(GetCurrentColumn());
		WriteMessage("Undeclared variable");
   }
  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  res->Reg = AvailTmpReg();
  res->Instrs = GenInstr(NULL,"lw",TmpRegName(res->Reg),name,NULL);

  return res;
}

struct ExprRes *  doAdd(struct ExprRes * Res1, struct ExprRes * Res2)  { 

   int reg;
   
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"add",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doSub(struct ExprRes * Res1, struct ExprRes * Res2)  {
	
	int reg;
	
	reg = AvailTmpReg();
	AppendSeq(Res1->Instrs,Res2->Instrs);
	AppendSeq(Res1->Instrs,GenInstr(NULL,"sub",
									TmpRegName(reg),
									TmpRegName(Res1->Reg),
									TmpRegName(Res2->Reg)));
	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(Res2->Reg);
	Res1->Reg = reg;
	free(Res2);
	return Res1;
}

struct ExprRes *  doNeg(struct ExprRes * Res1)  {
	
	int reg;
	
	reg = AvailTmpReg();
	AppendSeq(Res1->Instrs,GenInstr(NULL,"neg",
									TmpRegName(reg),
									TmpRegName(Res1->Reg),
									NULL));
	ReleaseTmpReg(Res1->Reg);
	Res1->Reg = reg;
	return Res1;
}

struct ExprRes *  doMult(struct ExprRes * Res1, struct ExprRes * Res2)  {
	
	int reg;
	
	reg = AvailTmpReg();
	AppendSeq(Res1->Instrs,Res2->Instrs);
	AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",
									TmpRegName(reg),
									TmpRegName(Res1->Reg),
									TmpRegName(Res2->Reg)));
	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(Res2->Reg);
	Res1->Reg = reg;
	free(Res2);
	return Res1;
}

struct ExprRes *  doDiv(struct ExprRes * Res1, struct ExprRes * Res2)  {
	
	int reg;
	
	reg = AvailTmpReg();
	AppendSeq(Res1->Instrs,Res2->Instrs);
	AppendSeq(Res1->Instrs,GenInstr(NULL,"div",
									TmpRegName(reg),
									TmpRegName(Res1->Reg),
									TmpRegName(Res2->Reg)));
	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(Res2->Reg);
	Res1->Reg = reg;
	free(Res2);
	return Res1;
}

struct ExprRes *  doMod(struct ExprRes * Res1, struct ExprRes * Res2)  {
	
	int reg;
	
	reg = AvailTmpReg();
	AppendSeq(Res1->Instrs,Res2->Instrs);
	AppendSeq(Res1->Instrs,GenInstr(NULL,"div",
									TmpRegName(Res1->Reg),
									TmpRegName(Res2->Reg),
									NULL));
	AppendSeq(Res1->Instrs,GenInstr(NULL,"mfhi",
									TmpRegName(reg),
									NULL,NULL));
	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(Res2->Reg);
	Res1->Reg = reg;
	free(Res2);
	return Res1;
}

struct InstrSeq * doPrint(struct ExprRes * Expr) {

  struct InstrSeq *code;
	
  code = Expr->Instrs;
  
    AppendSeq(code,GenInstr(NULL,"li","$v0","1",NULL));
    AppendSeq(code,GenInstr(NULL,"move","$a0",TmpRegName(Expr->Reg),NULL));
    AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    ReleaseTmpReg(Expr->Reg);
    free(Expr);

  return code;
}

struct InstrSeq * doPrintSp(struct ExprRes * Expr) {
	
	struct InstrSeq *code;
	
		int a0, v0, t0, t1;
	a0 = AvailTmpReg();
	v0 = AvailTmpReg();
	t0 = AvailTmpReg();
	t1 = AvailTmpReg();
	
	code = Expr->Instrs;
	
	// load n
	AppendSeq(code, GenInstr(NULL, "move", TmpRegName(a0), TmpRegName(Expr->Reg), NULL));
	// set constant 0 and 1
	AppendSeq(code, GenInstr(NULL, "li", TmpRegName(t0), "0", NULL));
		AppendSeq(code, GenInstr(NULL, "li", TmpRegName(t1), "1", NULL));
	
	char * start = GenLabel();
	char * repeat = GenLabel();
	char * end = GenLabel();
	// start:
	AppendSeq(code, GenInstr(start, NULL, NULL, NULL, NULL));
	// a0 = a0 - 1;
	AppendSeq(code,GenInstr(NULL,"sub",TmpRegName(a0),TmpRegName(a0),TmpRegName(t1)));
	// if (a0 < 0) v0 = 1 else v0 = 0;
	AppendSeq(code,GenInstr(NULL,"slt",TmpRegName(v0),TmpRegName(a0),TmpRegName(t0)));
	// if v0 = 0 then goto repeat
	AppendSeq(code, GenInstr(NULL, "beq", TmpRegName(v0), TmpRegName(t0), repeat));
	// if v0 != 0 then goto end
	AppendSeq(code, GenInstr(NULL, "beq", TmpRegName(t0), TmpRegName(t0), end));
	// repeat:
	AppendSeq(code, GenInstr(repeat, NULL, NULL, NULL, NULL));
	// print a space
	AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
	AppendSeq(code,GenInstr(NULL,"la","$a0","_sp",NULL));
	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
	// goto start
	AppendSeq(code, GenInstr(NULL, "beq", TmpRegName(t0), TmpRegName(t0), start));
	// end:
	AppendSeq(code, GenInstr(end, NULL, NULL, NULL, NULL));
	ReleaseTmpReg(Expr->Reg);
	ReleaseTmpReg(a0);
	ReleaseTmpReg(v0);
	ReleaseTmpReg(t0);
	ReleaseTmpReg(t1);
	free(Expr);
	
	return code;
}

struct InstrSeq * doPrintLn() {
	
	struct InstrSeq *code;
	
	code = GenInstr(NULL,"li","$v0","4",NULL);
	AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
	
	return code;
}

struct InstrSeq * doPrintStr(char * str) {
	struct InstrSeq *code;
	char * label = GenLabel();
	
	code = GenInstr(NULL, ".data", NULL, NULL, NULL);
	AppendSeq(code, GenInstr(label, ".asciiz", str, NULL, NULL));
	AppendSeq(code, GenInstr(NULL, ".text", NULL, NULL, NULL));
	AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
	AppendSeq(code,GenInstr(NULL,"la","$a0",label,NULL));
	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

	free(str);
	
	return code;
}

char * doStrLit(char * str) {
	return strdup(str);
}

struct InstrSeq * doAssign(char *name, struct ExprRes * Expr) {

  struct InstrSeq *code;
  

   if (!FindName(table, name)) {
		WriteIndicator(GetCurrentColumn());
		WriteMessage("Undeclared variable");
   }

  code = Expr->Instrs;
  
  AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(Expr->Reg), name,NULL));

  ReleaseTmpReg(Expr->Reg);
  free(Expr);
  
  return code;
}

extern struct BExprRes * doBExpr(struct ExprRes * Res1,  struct ExprRes * Res2) {
	struct BExprRes * bRes;
	AppendSeq(Res1->Instrs, Res2->Instrs);
 	bRes = (struct BExprRes *) malloc(sizeof(struct BExprRes));
	bRes->Label = GenLabel();
	AppendSeq(Res1->Instrs, GenInstr(NULL, "bne", TmpRegName(Res1->Reg), TmpRegName(Res2->Reg), bRes->Label));
	bRes->Instrs = Res1->Instrs;
	ReleaseTmpReg(Res1->Reg);
  	ReleaseTmpReg(Res2->Reg);
	free(Res1);
	free(Res2);
	return bRes;
}

extern struct InstrSeq * doIf(struct BExprRes * bRes, struct InstrSeq * seq) {
	struct InstrSeq * seq2;
	seq2 = AppendSeq(bRes->Instrs, seq);
	AppendSeq(seq2, GenInstr(bRes->Label, NULL, NULL, NULL, NULL));
	free(bRes);
	return seq2;
}

/*

extern struct InstrSeq * doIf(struct ExprRes *res1, struct ExprRes *res2, struct InstrSeq * seq) {
	struct InstrSeq *seq2;
	char * label;
	label = GenLabel();
	AppendSeq(res1->Instrs, res2->Instrs);
	AppendSeq(res1->Instrs, GenInstr(NULL, "bne", TmpRegName(res1->Reg), TmpRegName(res2->Reg), label));
	seq2 = AppendSeq(res1->Instrs, seq);
	AppendSeq(seq2, GenInstr(label, NULL, NULL, NULL, NULL));
	ReleaseTmpReg(res1->Reg);
  	ReleaseTmpReg(res2->Reg);
	free(res1);
	free(res2);
	return seq2;
}

*/
void							 
Finish(struct InstrSeq *Code)
{ struct InstrSeq *code;
  struct SymEntry *entry;
  struct Attr * attr;


  code = GenInstr(NULL,".text",NULL,NULL,NULL);
  AppendSeq(code,GenInstr(NULL,".align","2",NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".globl","main",NULL,NULL));
  AppendSeq(code, GenInstr("main",NULL,NULL,NULL,NULL));
  AppendSeq(code,Code);
  AppendSeq(code, GenInstr(NULL, "li", "$v0", "10", NULL)); 
  AppendSeq(code, GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".data",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".align","4",NULL,NULL));
  AppendSeq(code,GenInstr("_nl",".asciiz","\"\\n\"",NULL,NULL));
	AppendSeq(code,GenInstr("_sp",".asciiz","\" \"",NULL,NULL));

 entry = FirstEntry(table);
 while (entry) {
	AppendSeq(code,GenInstr((char *) GetName(entry),".word","0",NULL,NULL));
        entry = NextEntry(table, entry);
 }
  
  WriteSeq(code);
  
  return;
}




