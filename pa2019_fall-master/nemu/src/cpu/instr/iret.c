#include "cpu/instr.h"
#include "cpu/intr.h"

make_instr_func(iret){
	OPERAND IMM;
	

	IMM.data_size = 32;
	IMM.addr = cpu.esp;
	
	IMM.type = OPR_MEM;
	IMM.sreg = SREG_SS;

	if(data_size == 32)
    {
		operand_read(&IMM);
		cpu.esp += 4;
		cpu.eip = IMM.val;
	}
	
	IMM.addr = cpu.esp;
    
	operand_read(&IMM);
    
	cpu.esp += 4;
	cpu.cs.val = IMM.val;

	if(data_size == 32)
    {
		IMM.addr = cpu.esp;
		operand_read(&IMM);
		cpu.esp += 4;
		cpu.eflags.val = IMM.val;
	}
	
	print_asm_0("iret","", 1);

	return 0;
}