#include "cpu/instr.h"

make_instr_func(lidt)
{
    int len=1;
    OPERAND rel;
    rel.data_size=data_size;
    rel.sreg=SREG_SS;
    len+=modrm_rm(eip+1,&rel);

    OPERAND mem_limit,mem_base;

    mem_limit.type=mem_base.type=OPR_IMM;
    mem_limit.sreg=mem_base.sreg=SREG_SS;
    mem_limit.data_size=16;
    

    if(data_size==16)
    {
        mem_base.data_size=24;
    }
    else
    {
        if(data_size==32)
        {
            mem_base.data_size=32;
        }
    }
    
    mem_limit.addr=rel.addr;
    operand_read(&mem_limit);

    cpu.idtr.limit=mem_limit.val;
    
    mem_base.addr=mem_limit.addr+2;
    operand_read(&mem_base);
    cpu.idtr.base=mem_base.val;

    rel.data_size=16;
    operand_read(&rel);

    cpu.idtr.limit=rel.val;
    rel.data_size=32;

    rel.addr+=2;
    
   
    print_asm_1("lidt","",len,&rel);
    return len;
}
