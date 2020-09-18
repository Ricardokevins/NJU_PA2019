#include "cpu/instr.h"

static void instr_execute_1op()
{
    operand_read(&opr_src);
    switch (data_size)
    {
    case 8:
        if(opr_src.val==0x80)
            cpu.eflags.OF=1;
        break;
    case 16:
        if(opr_src.val==0x8000)
            cpu.eflags.OF=1;
        break;
    case 32:
        if(opr_src.val==0x80000000)
            cpu.eflags.OF=1;
        break;
    }
    opr_src.val-=1;
    if(opr_src.val==0)
        cpu.eflags.ZF=1;
    else
    {
        cpu.eflags.ZF=0;
    }
    if(sign(sign_ext(opr_src.val,data_size))==1)
    {
        cpu.eflags.SF=1;
    }
    else
    {
        cpu.eflags.SF=0;
    }
    int temp=opr_src.val&0xFF;
    int count=0;
    while(temp>0)
    {
        if(temp%2==1)
            count++;
        temp=temp/2;
    }
    if(count%2==1)
        cpu.eflags.PF=0;
    else
    {
        cpu.eflags.PF=1;
    }
    operand_write(&opr_src);
}

make_instr_impl_1op(dec,rm,b)
make_instr_impl_1op(dec,rm,v)
make_instr_impl_1op(dec,r,v)