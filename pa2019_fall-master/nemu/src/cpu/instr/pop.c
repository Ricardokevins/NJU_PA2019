#include "cpu/instr.h"

make_instr_func(pop_r_v)
{
    
    OPERAND mem;
    mem.type=OPR_IMM;
    mem.addr=cpu.esp;
    mem.data_size=data_size;
    operand_read(&mem);
    
    OPERAND imm;
    imm.type=OPR_IMM;
    imm.addr=eip;
    imm.data_size=8;
    operand_read(&imm);

    OPERAND reg;
    reg.type=OPR_REG;
    reg.addr=imm.val-0x58;
    reg.data_size=data_size;
    reg.val=mem.val;
    operand_write(&reg);
    if(data_size==16)
    {
        cpu.esp+=2;
    }
    else
    {
        cpu.esp+=4;
    }
    print_asm_1("pop","",1,&reg);
    
    return 1;
}

make_instr_func(popa)
{
	opr_src.data_size=32;
	opr_src.type=OPR_MEM;
	opr_src.sreg=SREG_DS;

	opr_src.addr=cpu.esp;
	operand_read(&opr_src);
	cpu.edi=opr_src.val;
	cpu.esp+=4;

    opr_src.addr=cpu.esp;
	operand_read(&opr_src);
	cpu.esi=opr_src.val;
	cpu.esp+=4;
	
	opr_src.addr=cpu.esp;
	operand_read(&opr_src);
	cpu.ebp=opr_src.val;
	cpu.esp+=4;
	
	cpu.esp+=4;
	
	opr_src.addr=cpu.esp;
	operand_read(&opr_src);
	cpu.ebx=opr_src.val;
	cpu.esp+=4;

	opr_src.addr=cpu.esp;
	operand_read(&opr_src);
	cpu.edx=opr_src.val;
	cpu.esp+=4;

	opr_src.addr=cpu.esp;
	operand_read(&opr_src);
	cpu.ecx=opr_src.val;
	cpu.esp+=4;

	opr_src.addr=cpu.esp;
	operand_read(&opr_src);
	cpu.eax=opr_src.val;
	cpu.esp+=4;
    print_asm_0("popa", "", 1);
	return 1;

}