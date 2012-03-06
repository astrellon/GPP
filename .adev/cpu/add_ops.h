#pragma once

#include <vector>
using std::vector;

#include "op_codes.h"
#include "cpu.h"

#include "op_mov.h"
#include "op_print.h"
#include "op_add.h"
#include "op_sub.h"
#include "op_jmp.h"
#include "op_cmp.h"
#include "op_misc.h"
#include "op_and.h"
#include "op_or.h"
#include "op_xor.h"
#include "op_shftl.h"
#include "op_shftr.h"


vector<OpClass*> allOpClasses;

void addOp(OpClass *op) {
	allOpClasses.push_back(op);
}

void removeAllOpClasses() {
	while(allOpClasses.size() > 0) {
		OpClass *op = allOpClasses.back();
		allOpClasses.pop_back();
		delete op;
	}
}

void addAllOps() {
	// MISC
	addOp(new OpNop());
	addOp(new OpCallC());
	addOp(new OpReturn());
	addOp(new OpEnd());
	addOp(new OpPushC());
	addOp(new OpPushR());
	addOp(new OpPopR());
	addOp(new OpMemR());
	addOp(new OpIncR());
	addOp(new OpDecR());
	// MOV
	addOp(new OpMovRC());
	addOp(new OpMovRR());
	// MEM MOV
	addOp(new OpMovRMC());
	addOp(new OpMovRMR());
	addOp(new OpMovMRR());
	addOp(new OpMovMRC());
	addOp(new OpMovMCR());
	addOp(new OpMovMCC());
	// PRINT
	addOp(new OpPrintC());
	addOp(new OpPrintR());
	addOp(new OpPrintMC());
	addOp(new OpPrintMR());
	addOp(new OpPrintC_C());
	addOp(new OpPrintC_R());
	addOp(new OpPrintC_MC());
	addOp(new OpPrintC_MR());
	// ADD
	addOp(new OpAddRC());
	addOp(new OpAddRR());
	addOp(new OpAddRRR());
	addOp(new OpAddRCR());
	addOp(new OpAddRRC());
	// SUB
	addOp(new OpSubRC());
	addOp(new OpSubRR());
	addOp(new OpSubRRR());
	addOp(new OpSubRCR());
	addOp(new OpSubRRC());
	// JMP
	addOp(new OpJmpC());
	addOp(new OpJmpR());
	// JEQ
	addOp(new OpJeqC());
	addOp(new OpJeqR());
	// JNEQ
	addOp(new OpJneqC());
	addOp(new OpJneqR());
	// JLT
	addOp(new OpJltC());
	addOp(new OpJltR());
	// JLTE
	addOp(new OpJlteC());
	addOp(new OpJlteR());
	// JGT
	addOp(new OpJgtC());
	addOp(new OpJgtR());
	// JGTE
	addOp(new OpJgteC());
	addOp(new OpJgteR());
	//CMP
	addOp(new OpCmpCR());
	addOp(new OpCmpRC());
	addOp(new OpCmpRR());
	addOp(new OpNot());
	 // AND
	addOp(new OpAndRC());
	addOp(new OpAndRR());
	addOp(new OpAndRRR());
	addOp(new OpAndRCR());
	addOp(new OpAndRRC());
	// OR
	addOp(new OpOrRC());
	addOp(new OpOrRR());
	addOp(new OpOrRRR());
	addOp(new OpOrRCR());
	addOp(new OpOrRRC());
	// XOR
	addOp(new OpXorRC());
	addOp(new OpXorRR());
	addOp(new OpXorRRR());
	addOp(new OpXorRCR());
	addOp(new OpXorRRC());
	// SHIFT LEFT
	addOp(new OpShftlRC());
	addOp(new OpShftlRR());
	addOp(new OpShftlRRR());
	addOp(new OpShftlRCR());
	addOp(new OpShftlRRC());
	// SHIFT RIGHT
	addOp(new OpShftrRC());
	addOp(new OpShftrRR());
	addOp(new OpShftrRRR());
	addOp(new OpShftrRCR());
	addOp(new OpShftrRRC());
}
