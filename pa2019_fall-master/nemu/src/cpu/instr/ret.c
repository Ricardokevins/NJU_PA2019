#include "cpu/instr.h"

make_instr_func(ret_near)
{
    OPERAND mem;
    mem.data_size=32;
    mem.type=OPR_MEM;
    mem.addr=cpu.esp;
    operand_read(&mem);
    print_asm_0("ret","",0);
    cpu.eip=mem.val;
    cpu.esp+=4;
    
    return 0;
}
make_instr_func(ret_near_imm16)
{
    OPERAND mem;
    mem.data_size=32;
    mem.type=OPR_MEM;
    mem.addr=cpu.esp;
    operand_read(&mem);
    print_asm_0("ret","",0);
    cpu.eip=mem.val;
    cpu.esp+=4;
    OPERAND imm;
    imm.type=OPR_IMM;
    imm.addr=eip+1;
    imm.data_size=16;
    operand_read(&imm);
    cpu.esp+=imm.val;
    return 0;
}
