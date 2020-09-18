#include "cpu/cpu.h"

void set_CF_add(uint32_t result,uint32_t src,size_t data_size)
{
	result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
	src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
	cpu.eflags.CF=result<src;
}

void set_ZF(uint32_t result,size_t data_size)
{
	result=result&(0xFFFFFFFF>>(32-data_size));
	cpu.eflags.ZF=(result==0);
}

void set_SF(uint32_t result,size_t data_size)
{
	result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
	cpu.eflags.SF=sign(result);
}

void set_PF(uint32_t result)
{
	uint32_t op=(result&0xFF);
	int count=0;
	int a=1;
	for(int i=0;i<8;i++)
	{
		int b=a&op;
		if(b)
		{
			count++;
		}
		a=a<<1;
	}
	if(count%2==0)
	{
		cpu.eflags.PF=1;

	}
	else
	{
		cpu.eflags.PF=0;
	}	
}

void set_OF_add(uint32_t result,uint32_t src,uint32_t dest,size_t data_size)
{
	switch(data_size)
	{
		case 8:
			{
				result=sign_ext(result&0xFF,8);
				src=sign_ext(src&0xFF,8);
				dest=sign_ext(dest&0xFF,8);
				break;
			}
		case 16:
			{
				result=sign_ext(result&0xFFFF,16);
				src=sign_ext(src&0xFFFF,16);
				dest=sign_ext(dest&0xFFFF,16);
				break;
			}
		default:break;
	}
	if(sign(src)==sign(dest))
	{
		if(sign(src)!=sign(result))
		{
			cpu.eflags.OF=1;
		}
		else
		{
			cpu.eflags.OF=0;
			
		}
		
	}
	else
	{
		cpu.eflags.OF=0;
	}
	
}

void set_CF_adc(uint32_t result,uint32_t src,size_t data_size)
{
	result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
	src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
	int a=cpu.eflags.CF;

	if(a == 0)
	{
		cpu.eflags.CF=result<src;
	}
	else
	{
		cpu.eflags.CF=result<=src;
	}
	
}

void set_CF_sub(uint32_t dest,uint32_t src,size_t data_size)
{
	dest=sign_ext(dest&(0xFFFFFFFF>>(32-data_size)),data_size);
	src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
	cpu.eflags.CF=dest<src;
}

void set_CF_sbb(uint32_t dest,uint32_t src,size_t data_size)
{
	dest=sign_ext(dest&(0xFFFFFFFF>>(32-data_size)),data_size);
	src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
	if(cpu.eflags.CF==0)
	{
		cpu.eflags.CF=dest<src;

	}
	else
	{
		cpu.eflags.CF=dest<=src;
	}
	
}

void set_OF_sub(uint32_t result,uint32_t src,uint32_t dest,size_t data_size)
{
	switch(data_size)
	{
		case 8:
			{
				result=sign_ext(result&0xFF,8);
				src=sign_ext(src&0xFF,8);
				dest=sign_ext(dest&0xFF,8);
				break;
			}
		case 16:
			{
				result=sign_ext(result&0xFFFF,16);
				src=sign_ext(src&0xFFFF,16);
				dest=sign_ext(dest&0xFFFF,16);
				break;
			}
		default:break;
	}
	if(sign(src)!=sign(dest))
	{
		if(sign(src)==sign(result))
		{
			cpu.eflags.OF=1;
		}
		else
		{
			cpu.eflags.OF=0;
			
		}
		
	}
	else
	{
		cpu.eflags.OF=0;
	}
}
uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	uint32_t res=0;
	res=dest+src;
	set_CF_add(res,src,data_size);
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_add(res,src,dest,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	uint32_t res=0;
	res=dest+src+cpu.eflags.CF;
	set_CF_adc(res,src,data_size);
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_add(res,src,dest,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	uint32_t res=0;
	uint32_t temp;
	temp=(src^0xFFFFFFFF)+1;
	res=dest+temp;
	set_CF_sub(dest,src,data_size);
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_sub(res,src,dest,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	uint32_t res=0;
	uint32_t temp;
	temp=(src^0xFFFFFFFF)+1;
	res=dest+temp-cpu.eflags.CF;
	set_CF_sbb(dest,src,data_size);
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_sub(res,src,dest,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	uint64_t res;
	res=(uint64_t)src*(uint64_t)dest;
	uint32_t flags = res>>data_size;
	if(flags&0xFFFFFFFF)
	{
		cpu.eflags.CF=1;
		cpu.eflags.OF=1;
	}
	else
	{
		cpu.eflags.CF=0;
		cpu.eflags.OF=0;		
	}

	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	return res;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	int64_t res;
	res=(uint64_t)src*(uint64_t)dest;
	
	if(!dest||res/dest==src)
	{
		cpu.eflags.CF=0;
		cpu.eflags.OF=0;
	}
	else
	{
		cpu.eflags.CF=1;
		cpu.eflags.OF=1;		
	}

	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	return res;


#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	//assert(src==0);
	uint32_t res=dest/src;
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	cpu.eflags.CF=0;
	cpu.eflags.OF=0;
	return res&(0xFFFFFFFF>>(32-data_size));

#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	dest=sign_ext_64(dest&(0xFFFFFFFFFFFFFFFF>>(64-data_size)),data_size);
	src=sign_ext_64(src&(0xFFFFFFFFFFFFFFFF>>(64-data_size)),data_size);
	int32_t res=dest/src;
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	cpu.eflags.CF=0;
	cpu.eflags.OF=0;
	return res;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	return dest%src;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	return dest%src;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	uint32_t res;
	res=src&dest;
	cpu.eflags.CF=0;
	cpu.eflags.OF=0;
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	uint32_t res;
	res=src^dest;
	cpu.eflags.CF=0;
	cpu.eflags.OF=0;
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t res;
	res=src|dest;
	cpu.eflags.CF=0;
	cpu.eflags.OF=0;
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	//dest=sign_ext(dest&(0xFFFFFFFF>>(32-data_size)),data_size);
	//src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
	uint32_t temp=0;
	uint32_t high_data=0;
	if(data_size!=32)
	{
		high_data=(dest>>data_size)<<data_size;
	}
	uint32_t res=dest-high_data;
	if(data_size>src)
		temp=(1<<(data_size-src));
	cpu.eflags.CF=0;
	cpu.eflags.CF=((res&temp)>=1);
	res<<=src;
	res=res&(0xFFFFFFFF>>(32-data_size));
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	cpu.eflags.OF=0;
	return res;
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	uint32_t temp;
	uint32_t res=dest;
	dest=dest&(0xFFFFFFFF>>(32-data_size));
	temp=dest>>(src-1);
	res=temp/2;
	temp=sign_ext(temp&(0xFFFFFFFF>>(32-data_size)),data_size);
	cpu.eflags.CF=(temp&0x1);
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	cpu.eflags.OF=0;
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}
//#define NEMU_REF_ALU
uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
    uint32_t temp2;
	temp2=dest;
	uint32_t temp;
	uint32_t res;
	res=dest;
	switch (data_size)
	{
	case(8):		
		
		temp2=temp2>>(7);
		if((temp2&0x1)>0)
		{
			dest=dest&(0xFFFFFFFF>>(32-data_size));
			temp=dest>>(src-1);
			res=temp/2;
			//temp=sign_ext(temp&(0xFFFFFFFF>>(32-data_size)),data_size);
			cpu.eflags.CF=((temp&0x1)>0);
			res=res|(0xFF<<(8-src));
			set_PF(res);
			set_ZF(res,data_size);
			set_SF(res,data_size);
			cpu.eflags.OF=0;
			res=res|(0xFFFFFFFF<<(8-src));
			return res&(0xFFFFFFFF>>(32-data_size));
		}
		else
		{
			dest=dest&(0xFFFFFFFF>>(32-data_size));
			temp=dest>>(src-1);
			res=temp/2;
			temp=sign_ext(temp&(0xFFFFFFFF>>(32-data_size)),data_size);
			cpu.eflags.CF=(temp&0x1);
			set_PF(res);
			set_ZF(res,data_size);
			set_SF(res,data_size);
			cpu.eflags.OF=0;
			return res&(0xFFFFFFFF>>(32-data_size));		
		}
		
		break;
	case(16):
		
		temp2=temp2>>(15);
		if((temp2&0x1)>0)
		{
			dest=dest&(0xFFFFFFFF>>(32-data_size));
			temp=dest>>(src-1);
			res=temp/2;
			//temp=sign_ext(temp&(0xFFFFFFFF>>(32-data_size)),data_size);
			cpu.eflags.CF=(temp&0x1);
			res=res|(0xFFFF<<(16-src));
			set_PF(res);
			set_ZF(res,data_size);
			set_SF(res,data_size);
			cpu.eflags.OF=0;
			res=res|(0xFFFFFFFF<<(16-src));
			return res&(0xFFFFFFFF>>(32-data_size));
		}
		else
		{
			dest=dest&(0xFFFFFFFF>>(32-data_size));
			temp=dest>>(src-1);
			res=temp/2;
			temp=sign_ext(temp&(0xFFFFFFFF>>(32-data_size)),data_size);
			cpu.eflags.CF=(temp&0x1);
			set_PF(res);
			set_ZF(res,data_size);
			set_SF(res,data_size);
			cpu.eflags.OF=0;
			return res&(0xFFFFFFFF>>(32-data_size));		
		}
		break;
	case(32):
		
		temp2=temp2>>(31);
		if((temp2&0x1)>0)
		{
			dest=dest&(0xFFFFFFFF>>(32-data_size));
			temp=dest>>(src-1);
			res=temp/2;
			//temp=sign_ext(temp&(0xFFFFFFFF>>(32-data_size)),data_size);
			cpu.eflags.CF=(temp&0x1);
			res=res|(0xFFFFFFFF<<(32-src));
			set_PF(res);
			set_ZF(res,data_size);
			set_SF(res,data_size);
			cpu.eflags.OF=0;
			res=res|(0xFFFFFFFF<<(32-src));
			return res&(0xFFFFFFFF>>(32-data_size));
		}
		else
		{
			dest=dest&(0xFFFFFFFF>>(32-data_size));
			temp=dest>>(src-1);
			res=temp/2;
			temp=sign_ext(temp&(0xFFFFFFFF>>(32-data_size)),data_size);
			cpu.eflags.CF=(temp&0x1);
			set_PF(res);
			set_ZF(res,data_size);
			set_SF(res,data_size);
			cpu.eflags.OF=0;
			return res&(0xFFFFFFFF>>(32-data_size));		
		}
		break;
	default:
		assert(0);
		break;
	}
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	uint32_t temp;
	uint32_t res=dest;
	temp=dest<<(src-1);
	res=temp<<1;
	temp=sign_ext(temp&(0xFFFFFFFF>>(32-data_size)),data_size);
	//res=temp<<1;
	cpu.eflags.CF=(temp>>31);
	
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	cpu.eflags.OF=0;
	return res&(0xFFFFFFFF>>(32-data_size));
#endif
}
