#ifndef _CACHE_H
#define _CACHE_H
#include "nemu.h"
typedef struct{
    bool vaild;
    uint32_t sign;
    uint8_t block[64];
}CacheLine;
extern CacheLine Cache[];
void init_cache();
uint32_t cache_read(paddr_t paddr,size_t len, CacheLine *cache);
void cache_write(paddr_t paddr,size_t len,uint32_t data,CacheLine *cache);

#endif