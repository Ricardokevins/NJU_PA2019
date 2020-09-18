#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

void raise_intr(uint8_t intr_no) {
#ifdef IA32_INTR
	OPERAND temp;

	temp.type = OPR_MEM;
	temp.sreg = SREG_SS;
	temp.data_size = 32;

	cpu.esp -= 4;
	temp.addr = cpu.esp;
	temp.val = cpu.eflags.val;
    operand_write(&temp);

	
	cpu.esp -= 4;
	temp.data_size = 32;
	temp.addr = cpu.esp;
	temp.val = cpu.cs.val;
	operand_write(&temp);


	cpu.esp -= 4;
	temp.addr = cpu.esp;
	temp.data_size = 32;
	temp.val = cpu.eip;
	operand_write(&temp);
	
	//get addr
	uint32_t idt_addr = cpu.idtr.base + intr_no * 8;
	
	GateDesc cur_gateDesc;

	cur_gateDesc.val[0] = laddr_read(idt_addr, 4);

	cur_gateDesc.val[1] = laddr_read(idt_addr + 4, 4);
	
	assert(cur_gateDesc.present == 1);//assert

	if(cur_gateDesc.type == 0xe){
		cpu.eflags.IF = 0;
	}
	
	cpu.cs.val = cur_gateDesc.selector;

	cpu.eip = cur_gateDesc.offset_15_0 + (cur_gateDesc.offset_31_16 << 16);
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	cpu.eip += 2;
	//head to next instr
	raise_intr(intr_no);
}
