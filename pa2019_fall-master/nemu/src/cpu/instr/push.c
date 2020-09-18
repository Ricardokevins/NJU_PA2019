#include "cpu/instr.h"
static void instr_execute_1op()
{
    operand_read(&opr_src);
    cpu.gpr[4].val-=data_size/8;
    OPERAND mem;
    mem.type=OPR_MEM;
    mem.data_size=data_size;
    if(opr_src.data_size==8)
    {
        opr_src.val=sign_ext(opr_src.val,8);
    }
   // printf("hit src_read\n");
    mem.val=opr_src.val;
    mem.sreg=SREG_SS;
    mem.addr=cpu.gpr[4].val;
  
   // printf("addr %x\n",mem.addr);
    
    operand_write(&mem);
    //printf("hit push inside\n");
}
make_instr_impl_1op(push,r,v);
make_instr_impl_1op(push,rm,v);
make_instr_impl_1op(push,i,v);

make_instr_func(push_i_b)
{
    cpu.esp-=4;
    int len=1;

    OPERAND imm;
    imm.data_size=8;
    imm.addr=eip+1;
    imm.type=OPR_IMM;
    operand_read(&imm);

    imm.val = sign_ext(imm.val,8);
    len+=1;
    OPERAND mem;
    mem.type=OPR_MEM;
    mem.addr=cpu.esp;
    mem.data_size=data_size;
    mem.val=imm.val;
    mem.sreg=SREG_DS;

    operand_write(&mem);
    print_asm_1("pushb"," ",len,&imm);
    return len;
}


make_instr_func(pusha){
	
	uint32_t temp=cpu.esp;
	opr_dest.data_size=32;
	opr_dest.type=OPR_MEM;
	opr_dest.sreg=SREG_DS;

	cpu.esp-=data_size/8;
	opr_dest.addr=cpu.esp;
	opr_dest.val=cpu.eax;
	operand_write(&opr_dest);

	cpu.esp-=data_size/8;
	opr_dest.addr=cpu.esp;
	opr_dest.val=cpu.ecx;
	operand_write(&opr_dest);

	cpu.esp-=data_size/8;
	opr_dest.addr=cpu.esp;
	opr_dest.val=cpu.edx;
	operand_write(&opr_dest);

	cpu.esp-=data_size/8;
	opr_dest.addr=cpu.esp;
	opr_dest.val=cpu.ebx;
	operand_write(&opr_dest);

	cpu.esp-=data_size/8;
	opr_dest.addr=cpu.esp;
	opr_dest.val=temp;
	operand_write(&opr_dest);

	cpu.esp-=data_size/8;
	opr_dest.addr=cpu.esp;
	opr_dest.val=cpu.ebp;
	operand_write(&opr_dest);

	cpu.esp-=data_size/8;
	opr_dest.addr=cpu.esp;
	opr_dest.val=cpu.esi;
	operand_write(&opr_dest);

	cpu.esp-=data_size/8;
	opr_dest.addr=cpu.esp;
	opr_dest.val=cpu.edi;
	operand_write(&opr_dest);
    
	print_asm_0("pusha", "", 1);
	return 1;
}
	
	