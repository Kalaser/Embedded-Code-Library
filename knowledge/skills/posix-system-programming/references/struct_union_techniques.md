# 写了十年C，才悟出的结构体与联合体骚操作

> 整理风格对齐《写了十年C，才悟出的循环与分支骚操作》，聚焦嵌入式/系统编程中结构体与联合体的实战用法。

---

## 引言

结构体和联合体是C语言里最朴素也最被低估的两个东西。教科书告诉你"结构体是把不同类型打包在一起，联合体是共用一块内存"，但真正在驱动、协议解析、内核代码里摸爬滚打过的人都知道——这两样东西玩到极致，能做出接近"面向对象"、接近"零成本抽象"的效果。

本文不讲`struct foo { int a; };`这种入门写法，只讲那些在寄存器操作、协议解析、内核链表、内存布局优化里天天能见到的"老手写法"。

---

## 一、位域（Bit-Field）：寄存器操作的减法

### 新手写法

```c
/* 手动移位掩码，写一个配置字段要算半天 */
#define REG_ENABLE_MASK   0x01
#define REG_MODE_MASK     0x06
#define REG_MODE_SHIFT    1
#define REG_SPEED_MASK    0x78
#define REG_SPEED_SHIFT   3

uint8_t reg = 0;
reg |= (1 << 0);                          /* 使能位 */
reg |= ((2 & 0x3) << REG_MODE_SHIFT);     /* 模式位 */
reg |= ((5 & 0xF) << REG_SPEED_SHIFT);    /* 速度位 */
```

### 老手写法

```c
typedef struct {
    uint8_t enable : 1;   /* bit0     使能 */
    uint8_t mode   : 2;   /* bit1-2   工作模式 */
    uint8_t speed  : 4;   /* bit3-6   速度档位 */
    uint8_t rsvd   : 1;   /* bit7     保留 */
} reg_ctrl_t;

reg_ctrl_t ctrl = {
    .enable = 1,
    .mode   = 2,
    .speed  = 5,
};
```

**要点：**

- 位域把"移位+掩码"这套体力活交给编译器，读代码的人一眼就知道每个字段管什么
- 常用于MCU寄存器映射、协议头解析（如TCP/IP头里的标志位）
- **坑**：位域的内存布局（从低位还是高位开始分配）是**实现定义**的，不同编译器/字节序下不保证一致，跨平台通信协议**不要**用位域直接当协议格式，应该用显式的移位掩码或者只在单一确定平台内部使用
- 位域底层类型建议用无符号类型（`uint8_t`/`uint32_t`），避免符号扩展带来的意外行为（这也是`coding_standard.md`规则15.6的要求）
- 相邻位域如果类型不同，编译器可能不会打包到同一个存储单元，想省内存要保证类型一致

### 位域直接映射硬件寄存器

```c
/* 假设某外设的控制寄存器手册定义如下 */
typedef union {
    struct {
        uint32_t enable    : 1;
        uint32_t clk_sel   : 3;
        uint32_t div       : 8;
        uint32_t rsvd      : 20;
    } bits;
    uint32_t reg;
} clk_ctrl_reg_t;

volatile clk_ctrl_reg_t *p_reg = (volatile clk_ctrl_reg_t *)CLK_CTRL_ADDR;

/* 按字段写，可读性拉满 */
p_reg->bits.enable  = 1;
p_reg->bits.clk_sel = 2;

/* 需要整体读写寄存器时（比如原子写一整个32位），走union的另一个成员 */
uint32_t snapshot = p_reg->reg;
```

**注意**：跨字段的寄存器操作要小心"读-改-写"不是原子操作，中断里/多核场景下要考虑加锁或者用`volatile`+适当的内存屏障（结合`coding_standard.md`第19章 Volatile与内存屏障规范一起看）。

---

## 二、联合体（Union）：类型双关与协议解析

### 2.1 用union做协议帧的"透视"

网络/串口协议解析最经典的写法：把裸字节buffer"投影"成有意义的结构体，省掉一次一次`memcpy`+手动拼字段。

```c
typedef struct {
    uint8_t  header;
    uint8_t  cmd;
    uint16_t length;
    uint8_t  payload[64];
} __attribute__((packed)) proto_frame_t;

typedef union {
    proto_frame_t frame;
    uint8_t       raw[sizeof(proto_frame_t)];
} proto_packet_t;

/* 收到裸字节，直接当结构体用 */
proto_packet_t pkt;
recv(fd, pkt.raw, sizeof(pkt.raw), 0);

if (pkt.frame.header == FRAME_MAGIC) {
    process_cmd(pkt.frame.cmd, pkt.frame.payload, pkt.frame.length);
}
```

**要点：**

- `__attribute__((packed))`（或`#pragma pack`）去掉结构体内存对齐的padding，保证结构体布局跟协议字节流严格一致——但这样做会牺牲访问性能（某些架构上未对齐访问要软件模拟，甚至直接触发异常），嵌入式MCU上要评估目标平台是否支持非对齐访问
- 字节序问题：`length`这种多字节字段，如果协议规定网络字节序（大端），而MCU是小端，直接union投影出来的值是错的，必须显式`ntohs`/`htons`转换，**不要**指望union帮你处理字节序
- 这正是`coding_standard.md`规则10.7强调的：**联合体使用时必须明确区分成员类型**，用哪个成员读、哪个成员写，什么时候整体投影、什么时候拆开处理，要在注释里写清楚

### 2.2 带Tag的union——手搓一个"可辨识联合"（tagged union）

C语言没有C++的`std::variant`，但可以自己搭一个"类型安全"的联合体：

```c
typedef enum {
    VAL_TYPE_INT,
    VAL_TYPE_FLOAT,
    VAL_TYPE_STRING,
} value_type_e;

typedef struct {
    value_type_e type;      /* tag：告诉你union里存的是什么 */
    union {
        int32_t  i_val;
        float    f_val;
        char     s_val[32];
    } data;
} tagged_value_t;

void print_value(const tagged_value_t *v)
{
    switch (v->type) {
        case VAL_TYPE_INT:
            printf("int: %d\n", v->data.i_val);
            break;
        case VAL_TYPE_FLOAT:
            printf("float: %f\n", v->data.f_val);
            break;
        case VAL_TYPE_STRING:
            printf("string: %s\n", v->data.s_val);
            break;
    }
}
```

**优势：**

- 用一个enum tag强制调用者"先看类型再取值"，比裸union安全得多
- 事件队列、消息总线、配置项存储这类"一种容器装多种数据"的场景非常常用（比如[[sunny-msgbus]]这类消息总线的payload设计）
- switch里记得写`default`兜底（对应`coding_standard.md`规则11.5），防止tag出现意外值时读到未定义的联合体成员

### 2.3 类型双关（Type Punning）要走union，不要走指针强转

```c
/* 危险写法：通过指针强转做类型双关，违反严格别名规则(strict aliasing)，
   优化编译器下行为未定义 */
float f = 3.14f;
uint32_t bits = *(uint32_t *)&f;   /* 不要这样写 */

/* 正确写法：通过union做类型双关，C标准允许（C11 6.5.2.3注7） */
union {
    float    f;
    uint32_t u;
} pun;
pun.f = 3.14f;
uint32_t bits = pun.u;
```

**要点**：这是少数几个C标准明确认可、可以安全用union做的"非常规"操作之一。想看一个float的裸bit（比如实现快速反平方根之类的算法），用union而不是指针强转。

---

## 三、匿名结构体/联合体：去掉一层`.`

C11开始，匿名的嵌套struct/union成员可以直接访问，不用多打一层成员名：

### 老写法（C11之前）

```c
typedef struct {
    int type;
    union {
        int   i_val;
        float f_val;
    } data;   /* 必须多写一层 .data */
} value_t;

value_t v;
v.data.i_val = 10;
```

### C11匿名union写法

```c
typedef struct {
    int type;
    union {          /* 匿名union，没有成员名 */
        int   i_val;
        float f_val;
    };
} value_t;

value_t v;
v.i_val = 10;        /* 直接访问，少一层嵌套 */
```

**注意**：`coding_standard.md`规则10.6提到"禁止使用匿名结构体/联合体（C11前）"——如果你的工具链确认支持C11（大部分现代arm-none-eabi-gcc都支持），可以用；如果目标平台/编译器版本不确定支持到什么标准，先查`-std=`编译选项，避免踩坑。

---

## 四、柔性数组成员（Flexible Array Member, FAM）

C99允许结构体最后一个成员声明为不定长数组，实际大小在`malloc`时才确定：

```c
typedef struct {
    uint32_t len;
    uint8_t  data[];   /* 柔性数组成员，必须是最后一个成员，且不能是唯一成员 */
} dyn_buffer_t;

dyn_buffer_t *create_buffer(uint32_t size)
{
    /* 一次malloc同时分配头部+数据区，避免头和数据分两块内存、两次malloc/free */
    dyn_buffer_t *buf = malloc(sizeof(dyn_buffer_t) + size);
    if (buf == NULL) {
        return NULL;
    }
    buf->len = size;
    return buf;
}

/* 用完一次free搞定，不用分别释放头和数据 */
free(buf);
```

**优势：**

- 比"结构体里放一个指针，指针再单独malloc一块内存"少一次内存分配、少一次指针跳转，缓存局部性也更好（对应`coding_standard.md`第27章"合理使用缓存行和局部性"）
- 常见于内核/驱动里变长消息、变长包的场景

**坑：**

- 柔性数组成员**不能**用`sizeof`算出实际数据大小，`sizeof(dyn_buffer_t)`只包含头部
- 结构体里不能有柔性数组成员的数组（比如不能`dyn_buffer_t arr[10]`，因为每个元素大小不固定）
- 老代码里有时会看到"伪柔性数组"写法`uint8_t data[1];`（C99前的权宜之计），新代码优先用标准柔性数组`data[]`

---

## 五、container_of宏：从成员指针反推结构体指针

Linux内核/NuttX这类系统代码里到处都是的经典宏，写侵入式链表（intrusive linked list）的核心技巧：

```c
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
```

### 使用场景

```c
typedef struct {
    struct list_node node;   /* 侵入式链表节点，嵌在业务结构体里 */
    int   id;
    char  name[32];
} device_t;

/* 链表遍历函数只知道 struct list_node*，不知道外层的 device_t */
void list_foreach(struct list_node *head, void (*visit)(struct list_node *))
{
    struct list_node *p = head;
    while (p != NULL) {
        visit(p);
        p = p->next;
    }
}

void print_device(struct list_node *node)
{
    /* 从 node 指针反推出外层 device_t 指针 */
    device_t *dev = container_of(node, device_t, node);
    printf("device: %s\n", dev->name);
}
```

**要点：**

- 好处是链表节点结构体（`struct list_node`）完全不需要知道自己被嵌在什么业务结构体里，实现了"通用容器代码 + 业务结构体"的解耦，一套链表代码可以给任意结构体复用（这就是C语言里"泛型"的土办法）
- `offsetof`是标准库宏（`<stddef.h>`），计算某个成员在结构体里的字节偏移
- **前提**：结构体成员的内存布局必须是确定的、不能有编译器插入的意外行为，这也是为什么`coding_standard.md`规则10.5要求关注结构体对齐和填充——用container_of的结构体，成员顺序变了、编译器优化布局了，偏移量跟着变，宏依然安全（因为offsetof是编译期算出来的），但**手写魔法数字代替offsetof就危险了**
- NuttX、Linux内核的队列/链表/定时器实现大量用这个技巧，如果你在NuttX里看到`container_of`或类似宏，就是这个套路

---

## 六、指定初始化器与结构体常量表

C99指定初始化器（Designated Initializer）配合结构体数组，是写"配置表/命令表/状态表"的标准写法：

### 老写法：按位置初始化，改一个字段要数半天位置

```c
typedef struct {
    int         cmd_id;
    const char *name;
    int         (*handler)(void *arg);
    int         min_args;
} cmd_entry_t;

static const cmd_entry_t cmd_table[] = {
    {1, "start", cmd_start, 0},
    {2, "stop",  cmd_stop,  0},
    {3, "config", cmd_config, 2},
};
```

### 老手写法：指定初始化器，字段名说明一切，顺序无关

```c
static const cmd_entry_t cmd_table[] = {
    [0] = { .cmd_id = 1, .name = "start",  .handler = cmd_start,  .min_args = 0 },
    [1] = { .cmd_id = 2, .name = "stop",   .handler = cmd_stop,   .min_args = 0 },
    [2] = { .cmd_id = 3, .name = "config", .handler = cmd_config, .min_args = 2 },
};
```

**优势：**

- 新增/删除字段时不用数逗号数位置，编译器帮你对齐
- 没写的字段自动零初始化（数值成员是0，指针成员是NULL），比手动补齐所有位置的`0`/`NULL`更不容易漏
- 这正是`coding_standard.md`规则10.2推荐的"结构体初始化使用C99指定初始化器"

**进阶**：数组下标也可以指定初始化，`[0]`/`[1]`这种写法在"数组下标本身就是枚举/命令码"的场景下（比如上面`cmd_table[cmd_id]`直接索引）非常实用，还能顺手让稀疏数组的空洞自动补0。

---

## 七、内存对齐与padding优化：结构体成员重排省内存

编译器会按对齐规则在结构体成员间插入padding，成员声明顺序不同，同样的字段占用的总内存可能差好几倍。

### 排列不佳的写法

```c
typedef struct {
    uint8_t  a;   /* 1字节 + 3字节padding（因为下一个是4字节对齐的int） */
    uint32_t b;   /* 4字节 */
    uint8_t  c;   /* 1字节 + 3字节padding（结构体整体要按最大对齐成员对齐） */
} bad_layout_t;   /* sizeof = 12 */
```

### 按大小从大到小排列

```c
typedef struct {
    uint32_t b;   /* 4字节 */
    uint8_t  a;   /* 1字节 */
    uint8_t  c;   /* 1字节 + 2字节padding（凑到4字节整体对齐） */
} good_layout_t;  /* sizeof = 8 */
```

**要点：**

- 经验法则：**成员按类型大小从大到小声明**，能让编译器自动填的padding最少
- 用`sizeof`实际测一下，不要凭感觉，不同架构的对齐规则可能不同
- 嵌入式RAM紧张的场景，结构体数组（比如传感器采样数据、消息队列节点）稍微调一下成员顺序，可能省下可观的内存
- 这属于`coding_standard.md`规则10.5"注意结构体对齐和填充"的具体落地手法，跟第27章性能优化规范里"合理使用缓存行和局部性"也有关系——更紧凑的结构体意味着同样大小的cache line能塞进更多有效数据

### AoS vs SoA：数组的结构体 vs 结构体的数组

```c
/* AoS：Array of Structs，直觉写法 */
typedef struct { float x, y, z; } point_t;
point_t points[1000];

/* SoA：Struct of Arrays，批量处理时缓存命中率更高 */
typedef struct {
    float x[1000];
    float y[1000];
    float z[1000];
} point_soa_t;
```

如果代码经常"只批量处理所有点的x"（比如SIMD向量化、批量传感器数据处理），SoA能让同一次内存访问加载的都是有效数据，不像AoS那样每次都要跳过y、z——这是性能敏感路径（第27章）才需要考虑的优化，普通业务代码用AoS更直观，不要过早优化（原则27.1）。

---

## 八、函数指针 + 结构体：C语言里手搓"面向对象"

### 8.1 vtable模式实现多态

```c
/* "基类"：一组操作接口 */
typedef struct {
    int  (*open)(void *self);
    int  (*read)(void *self, void *buf, size_t len);
    void (*close)(void *self);
} device_ops_t;

/* "子类"：具体设备 + 关联的操作表 */
typedef struct {
    device_ops_t *ops;
    int           fd;
} uart_device_t;

static int  uart_open(void *self)  { /* ... */ return 0; }
static int  uart_read(void *self, void *buf, size_t len) { /* ... */ return 0; }
static void uart_close(void *self) { /* ... */ }

static device_ops_t uart_ops = {
    .open  = uart_open,
    .read  = uart_read,
    .close = uart_close,
};

/* 调用方完全不需要知道底层是uart还是spi，面向接口编程 */
void device_use(device_ops_t *ops, void *self)
{
    ops->open(self);
    ops->close(self);
}
```

**要点：**

- 这就是C语言实现"接口/多态"的标准方式，Linux内核`file_operations`、NuttX的驱动框架（`struct file_operations`风格）都是这个套路
- 函数指针表建议声明为`static const`（如果运行时不会改），编译器可以把它放到只读段，还能帮你防止意外篡改
- 跟`coding_standard.md`第22章"API设计与版本管理规范"结合：这种ops表本身就是一种"接口"，新增操作时要考虑二进制兼容性（新增字段放最后，别插中间）

### 8.2 用结构体嵌套模拟"继承"

```c
typedef struct {
    int id;
    char name[32];
} base_device_t;

typedef struct {
    base_device_t base;   /* 把"基类"结构体放在第一个成员，模拟继承 */
    int baud_rate;
} uart_device2_t;

/* 关键技巧：因为base是第一个成员，
   uart_device2_t* 和 base_device_t* 的地址是同一个，可以互相转换 */
void print_base_info(base_device_t *dev)
{
    printf("id=%d name=%s\n", dev->id, dev->name);
}

uart_device2_t uart = { .base = {.id = 1, .name = "uart0"}, .baud_rate = 115200 };
print_base_info((base_device_t *)&uart);   /* 安全：base是第一个成员，地址对齐 */
```

**注意**：这个技巧成立的前提是"基类"结构体必须是**第一个成员**（C标准保证结构体第一个成员的地址和结构体本身地址相同），如果放在中间或最后，指针转换就是未定义行为。这也是为什么很多C风格"面向对象"框架的编码约定里，会强制要求"基类成员必须放第一位"。

---

## 九、总结表

| 技巧 | 适用场景 | 优势 | 风险/坑 |
|------|---------|------|--------|
| 位域 | MCU寄存器、协议标志位 | 免去手动移位掩码，可读性好 | 内存布局实现定义，不能跨平台通信直接用 |
| union协议投影 | 串口/网络协议解析 | 省去手动拼字段 | 需配合packed属性和字节序转换，注意非对齐访问 |
| tagged union | 事件队列、消息总线payload | 比裸union多一层类型安全 | switch必须有default兜底 |
| union类型双关 | 查看float/double裸bit | 标准认可，比指针强转安全 | 仅限union方式，指针强转是UB |
| 匿名struct/union | 简化多层嵌套访问 | 少写一层成员名 | 需C11支持，注意编译器/标准版本 |
| 柔性数组成员 | 变长消息/变长包 | 一次malloc，缓存局部性好 | 不能sizeof取数据长度，不能整个数组化 |
| container_of | 侵入式链表、内核风格容器 | 容器代码与业务结构体解耦 | 依赖offsetof，成员顺序变化需重新编译 |
| 指定初始化器 | 命令表、配置表 | 字段自解释，未写字段自动清零 | 需C99支持 |
| 成员重排/对齐优化 | 大批量数据结构、RAM紧张场景 | 减少padding，省内存 | 需要用sizeof实测，不同架构结果不同 |
| AoS vs SoA | 批量数值计算/SIMD | 提升缓存命中率 | 会牺牲代码直观性，非性能热点不建议用 |
| vtable模式 | 驱动框架、可插拔实现 | 实现接口/多态 | 新增字段要放最后，注意ABI兼容 |
| 结构体嵌套模拟继承 | 设备/对象层级建模 | "基类"字段和函数复用 | 基类成员必须是第一个成员 |

---

## 写在最后

结构体和联合体这两个"傻瓜积木"，用得好，能在没有C++/没有虚函数表语法糖的情况下，搭出接口抽象、多态、变长容器、协议编解码这些"高级语言"才有的能力。

这些写法不是为了炫技，而是嵌入式/系统编程里真实存在的性能约束（没有堆、没有GC、寄存器要精确控制）逼出来的解法。用之前先想清楚：**这段代码是否真的需要这种复杂度**，团队协作的业务代码，可读性和`coding_standard.md`里的规范优先；驱动、协议栈、内核风格的底层代码，这些技巧就是本行的基本功。

**文档结束**
