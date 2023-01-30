# ICS2021

这里记录我写ICS2021的思路和收获，pa4没有写，因为我觉得pa4和os相关，而我已经接收了os相关的实验训练

### PA0

pa0阅读实验讲义，我提取出如下几个cser的基本素质，有些是我不完全具备的

+ 阅读英文

+ 备份重要数据

+ 光看说明书不做不能学会

+ STFW RTFM RTFSC（注意阅读的仔细）

+ 使用工具提高效率（vim）

+ 注意提问艺术

+ 使用linux

### PA1

PA1主要是基础设施，即调试代码的实现

##### 单步执行

将执行次数传入函数cpu_exec即可

##### 打印程序状态

根据输入参数打印寄存器内容或者监视点信息即可

##### 扫描内存

调用内存读取接口vaddr_read打印指定内存值即可，注意格式美观

##### 表达式求值

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

##### 设置监视点

+ 注意在watchpoint结构体中加入寄存器或者指定内存的old_value字段

+ 将新建节点加入全局链表

+ 定义CONFIG_WATCHPOINT宏，若此宏被定义，则在trace_and_difftest函数中将监视点当前值和old值进行对比，若不同则停下

##### 删除监视点

+ 和链表删除类似

### PA2

pa2会遇到很多宏，这方面注意不要强行说理解所有宏的展开内容。

##### 指令的完善

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

##### 运行时环境，实现常见库函数

+ 

##### 实现驱动程序

+ 时钟

+ 键盘

+ 显卡


### PA3
