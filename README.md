# ICS2021

这里记录我写ICS2021的思路和收获，pa4没有写，因为我觉得pa4和os相关，而我已经接收了os相关的实验训练

## PA0

pa0阅读实验讲义，我提取出如下几个cser的基本素质，有些是我不完全具备的

+ 阅读英文

+ 备份重要数据

+ 光看说明书不做不能学会

+ STFW RTFM RTFSC（注意阅读的仔细）

+ 使用工具提高效率（vim）

+ 注意提问艺术

+ 使用linux

## PA1

PA1主要是基础设施，即调试代码的实现

#### 单步执行

将执行次数传入函数cpu_exec即可

#### 打印程序状态

根据输入参数打印寄存器内容或者监视点信息即可

#### 扫描内存

调用内存读取接口vaddr_read打印指定内存值即可，注意格式美观

#### 表达式求值

主要还是完整的求值函数eval流程

```
eval(p, q) {
  if (p > q) {
    /* Bad expression */
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1);
  }
  else {
    op = the position of 主运算符 in the token expression; /* 根据优先级找出不在括号内的、在最右边的、优先级最低的运算符 */
    val1 = eval(p, op - 1);
    val2 = eval(op + 1, q);

    switch (op_type) {
      case '+': return val1 + val2;
      case '-': /* ... */
      case '*': /* ... */
      case '/': /* ... */
      default: assert(0);
    }
  }
}
```

对于解引用运算符，由于和乘号长的一样，在词法分析时统一被识别为乘号，在进行语法分析之前，对所有 * 之前的符号进行判断，除非是寄存器或者是数据，否则判断为解引用运算符

#### 设置监视点

+ 注意在watchpoint结构体中加入寄存器或者指定内存的old_value字段

+ 将新建节点加入全局链表

+ 定义CONFIG_WATCHPOINT宏，若此宏被定义，则在trace_and_difftest函数中将监视点当前值和old值进行对比，若不同则停下

#### 删除监视点

+ 和链表删除类似

## PA2

pa2会遇到很多宏，这方面注意不要强行说理解所有宏的展开内容。

#### 指令的完善

nemu执行指令的流程分为四部分：IF、ID、EX和更新pc

眼前只能识别和执行四五条指令，我们的任务就是实现更多指指令

+ 在isa-def.c中添加缺失指令类型的识别

+ 在isa-all-instr.h添加指令名称

+ 在decode.c中添加识别机理

+ 在instr目录下的文件（如compute.h）使用rtl实现执行机理

实现完指令后，在am-kernels/tests/cpu-tests目录下

```
make ARCH=$ISA-nemu ALL=程序名称 run
``` 

去掉ALL则运行批量测试

一开始我的dummy都出现了错误，后来仔细阅读手册时，发现我没有注意到符号位扩展，很多指令是符号位扩展

#### 运行时环境，实现常见库函数

在计算机硬件之上，实现对上层能够屏蔽底层isa架构的裸机运行时库

##### string.c

遇到两个坑：

+ strncpy函数注意，如果复制长度不够，dst剩余部分需要填充0

+ 涉及到构造新的字符串，诸如strcpy和strcat，不要忘记在字符串末尾填0
  
##### stdlib.c

简单堆区的实现，只需要定义一个较大长度的数组作为堆区即可
  
##### stdio.c

c语言变长参数的实现，主要是通过va_start、va_end、va_arg这三个宏和va_list这个类型实现的

va_start ( va_list ap, paramN);
参数：ap: 可变参数列表地址   paramN: 确定的参数
功能：初始化可变参数列表（把函数在 paramN 之后的参数地址放到 ap 中）

void va_end ( va_list ap );
参数：ap: 可变参数列表地址
功能：关闭初始化列表（将 ap 置空）

type va_arg ( va_list ap, type );
参数：ap: 可变参数列表地址   type：下一个参数的类型
功能：返回下一个参数的值。

va_list ：存储参数的类型信息

对于核心函数vsprintf的实现，最后注意给格式化的字符串末尾添加0

#### 实现驱动程序

说实话，之前对计算机的学习，我对cpu如何指示外设工作的原理一直不是很明白，直到看完了jyy老师的os课，写了这里的pa实验，我才算是真正明白了

##### 串口

putstr的实现，底层就是往串口的指定寄存器SERIAL_PORT写入要输出的字符串ascii码

##### 时钟

从时钟相关寄存器RTC_ADDR读取出现在离开机时的时间戳，0~3字节是低4位，4~7字节是高4位

##### 键盘

从键盘相关寄存器KBD_ADDR可以读取出4字节键盘码，第15位（从0开始）代表是按下还是弹回，其余则代表键位

##### 显卡

VGACTL_ADDR的2个字节代表显示器高度，VGACTL_ADDR+2的2个字节代表显示器宽度，FB_ADDR代表显存首地址，SYNC_ADDR则代表是否将当前显存内容写入显示器

注意，在写入显存的时候，使用memcpy整行的拷贝要快于单个字节的拷贝

##### 总结

从上面的驱动程序编写原理我们可以知道，无论是端口io还是内存映射io，cpu要控制外设，只需要通过相关的in out或者访存指令去写入或者读取设备相关寄存器，即可实现对设备的访问！

在运行benchmark打分程序时，我发现我的程序出现了访存错误，居然访存地址为0，我觉得cputest通过了之后不会出错，但是看到difftest之后还是测试了下，发现是isa-def.h文件里的

```
int32_t  imm10_5  : 6;
int32_t  imm12    : 1;

应该要是
      
uint32_t  imm10_5  : 6;
uint32_t  imm12    : 1;
```

这是copy-paste引发的bug！！！

成功解决后，我发现我的打分只有不到1，正常应该是三百。后来发现是自己的虚拟机问题，vmvare如果锁屏后重新唤醒，跑分会格外低，原因我没有深究

最后说说pa2的基础设施，对于ftrace，即函数调用栈的打印，我觉得很棒，虽然我没有实现，但是我在这讲述下实现思路

+ 读取elf文件的符号表与字符串表，将符号表中代表函数的信息：函数名称，起始地址，长度存储下来

+ 在每次执行跳转指令jal和jarl时候，我们通过信息记录要跳转地址所在的函数


## PA3

在这个实验中，我们加入了nanos-lite和navy-apps，现在完整的计算机系统已经是基本形成

+ nemu 计算机硬件

+ abstract-machine 裸机库

+ nanos-lite 以abstract-machine为运行时环境，运行在nemu上的简易批处理os

+ navy-apps 运行在nanos-lite上的简易libc库和用户态程序

#### 实现特权指令

##### csrrw

记控制状态寄存器csr中的值为t，把rs1的结果写入csr，再把t的值写入rd

##### csrrs

记控制状态寄存器csr中的值为t，把t和rs1按位或的结果写入csr，再把t的值写入rd

##### ecall

硬件需要执行以下操作

将pc保存在epc寄存器，将异常号加入到mcause寄存器，将下一指令地址设置为mtvec（省去状态切换）

##### eret

将下一指令地址设置为epc

#### 保存上下文

在trap.S中，我们需要保存上下文，通过c语言结构体和内存布局的对应关系，对context结构体进行相应修改即可

最后我们成功在nemu上运行nanos-lite

#### 加载程序

```
uintptr_t loader(PCB *pcb, const char *filename) {
  打开elf文件
  
  读取elf header
  
  进行相关检查，诸如魔数，文件类型
  
  将可以加载的seg从elf文件加载进内存的特定位置
  
  for (i = 0, off = elf_header.e_phoff; i < elf_header.e_phnum; i++, off += sizeof(Elf32_Phdr)) {

    读取pro header信息
    
    if (pgm_header.p_type != PT_LOAD) { continue; }  // 如果不能加载进入内存，继续

    load_seg(fd, pgm_header.p_vaddr, pgm_header.p_offset, pgm_header.p_filesz, pgm_header.p_memsz);  // 加载入内存
  }
  
  return elf_header.e_entry;  // 返回可执行文件入口地址
}
```

#### 系统调用的实现

首先用户态程序入口是start，start是汇编写的函数，它直接调用call_main函数，call_main函数进行相关初始化，调用main函数，最后调用exit系统调用退出

##### yield系统调用

很简单

##### exit系统调用

很简单

##### brk系统调用

注意，sbrk是库函数，不是系统调用。

首先说明end符号地址位置，这是用户进程的地址空间

```

text

rodata

data

bss    
      <--end
heap

stack

```

对于sbrk库函数的实现，可以通过end符号获取堆区起始位置，通过brk系统调用扩大堆内存，对堆区进行管理

##### gettimeofday系统调用

很简单

##### execv系统调用

调用load调用加载相关可执行文件即可


#### 虚拟文件系统

vfs，这实际上是一种面向对象思想的运用，底层不同的文件系统，不同设备（磁盘，摄像机，录音机等）套接字，管道。均可抽象成可以读写的对象

而对于nanos-lite这个批处理os，对文件系统的处理很简陋，由于我受过文件系统的训练，所以不详细解释，这里重点讲述读写系统调用的思路

##### read系统调用

判断读取的文件是否有回调函数（设备一般就是），没有则读磁盘，有则调用回调函数

##### write系统调用

判断写入的文件是否有回调函数（设备一般就是），没有则写磁盘，有则调用回调函数

下面是各个回调函数

##### 标准输出（串口）的回调函数

write：调用串口驱动程序往串口输出字节流即可

##### 键盘回调函数

read：读取键盘驱动程序中的相关结构体信息，按指定格式输出即可

##### 显示器回调函数

write：调用驱动程序相关接口，写入指定显存即可


#### 运行仙剑奇侠传等用户态程序

对于libndl和libminSDL目录下的用户态相关SDL的库函数实现，我觉得和计算机系统几乎毫无关系，所以，这里不费口舌（注意要在终端程序中加入execv系统调用加载指定程序）

最后成功运行仙剑奇侠传





## 最后的感想












