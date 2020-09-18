#include "cpu/instr.h"

make_instr_func(lea)
{
    //printf("eip%d\n",eip);
    int len=1;
    OPERAND mem, rm;//
    mem.data_size=data_size;
    rm.data_size=data_size;
    len+=modrm_r_rm(eip+1,&rm,&mem);
    operand_read(&rm);
    //printf("LEA1 %d\n",rm.val);
    //printf("LEA2 %d\n",mem.val);
    //printf("LEA2 %d\n",rm.val);
    //printf("LEA2 %d\n",mem.addr);
    print_asm_2("lea","l",len,&mem,&rm);
    rm.val=mem.addr;
    operand_write(&rm);
    //assert(0);
    //assert(0);
    return len;
}