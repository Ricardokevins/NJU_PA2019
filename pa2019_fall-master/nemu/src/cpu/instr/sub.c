#include "cpu/instr.h"

static void instr_execute_2op(){
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_sub(opr_src.val, opr_dest.val ,data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(sub, i, a, b);
make_instr_impl_2op(sub, i, a, v);
make_instr_impl_2op(sub, i, rm, b);

make_instr_impl_2op(sub, r, rm, b);
make_instr_impl_2op(sub, rm, r, b);

make_instr_func(mysub_i2rm_bv)
{
    int len=1;
    OPERAND imm, rm;//

    len+=modrm_rm(eip+1,&rm);
    rm.data_size=data_size;

    imm.type=OPR_IMM;
    imm.addr=len+eip;
    imm.data_size=8;

    operand_read(&imm);
    operand_read(&rm);
    //printf("sub%d\n",rm.val);
    imm.val=sign_ext(imm.val,8);
    
    rm.val=alu_sub(imm.val,rm.val,data_size);
    operand_write(&rm);
    print_asm_2("sub","bv",len+1,&imm,&rm);
    return len+imm.data_size/8;
}
make_instr_func(sub_rm2r_v)
{
    int len=1;
    OPERAND reg, rm;
    len+=modrm_r_rm(eip+1, &reg, &rm);
    rm.data_size=data_size;
    reg.data_size=data_size;
    operand_read(&reg);
    operand_read(&rm);
    
    reg.val=alu_sub(rm.val,reg.val,data_size);
    operand_write(&reg);
    
    print_asm_2("sub","v",len+data_size/8,&rm,&reg);
    return len;
}

make_instr_func(sub_r2rm_v)
{
    int len=1;
    OPERAND reg, rm;//
    len+=modrm_r_rm(eip+1, &reg, &rm);
    rm.data_size=data_size;
    reg.data_size=data_size;
    operand_read(&reg);
    operand_read(&rm);
    
    rm.val=alu_sub(reg.val,rm.val,data_size);
    operand_write(&rm);
    
    print_asm_2("sub","v",len+data_size/8,&reg,&rm);
    return len;
}

make_instr_func(sub_i2rm_v)
{
    int len=1;
    OPERAND imm, rm;

    len+=modrm_rm(eip+1,&rm);
    rm.data_size=data_size;

    imm.type=OPR_IMM;
    imm.addr=len+eip;
    imm.data_size=data_size;

    operand_read(&imm);
    operand_read(&rm);
    
    
    rm.val=alu_sub(imm.val,rm.val,data_size);
    operand_write(&rm);
    print_asm_2("sub","v",len+data_size/8,&imm,&rm);
    return len+imm.data_size/8;
}