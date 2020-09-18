#include "cpu/instr.h"

make_instr_func(mytest_r2rm_v)
{
    int len=1;
    OPERAND reg, rm;//
    len+=modrm_r_rm(eip+1, &reg, &rm);
    rm.data_size=data_size;
    reg.data_size=data_size;
    operand_read(&reg);
    operand_read(&rm);
    //printf("test%d\n",reg.val);
    //printf("test%d\n",rm.val);
    alu_and(reg.val,rm.val,data_size);
    
    //printf("test%d\n",reg.val);
    //printf("test%d\n",rm.val);
    print_asm_2("test","",len,&rm,&reg);
    //printf("%d\n",imm.val);
    return len;
}

make_instr_func(test_r2rm_b)
{
    int len=1;
    OPERAND reg, rm;//
    len+=modrm_r_rm(eip+1, &reg, &rm);
    rm.data_size=8;
    reg.data_size=8;
    operand_read(&reg);
    operand_read(&rm);
    //printf("test%d\n",reg.val);
    //printf("test%d\n",rm.val);
    alu_and(reg.val,rm.val,data_size);
    
    //printf("test%d\n",reg.val);
    //printf("test%d\n",rm.val);
    print_asm_2("test","",len,&rm,&reg);
    //printf("%d\n",imm.val);
    return len;
}
make_instr_func(test_i2rm_v)
{
    int len=1;
    OPERAND imm, rm;//
    len+=modrm_rm(eip+1, &rm);
    rm.data_size=data_size;
    imm.data_size=data_size;
    imm.type=OPR_IMM;
    imm.addr=len+eip;
    operand_read(&imm);
    operand_read(&rm);
    //printf("test%d\n",reg.val);
    //printf("test%d\n",rm.val);
    alu_and(imm.val,rm.val,data_size);
    
    //printf("test%d\n",reg.val);
    //printf("test%d\n",rm.val);
    print_asm_2("test","",len+data_size/8,&imm,&rm);
    //printf("%d\n",imm.val);
    return len+data_size/8;
}

make_instr_func(test_i2a_v)
{
    int len=1;
    OPERAND imm, rm;//
    rm.type=OPR_REG;
    rm.data_size=data_size;
    rm.addr=0;
    imm.data_size=data_size;
    imm.type=OPR_IMM;
    imm.addr=len+eip;
    operand_read(&imm);
    operand_read(&rm);
    //printf("test%d\n",reg.val);
    //printf("test%d\n",rm.val);
    alu_and(imm.val,rm.val,data_size);
    
    //printf("test%d\n",reg.val);
    //printf("test%d\n",rm.val);
    print_asm_2("test","v",len+data_size/8,&imm,&rm);
    //printf("%d\n",imm.val);
    return len+data_size/8;
}

make_instr_func(test_i2a_b)
{
    int len=1;
    OPERAND imm, rm;//
    rm.type=OPR_REG;
    rm.data_size=8;
    rm.addr=0;
    imm.data_size=8;
    imm.type=OPR_IMM;
    imm.addr=len+eip;
    operand_read(&imm);
    operand_read(&rm);
    //printf("test%d\n",reg.val);
    //printf("test%d\n",rm.val);
    alu_and(imm.val,rm.val,8);
    
    //printf("test%d\n",reg.val);
    //printf("test%d\n",rm.val);
    print_asm_2("test","v",len+1,&imm,&rm);
    //printf("%d\n",imm.val);
    return len+1;
}

make_instr_func(test_i2rm_b)
{
    int len=1;
    OPERAND imm, rm;//
    len+=modrm_rm(eip+1, &rm);
    rm.data_size=8;
    imm.data_size=8;
    imm.type=OPR_IMM;
    imm.addr=len+eip;
    operand_read(&imm);
    operand_read(&rm);
    //printf("test%d\n",reg.val);
    //printf("test%d\n",rm.val);
    alu_and(imm.val,rm.val,8);
    
    //printf("test%d\n",reg.val);
    //printf("test%d\n",rm.val);
    print_asm_2("test","b",len+1,&imm,&rm);
    //printf("%d\n",imm.val);
    return len+1;
}