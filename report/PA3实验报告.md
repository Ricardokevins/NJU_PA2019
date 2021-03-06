# PA实验报告

​																																		181860077 佘帅杰

### 简单思考想法

整体的前期的实现没有什么问题

在3-3的时候遇到了一些问题，然后重新反复阅读了init_page函数对应的汇编代码，大致对照着C代码读完了那一段的汇编

在Pa3-3有一些Debug的建议

1. 反汇编输入 objdump -d kernel/kernel
2. 在cpu.c 的exec_instr里面可以加上打印eip和opcode的代码
3. 然后利用nemu调试进行调试

初步是可以发现两层循环的汇编框架，对应在C代码里也有初始化页表的过程，这个过程基本是框架代码实现的没有问题，接着就是write_cr0 和 write_cr3相关的，cr0的赋值操作可参考下一题贴出的部分汇编代码，cr3基本也可以看到类似的框架的痕迹 push 一个参数，然后把参数mov_r2c，所以就是要检查mov写的是否正确。

在这里可以设置断点，在cr3的写入的时候会有进入对内存的写，这个时候就会调用translate，如果没有就说明在此之前就有问题了，多半是cr0依旧是0，所以就可以进行汇编的查看，看看是不是传入的参数不对

最后还可以在config.h里打开ref_opcode 的宏进行调试

##### NEMU在什么时候进入了保护模式？

计算机在刚开始的时候进入的是实模式。在实模式下，操作系统需要完成包括初始段表（GDT）和描述符表寄存器（GDTR）。在初始化完成之后，操作系统通过把0号寄存器CR0中的PE位置为1，来通知机器进入了保护模式，在此之前PE是0

对应在汇编代码里大致有如下的若干条

```c
mov xxx %eax
orl 0x1 %eax
mov %eax %cr0
```

那么显然这样对cr0进行了赋值，也就是打开了保护模式

##### GDTR里的段表首地址是虚拟地址，线性地址还是物理地址，为什么

线性地址。GDTR的段表首地址的存在意义就是为了在能够给虚拟地址的时候就能够提供基地址以能计算出线性地址（物理地址）。如果GDTR段表首地址也是虚拟地址，那GDTR的段表首地址也需要进行转化，这个是无法做到的。所以一定是线性地址。（物理地址是线性地址基于页表进行换算的，这里不需要直接是物理地址，可以用页表计算）

##### Kernel的虚拟页和物理页的映射关系是什么，请画图进行说明

全相联映射关系

| 地址                           | 内容           |
| ------------------------------ | -------------- |
| 0xc8000000---0xc7fff000        | 操作系统栈区   |
| 0xc7fff000---0xc0032000        | 空             |
| 0xc0032000---0xc0031000        | 操作系统代码区 |
| 0xc0031000---0xc7003000        | 空             |
| 0xc7003000-------------------- | 空             |

##### 以某个测试样例为例，画图说明用户进程的虚拟页和物理页之间的映射关系是怎么样的？Kernel映射的是哪一段？你可以在loader中通过输出Log()输出mm_malloc的结果来查看映射关系，并结合init_mm()中的代码来画出内核映射关系

全相联映射。没有进行说明的就是空页（下同）

| 地址                  | 内容           | 地址                | 页    |
| --------------------- | -------------- | ------------------- | ----- |
| 0xc8000000-0xc7fff000 | Kernel 栈区    | 0x8000000-0x7fff000 | pageA |
| 0xc0031000-0xc0030000 | Kernel 代码区  | 0x31000-0x30000     | pageB |
| 0xc0000000-0xbffff000 | 用户进程栈区   | 0x1102000-0x1101000 | pageC |
| 0x804b000-0x804a000   | 用户进程数据区 | 0x1002000-0x1001000 | pageD |
| 0x8049000-0x8048000   | 用户进程代码区 | 0x1001000-0x1000000 | pageE |


##### 在kernel完成页表初始化之前，程序无法访问全局变量是否正确这个表述是否正确？在init_page里我们对全局变量进行了怎么样的处理？

正确

由于全局变量使用的是虚拟地址，所以在页表初始化完成之前是不知道全局变量对应的物理地址的，也就无法访问

但是有一个例外，在init_page这个函数所在的代码文件kvm.c里就有好几个全局变量如下

```c
#include "common.h"
#include "x86.h"
#include "memory.h"
#include <string.h>

PDE kpdir[NR_PDE] align_to_page;				// kernel page directory
PTE kptable[PHY_MEM / PAGE_SIZE] align_to_page;		// kernel page tables

PDE* get_kpdir() { return kpdir; }
/* set up page tables for kernel */
void init_page(void) {
	CR0 cr0;
	CR3 cr3;
	PDE *pdir = (PDE *)va_to_pa(kpdir);
	PTE *ptable = (PTE *)va_to_pa(kptable);
	uint32_t pdir_idx, ptable_idx, pframe_idx;

```

那么对于这几个全局变量，可以看到在函数里没有被直接的使用，而是使用了一个宏va_to_pa进行了处理，对va_to_pa进行搜索，在memory.h里找到了这个宏

```c
#define va_to_pa(addr) ((void *)(((uint32_t)(addr)) - KOFFSET))
#define pa_to_va(addr) ((void *)(((uint32_t)(addr)) + KOFFSET))
```

显然这个宏实现了虚拟地址到物理地址的转化，减去了KOFFSET。这里是应用了已有的知识，是已知地址是从c000000开始加载的，所以可以直接进行这么操作。进行处理

