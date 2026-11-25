---
name: embedded-c-engineering
version: 1.0.0
description: 面向嵌入式、RTOS、驱动、协议栈和系统软件开发的 C 语言工程技能。用于制定或审查 C 编码规范、分析控制流与状态机、设计结构体和联合体、处理内存布局与并发边界，并生成可验证的工程代码。
---

# Embedded C Engineering Skill

## 1. Purpose

本技能为嵌入式 C 工程提供统一的编码约束、控制流设计方法和数据结构设计方法。目标是产出可读、可审查、可测试、可移植且符合目标平台约束的 C 代码。

本技能包含三份按职责划分的参考资料：

| Reference | 文档定位 | 主要用途 |
|---|---|---|
| `references/c-coding-standard.md` | Normative Standard | 编码规则、错误处理、安全、并发、构建与审查 |
| `references/c-control-flow-and-state-machine.md` | Engineering Guide | 循环、分支、状态机、事件循环和资源清理 |
| `references/c-data-structures-and-memory-layout.md` | Engineering Guide | 结构体、联合体、内存布局、协议解析和接口表 |

## 2. When to Activate

以下任务应使用本技能：

- 新建、重构或审查 C 语言模块、公共 API、驱动和固件代码。
- 制定团队编码规范、代码审查清单或静态检查策略。
- 设计协议解析器、设备状态机、任务主循环或事件驱动流程。
- 设计结构体、联合体、位域、柔性数组、回调表和侵入式容器。
- 分析整数溢出、缓冲区越界、指针生命周期、对齐、别名和并发风险。
- 在 Linux、NuttX 或其他 RTOS 之间评估代码可移植性。

如果任务主要涉及 POSIX API 语义，应同时使用 POSIX System Programming 资料；如果涉及 NuttX 特有接口，应查阅目标 NuttX 版本的官方文档、配置和源码。

## 3. Authority and Evidence

分析时必须区分以下证据层级，不得混为一谈：

1. **语言标准保证**：ISO C 对语法、类型、对象表示、求值和标准库行为的规定。
2. **平台或 ABI 约定**：编译器、CPU、调用约定、结构体布局和目标 ABI 的实现行为。
3. **操作系统约定**：POSIX、NuttX 或其他 RTOS 的接口语义。
4. **项目规范**：项目有意采用的命名、格式、模块边界和质量门禁。
5. **工程建议**：在特定场景下有价值，但不是语言或平台的强制要求。

遇到不确定行为时，明确标记为“需目标编译器/平台验证”。不得将常见 GCC 行为描述为 ISO C 保证，也不得仅凭函数名推断 RTOS 实际支持能力。

## 4. Workflow

### Step 1 — Identify Context

先识别：

- C 语言版本：C99、C11、C17、C23 或编译器扩展模式。
- 编译器及版本、目标 CPU、字长、端序和 ABI。
- 操作系统/RTOS 及版本、构建配置和可用标准库。
- 代码上下文：普通任务、ISR、信号处理、启动阶段或引导程序。
- 资源约束：RAM、ROM、栈、堆、实时性和功耗。

缺少信息时，列出假设并说明哪些结论需要补充环境后验证。

### Step 2 — Load Relevant References

按需加载，不要无差别读取全部资料：

- 编码风格、内存安全、错误处理、并发、构建和审查：加载 `c-coding-standard.md`。
- 循环、条件分支、状态机、事件循环和退出流程：加载 `c-control-flow-and-state-machine.md`。
- 数据表示、位域、联合体、协议帧、FAM、`container_of`、ABI：加载 `c-data-structures-and-memory-layout.md`。

### Step 3 — Classify the Requirement

将要求分类为：

- **MUST**：标准、平台或项目规范明确要求。
- **SHOULD**：强烈推荐，偏离时需要说明原因。
- **MAY**：可选方案，依据性能、资源或维护成本选择。

不能把个人偏好或经验法则提升为 C 标准要求。

### Step 4 — Design Before Coding

对非平凡任务，先说明：

- 模块职责和接口边界。
- 状态、数据所有权和资源生命周期。
- 并发上下文及同步策略。
- 错误传播、超时和恢复路径。
- 平台依赖与可移植性边界。

### Step 5 — Implement and Validate

代码必须尽可能提供完整上下文，包括必要头文件、类型定义、错误路径和资源释放。验证至少考虑：

- 编译器警告和目标标准模式。
- 边界值、空输入、最大输入和溢出。
- 失败路径、资源回收和重复调用。
- 并发访问、ISR/任务交互和生命周期。
- 目标平台编译及必要的板级测试。

不能运行编译或测试时，明确说明“代码未在目标环境验证”。

## 5. Output Contract

回答工程问题时，按任务复杂度提供以下内容：

1. **结论**：推荐方案及其适用条件。
2. **依据**：语言标准、平台文档或项目规则；区分规范要求与建议。
3. **设计**：模块、控制流、数据结构或资源所有权。
4. **实现**：可编译的代码片段或文件级实现。
5. **边界与风险**：错误路径、并发、内存、ABI 和移植问题。
6. **验证**：编译选项、测试用例、静态分析或目标板验证方法。

## 6. Competency Levels

| Level | Name | Observable capability |
|---|---|---|
| L1 | Language Practitioner | 正确使用基础语法、类型、函数、指针和标准库；识别常见越界与生命周期问题 |
| L2 | Embedded C Developer | 编写模块化固件代码，处理资源、错误、缓冲区和目标平台限制 |
| L3 | Systems Engineer | 设计状态机、并发模型、数据布局、驱动接口和可测试模块 |
| L4 | Senior/Reviewer | 审查标准与 ABI 边界，评估实时性、安全性、可移植性，并制定团队规则 |

等级评估以可复核的工程产物为依据，不以 API 背诵量或代码复杂度作为标准。

## 7. Quality Gates

交付前检查：

- [ ] 已明确 C 语言版本和目标工具链。
- [ ] 已区分语言标准、平台实现和项目约定。
- [ ] 没有把 `volatile` 当作线程同步原语。
- [ ] 没有假设结构体/位域布局可直接作为跨平台协议格式。
- [ ] 动态内存大小计算检查了溢出。
- [ ] 指针、缓冲区、字符串和资源生命周期有明确边界。
- [ ] 控制流具有明确退出条件和错误路径。
- [ ] 并发共享状态具有明确同步或所有权约定。
- [ ] 示例没有未经说明的编译器扩展。
- [ ] 未验证的行为已明确标注。

## 8. Related Skills

- POSIX System Programming：POSIX 文件 I/O、线程、Socket、时钟、消息队列等接口语义。
- NuttX RTOS Development：NuttX Task/Pthread、驱动、Kconfig 和板级实现。
- Communication Protocol Engineering：帧格式、编解码、传输层和会话管理。
- Embedded Software Architecture：HAL、OSAL、模块边界、依赖方向和固件框架。
