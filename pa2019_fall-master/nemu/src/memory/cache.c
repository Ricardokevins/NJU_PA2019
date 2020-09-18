#include "nemu.h"
#include "memory/memory.h"
#include "memory/cache.h"
#include <memory.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

CacheLine Cache[1024];

void init_cache()
{
    for(int i=0;i<1024;i++)
    {
        Cache[i].vaild=0;
    }
}

uint32_t cache_read(paddr_t paddr,size_t len, CacheLine*cache)
{
    uint32_t mysign=(paddr>>13)&0x7ffff;
    uint32_t mygroup_num=(paddr>>6)&0x7f;
    uint32_t offset=(paddr)&0x3f;
    for(int i=0;i<8;i++)
    {
        if(cache[mygroup_num*8+i].sign==mysign&&cache[mygroup_num*8+i].vaild==1)
        {
            if(offset+len>64)
            {
                uint32_t data1=0;
                uint32_t data2=0;
                memcpy(&data1,cache[mygroup_num*8+i].block+offset,64-offset);
                data2=cache_read(paddr+64-offset,offset+len-64,Cache);
                data2=data2<<(8*(64-offset));
                return (data2|data1);
            }
            else
            {
                uint32_t temp_data= 0;
                memcpy(&temp_data,cache[mygroup_num*8+i].block+offset,len);
                return temp_data;            
            }  
        }
        
    }
    uint32_t temp_data2=0;
    memcpy(&temp_data2,hw_mem+paddr,len);
    for(int i=0;i<8;i++)
    {
        if(cache[mygroup_num*8+i].vaild==0)
        {
            cache[mygroup_num*8+i].vaild=1;
            cache[mygroup_num*8+i].sign=mysign;
            memcpy(cache[mygroup_num*8+i].block,hw_mem+paddr-offset,64);
            return temp_data2;
        }
    }
    srand((unsigned)time(0));
    int change_num=rand()%8;
    cache[mygroup_num*8+change_num].sign=mysign;
    cache[mygroup_num*8+change_num].vaild=1;
    memcpy(cache[mygroup_num*8+change_num].block,hw_mem+paddr-offset,64);
    return temp_data2;
}

void cache_write(paddr_t paddr,size_t len, uint32_t data,CacheLine*cache)
{
    memcpy(hw_mem+paddr,&data,len);
    uint32_t mysign=(paddr>>13)&0x7ffff;
    uint32_t mygroup_num=(paddr>>6)&0x7f;
    uint32_t offset=(paddr)&0x3f;
    for(int i=0;i<8;i++)
    {
        if(cache[mygroup_num*8+i].sign==mysign&&cache[mygroup_num*8+i].vaild==1)
        {
            if(offset+len>64)
            {
                cache_write(paddr,64-offset,data,Cache);
                cache_write(paddr+64-offset,len+offset-64,data>>(8*(64-offset)),Cache);
                return ;
            }
            else
            {
                memcpy(cache[mygroup_num*8+i].block+offset,&data,len);
                return ;
            }
            
        }
    }

}