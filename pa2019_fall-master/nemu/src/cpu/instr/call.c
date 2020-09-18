#include "cpu/instr.h"

make_instr_func(mycall_near){
	uint32_t eipval = 0;
	OPERAND dest,ret;

	if(data_size == 16){
		cpu.esp -= 2;
		eipval = cpu.eip & 0xffff;
	}
	if(data_size == 32){
		cpu.esp -= 4;
		eipval = cpu.eip;
	}

	ret.data_size = dest.data_size = data_size;
	ret.type = OPR_MEM;
	ret.addr = cpu.esp;
	ret.sreg = SREG_SS;
	ret.val = eipval = eipval + 1 +data_size / 8;
	operand_write(&ret);

	dest.type = OPR_IMM;
	dest.addr = eip + 1;
	dest.sreg = SREG_CS;
	operand_read(&dest);
	eipval += dest.val;

	if(data_size == 16){
		cpu.eip = eipval & 0xffff;	
	}
	if(data_size == 32){
		cpu.eip = eipval;
	}
	print_asm_0("call","",1);

	return 0;
}

make_instr_func(mycall_near_indirect){
	uint32_t eipval = 0;
	OPERAND dest, ret;
	if(data_size == 16){
		cpu.esp -= 2;
		eipval = cpu.eip & 0xffff;
	}
	if(data_size == 32){
		cpu.esp -= 4;
		eipval = cpu.eip;
	}

	ret.data_size = dest.data_size = data_size;
	ret.sreg = SREG_SS;
	dest.sreg = SREG_CS;
	
	int len = 1;
	len += modrm_rm(eip + 1, &dest);

	ret.type = OPR_MEM;
	ret.addr = cpu.esp;
	ret.val = eipval + len;
	operand_write(&ret);
	operand_read(&dest);

	if(data_size == 16){
		cpu.eip = dest.val & 0xffff;
	}
	if(data_size == 32){
		cpu.eip = dest.val;
	}

	print_asm_1("call","", len, &dest);

	return 0;

}
