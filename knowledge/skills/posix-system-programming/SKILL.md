---
name: posix-system-programming
description: Use when designing, implementing, reviewing, debugging, or porting C/C++ system software that depends on POSIX interfaces or operating-system services. Covers POSIX.1-2024 research, API contracts, file and device I/O, polling, pthreads, synchronization, clocks and timers, message queues, sockets, process/task control, and Linux/NuttX compatibility. Also use when a task requires distinguishing POSIX requirements from an OS-specific extension, build configuration, driver capability, or project abstraction.
license: Internal use; source references retain their original terms and provenance.
compatibility: Works with tool-enabled coding agents that can inspect project files and consult official online documentation.
metadata:
  version: "1.0.0"
  language: "zh-CN"
  domain: "C/C++ system programming"
  proficiency: "L1-L4"
---

# POSIX System Programming Skill

## 1. Mission

本 Skill 为 C/C++ 系统软件开发提供一套可重复执行的 POSIX 标准查阅、API 语义分析、目标操作系统核验、平台抽象设计和工程验证流程。

本 Skill 的目标不是复述 POSIX API 清单，也不是将某个操作系统的行为推广为通用规则，而是要求每项工程结论都能区分并追溯到以下依据：

1. **标准要求**：POSIX 规范性条文定义了什么。
2. **平台实现**：目标操作系统文档和源码实际提供了什么。
3. **工程配置**：当前构建配置启用了什么能力。
4. **运行验证**：目标环境中的测试实际证明了什么。
5. **设计建议**：基于上述证据提出的工程方案。

不得将这五类信息混为一谈。

## 2. Activation Rules

当任务满足以下任一条件时启用本 Skill：

- 需要查询、解释或使用 POSIX API、头文件、类型、宏、错误码或行为约束。
- 需要实现文件、设备、串口、Socket 或其他基于文件描述符的 I/O。
- 需要设计线程、互斥锁、条件变量、信号量、消息队列或进程间通信。
- 需要处理超时、时钟源、定时器、阻塞/非阻塞行为或 I/O 多路复用。
- 需要评估 Linux、NuttX 或其他 RTOS 的 POSIX 兼容性。
- 需要把 OS 原生接口封装为跨平台 OS Abstraction Layer（OSAL）。
- 需要审查代码是否正确处理 API 返回值、资源生命周期、并发和平台差异。

如果任务仅涉及 C 语言语法、数据结构或项目格式规范，应优先使用相应配套资料，不要强行套用 POSIX 分析流程。涉及多个领域时，可以组合使用本 Skill 和对应参考文件。

## 3. Scope and Boundaries

### 3.1 In Scope

| 能力域 | 常见接口或主题 | 典型工程问题 |
|---|---|---|
| 文件与设备 I/O | `open`、`read`、`write`、`close`、`fcntl`、`ioctl` | 文件描述符、设备节点、部分读写、错误恢复 |
| I/O 多路复用 | `poll`、`select`、`pselect` | 事件循环、多设备监听、就绪通知 |
| 线程与同步 | `pthread_*`、`sem_*` | 线程生命周期、共享资源、等待和唤醒 |
| 消息与 IPC | `mq_*`、Pipe、共享内存 | 生产者/消费者、队列满、进程间通信 |
| 时间与定时器 | `clock_gettime`、`nanosleep`、`timer_*` | 超时、周期任务、运行时间测量 |
| 网络 | `socket`、`bind`、`listen`、`accept`、`connect`、`send`、`recv` | 连接管理、事件驱动收发、断线处理 |
| 任务/进程控制 | `fork`、`exec`、`wait`、`posix_spawn` | 任务创建、退出状态和资源继承 |
| 可移植性 | 特性宏、可选接口、实现定义行为 | 跨 OS 移植和能力矩阵 |

### 3.2 Out of Scope

POSIX 不替代以下资料：

- MCU 寄存器、时钟树、中断控制器、DMA：使用芯片参考手册、架构手册和 BSP 文档。
- UART、USB、Wi-Fi、Bluetooth 等硬件或协议细节：使用控制器手册、协议规范和驱动文档。
- C 语言本身及标准库：使用 ISO C 标准和项目 C 编码规范。
- C++ 语言及标准库：使用 ISO C++ 标准和项目 C++ 规范。
- LVGL 或其他 GUI 框架：使用相应框架文档和项目 UI 规范。

如果问题超出 POSIX 范围，应明确指出边界，并转向正确的规范或平台资料。

## 4. Source-of-Truth Hierarchy

查证时按问题类型选择资料，不得把单一来源作为所有问题的答案：

1. **POSIX 规范语义**：The Open Group Base Specifications Issue 8 / POSIX.1-2024。<https://pubs.opengroup.org/onlinepubs/9799919799/>
2. **目标 OS 的官方支持情况**：对应 OS 版本的官方文档。
3. **当前工程配置**：实际使用的配置文件、Kconfig、编译选项和构建脚本。
4. **实现细节**：对应版本源码、头文件和驱动代码。
5. **实际行为**：目标平台上的可复现测试结果。
6. **项目约束**：项目编码规范、架构文档和已有接口契约。

出现冲突时：

- 解释 POSIX 语义时，以适用版本的规范性条文为准。
- 判断某个平台是否支持时，以该平台对应版本的官方文档、源码、配置和实测为依据。
- 判断项目是否允许某种写法时，以项目生效的编码规范和架构约束为依据。
- 任何无法查证的结论必须标注为“待验证”，不得用推测补齐。

## 5. POSIX Document Map

POSIX.1-2024 / Issue 8 的主要组成部分：

| 分册 | 名称 | 主要职责 | 何时查阅 |
|---|---|---|---|
| XBD | Base Definitions | 基础定义、通用术语、头文件、类型和宏 | 查询类型、常量、头文件及通用约定 |
| XSH | System Interfaces | 系统接口及函数行为规范 | 查询具体系统 API 的原型、行为和错误条件 |
| XCU | Shell and Utilities | Shell 语言及标准工具 | 编写可移植 Shell 脚本和标准工具流程 |
| XRAT | Rationale | 设计背景和解释性说明 | 需要理解接口设计理由时查阅 |

查阅原则：先定位 XSH 中的接口，再按需回查 XBD；涉及 Shell/Utilities 时查 XCU；只有需要理解设计背景时再查 XRAT。XRAT 是说明性材料，不得代替规范性条文。

## 6. Required Workflow

每次执行 POSIX 相关任务时，按以下阶段开展。简单查询可以精简输出，但不得跳过与结论相关的证据核验。

### Phase A — Understand the Task

收集或从工程中确认：

- 目标功能和预期行为。
- 操作系统、版本、芯片/架构和工具链。
- 构建形态及实际配置。
- 运行上下文：线程、任务、中断、回调或主循环。
- 并发模型及资源预算：RAM、栈、优先级、句柄数量和时延要求。
- 是否要求跨平台，以及目标平台清单。

缺少信息时，优先从项目文件中查找。若仍无法确定，列出明确假设；不要把假设写成事实。

### Phase B — Locate the Standard Contract

对目标 API 查阅 XSH 对应条目，并按需查阅 XBD。至少记录：

- 标准名称和版本、章节/条目、规范链接。
- 函数原型和必需头文件。
- 参数方向、有效范围、长度单位和所有权。
- 成功、失败、EOF、部分完成等返回语义。
- 错误条件及错误码的获取方式。
- 阻塞、非阻塞、超时和信号中断语义。
- 并发、线程安全及异步信号安全要求（如适用）。
- 资源创建、共享、关闭和销毁规则。
- 可选能力、特性测试宏、实现定义或未指定行为（如适用）。

不要根据函数名推断语义。标准条目没有明确说明的内容，应标注“该条目未说明”，再寻找相关通用定义或平台文档。

### Phase C — Verify the Target OS

逐项核实目标系统：

1. 官方文档是否列出该接口及其限制。
2. 项目所用 OS 版本是否与文档版本对应。
3. 当前 `.config`、`defconfig`、Kconfig、编译选项是否启用所需能力。
4. 头文件声明、源码实现和驱动是否支持目标用法。
5. 是否存在返回值、资源共享、超时、调度或错误语义上的偏离。
6. 是否需要目标板测试才能确认实际行为。

对 NuttX，特别区分 POSIX `pthread` 与 NuttX 原生 Task API。`task_create`、`task_delete` 等原生接口不得描述为 POSIX 标准接口。还应结合 NuttX 对应版本文档及当前配置判断能力，不能将某份“latest”或历史汇总文档直接视为当前工程事实。

### Phase D — Design the Integration

先定义模块边界，再确定是否需要 OSAL：

```text
Application / Business Logic
              |
              v
       Framework Interface
              |
              v
        Platform Adapter
          /          \
         v            v
     POSIX API    OS-native API
          \          /
           v        v
          Operating System
```

设计要求：

- 上层业务依赖项目接口，不在业务代码中散布 OS 宏和条件编译。
- 平台适配层封装标准接口与 OS 扩展，并负责错误码转换。
- 只有存在实际复用、移植或测试隔离收益时才增加抽象，不为单次调用制造无意义包装。
- 平台专有能力必须显式命名、记录限制并提供降级策略（如需要）。
- 明确资源所有权、线程归属、关闭顺序、失败回滚和并发访问协议。

### Phase E — Implement and Validate

根据风险选择测试，至少考虑：

- 正常路径、失败路径和资源耗尽。
- 空指针、零长度、最大长度和非法参数。
- 部分 I/O、EOF、`EINTR`、`EAGAIN`/`EWOULDBLOCK` 和超时（适用时）。
- 线程竞争、锁顺序、线程退出及对象销毁（适用时）。
- 断连、队列满、缓冲区溢出和异常重启（适用时）。
- 不同构建配置和目标平台的编译验证。
- 目标硬件的时序、栈空间、内存占用和驱动事件能力。

记录测试环境、配置、步骤、预期结果、实际结果和未覆盖风险。

## 7. Mandatory Engineering Rules

本 Skill 与项目编码规范衔接，采用“原则 → 规则 → 建议”体系。项目正式规则高于本 Skill 的一般建议。

### Principles

**P1 — Standard Semantics First**  
使用适用版本的规范性条文确定 POSIX 接口语义，不得把某个 OS 的行为直接推广为通用规则。

**P2 — Verify Implementation Capability**  
标准定义接口不代表目标 OS、构建配置和驱动必然完整提供该能力。必须核验实现和配置。

**P3 — Make Platform Differences Explicit**  
平台扩展、部分实现、行为偏离和不可用能力必须显式记录，并限制在适当的适配边界内。

### Rules

**R1 — Distinguish Error-Reporting Conventions**  
不得假设所有 API 都通过 `-1` 和 `errno` 报错。普通系统接口、pthread 函数及 OS 原生扩展可能采用不同约定；必须按具体接口规范处理。

**R2 — Handle Partial I/O**  
不得假设一次 `read()` 或 `write()` 一定完成全部请求。根据返回字节数推进缓冲区，并处理 EOF、错误和中断。需要完整协议帧时，必须在协议层实现累计收发和有界超时。

**R3 — Retry Only When Semantically Valid**  
不得对所有错误无条件重试。区分 `EINTR`、`EAGAIN`/`EWOULDBLOCK`、超时、连接关闭及不可恢复错误；重试必须有明确条件和边界。

**R4 — Define Resource and Thread Lifecycles**  
线程、锁、条件变量、信号量、消息队列、文件描述符和 Socket 必须定义创建者、使用者、关闭者及销毁时机。所有失败路径必须有资源回滚策略。

**R5 — Define a Synchronization Protocol for Shared State**  
多线程共享状态必须有明确的互斥、原子或消息传递协议。不得仅根据变量宽度或单条 C 语句推断并发安全。

**R6 — Select Clock Semantics Deliberately**  
计算持续时间、通信超时和任务期限时，应核对所选时钟的规范语义。通常应评估单调时钟；不得未经分析就用可能受校时影响的日历时钟计算耗时。

**R7 — Verify Scheduling Parameters**  
优先级范围、调度策略、时间片和优先级继承能力可能因 OS 与配置而不同。不得跨平台硬编码未经核实的优先级数值。

**R8 — Verify Driver Readiness Support**  
系统提供 `poll()` 或 `select()` 不代表所有设备驱动都支持预期的就绪通知。必须检查目标驱动和目标配置，并通过测试验证。

### Recommendations

**S1**：通用需求优先考虑 POSIX 标准接口；只有标准能力不足时才采用平台扩展。

**S2**：为跨平台模块维护接口兼容性矩阵，记录标准接口、平台实现、配置依赖、差异和验证状态。

**S3**：对多阶段初始化、通信和错误恢复流程使用显式状态机；优先保证状态转换可审查，不以减少代码行数为目标。

## 8. API Analysis Template

分析单个 API 时使用以下模板。无关字段标记为“不适用”，无证据字段标记为“待验证”。

```text
API 名称：
标准版本 / 分册 / 章节：
规范链接：
头文件：
接口原型：
参数语义与数据所有权：
成功返回：
失败返回与错误码：
阻塞 / 非阻塞行为：
部分完成 / EOF 行为：
信号中断与重试规则：
线程安全 / 并发约束：
资源生命周期：
时间与超时语义：
可选能力 / 特性宏：
目标 OS 与版本：
目标 OS 官方支持状态：支持 / 部分支持 / 不支持 / 待验证
工程配置依赖：
源码或驱动核验：
平台差异：
项目封装接口：
验证用例与结果：
剩余风险：
```

## 9. Output Modes

根据用户任务选择输出结构，不必在每次回答中重复全部章节。

### Mode A — API Lookup

输出：接口定位、规范语义、返回值/错误码、关键限制、目标平台注意事项、官方链接。

### Mode B — Implementation or Debugging

输出：问题定位、标准依据、根因分析、修正代码、资源与错误处理、测试用例。

### Mode C — Architecture Design

输出：需求与约束、模块边界、接口契约、线程/事件模型、生命周期、错误恢复、平台适配、验证计划。

### Mode D — Portability Assessment

输出：目标平台矩阵、配置依赖、接口差异、不可移植点、替代策略、验证结果和风险。

### Mode E — Code Review

输出：问题等级、违反的规则、代码位置、风险解释、修复建议和是否需要平台验证。

## 10. Evidence and Confidence Labels

涉及标准或平台行为的结论应尽可能标注证据状态：

| 标记 | 含义 | 允许的表述 |
|---|---|---|
| `NORMATIVE` | 已定位 POSIX 规范性条文 | “POSIX 规定……” |
| `OS-DOC` | 已定位对应版本 OS 官方文档 | “该版本官方文档说明……” |
| `CONFIG` | 已检查当前工程配置 | “当前构建配置启用了……” |
| `SOURCE` | 已核查对应版本源码/驱动 | “当前源码实现……” |
| `TESTED` | 已在目标环境复现测试 | “在该配置/硬件上测试观察到……” |
| `INFERENCE` | 基于现有证据的工程推断 | “据此推断……”并说明前提 |
| `UNVERIFIED` | 尚无充分证据 | “待验证”，不得写成确定事实 |

不得伪造章节编号、网页内容、Kconfig 名称、返回值、测试结果或源码行为。

## 11. Proficiency Levels

等级表示可独立承担的工程任务范围，不以记忆 API 数量评定。

| 等级 | 名称 | 能力目标 | 最低验收产物 |
|---|---|---|---|
| L1 | 标准查阅级 | 能定位 XBD/XSH/XCU 条目，解释原型、参数和错误条件 | API 分析卡，含标准链接和条目定位 |
| L2 | API 应用级 | 能编写满足接口契约的调用代码，正确处理错误、I/O 边界和资源生命周期 | 可编译模块、单元测试及失败路径说明 |
| L3 | 系统设计级 | 能组合 I/O、线程、同步、队列和定时器，设计完整生命周期与恢复策略 | 模块设计、状态图/时序图、测试报告 |
| L4 | 可移植性与架构审查级 | 能评估标准与 OS 差异，建立 OSAL 和兼容性验证方案 | 平台兼容性矩阵、移植方案、风险评审报告 |

### L1 — Standard Lookup

- 能区分规范性条文和说明性资料。
- 能找到 API 的原型、头文件、参数与错误条件。
- 能提供精确的标准链接，不以搜索摘要代替标准条目。

### L2 — API Application

- 能正确区分 `errno` 与直接返回错误码的接口。
- 能处理部分读写、中断、超时、资源释放和非法参数。
- 能编写并运行基础测试验证调用行为。

### L3 — System Design

- 能根据阻塞行为、时序和资源预算选择线程或事件模型。
- 能定义共享状态、锁顺序、消息协议和退出流程。
- 能设计启动、运行、故障恢复和关闭的完整状态路径。

### L4 — Portability and Architecture Review

- 能区分 POSIX、XSI/可选能力、实现定义行为和 OS 私有扩展。
- 能核对 OS 版本、配置、源码、驱动和实测结果。
- 能建立稳定的 OSAL 边界，并评估实时性、内存、栈和故障恢复风险。

## 12. Completion Checklist

完成任务前检查：

- [ ] 是否明确目标 OS、版本和构建配置？
- [ ] 是否定位了正确的 POSIX 条目？
- [ ] 是否区分标准要求与平台实现？
- [ ] 是否核查错误返回约定和 `errno` 使用方式？
- [ ] 是否处理部分 I/O、EOF、中断、超时和非阻塞行为（适用时）？
- [ ] 是否定义线程、锁、队列、文件描述符和 Socket 的生命周期？
- [ ] 是否定义共享状态同步协议？
- [ ] 是否核验驱动对就绪事件的支持？
- [ ] 是否把 OS 扩展限制在适配层？
- [ ] 是否有正常、异常和资源耗尽测试？
- [ ] 是否把未验证项明确标注？

## 13. Reference Files — Load on Demand

不要在每次任务开始时读取所有参考文件。根据任务主题只加载必要文件或相关章节：

| 文件 | 用途 | 何时读取 |
|---|---|---|
| `references/coding_standard.md` | 项目 C 编码规范、模块边界、错误处理、并发与安全要求 | 生成或审查 C 代码时 |
| `references/nuttx_api.md` | NuttX API 分类、原生扩展、任务模型、配置和已知差异 | 目标系统为 NuttX 时 |
| `references/loop_branch_techniques.md` | 循环、分支、状态机和控制流程实践 | 设计事件循环、协议状态机或恢复流程时 |
| `references/struct_union_techniques.md` | 结构体、联合体、位域、柔性数组、`container_of` 等实践 | 设计消息载荷、设备对象或内存布局时 |

参考文件是项目知识资料，不自动等同于 POSIX 规范。遇到与目标 OS 官方文档、当前源码或项目现行规则不一致的内容，应标出差异并进一步核验；不得无提示地将参考资料中的示例当作通用标准写法。

## 14. Agent Operating Instructions

执行本 Skill 时，Agent 必须：

1. 先判断任务是否属于本 Skill 的触发范围。
2. 优先读取项目中的实际配置、接口声明和调用代码；不要重复向用户索取已提供的信息。
3. 按需读取 `references/`，不要一次性加载全部长文档。
4. 涉及当前版本、标准条款或平台能力时，查阅用户指定标准和目标 OS 官方来源。
5. 将结论分成标准要求、OS 官方能力、当前配置、源码观察、测试结果和工程建议。
6. 代码输出必须包含必要头文件、返回值检查、资源释放和关键平台限制。
7. 复杂设计先给接口契约和模块边界，再给流程、代码和验证方案。
8. 无法验证的结论使用 `UNVERIFIED` 标注；不得编造 API、Kconfig、版本行为或测试结果。
9. 尊重项目生效的编码规范；如建议偏离，必须说明原因、风险和审批需求。
10. 输出应直接服务用户当前任务，不要机械复述本 Skill 的全部内容。

## 15. Official Sources

- The Open Group Base Specifications Issue 8 / POSIX.1-2024: <https://pubs.opengroup.org/onlinepubs/9799919799/>
- Apache NuttX Documentation: <https://nuttx.apache.org/docs/latest/>
- Apache NuttX POSIX Support: <https://nuttx.apache.org/docs/latest/standards/posix.html>

**最终原则：POSIX 用于确认接口语义；目标 OS 官方文档和源码用于确认实现能力；工程配置用于确认当前构建启用情况；目标环境测试用于确认实际行为。四者不可互相替代。**
