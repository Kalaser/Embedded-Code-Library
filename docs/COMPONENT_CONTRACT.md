# Component Contract

每个进入 `components/` 的组件必须定义以下契约。

## 1. Identity

- 稳定组件名和版本。
- 一句话职责描述。
- 维护者/联系人。
- 状态：draft / experimental / validated / production / deprecated。

## 2. API contract

- 公开头文件、类型和函数。
- 参数有效范围和空指针策略。
- 返回值、错误语义和部分完成语义。
- 调用上下文限制：任务线程、ISR、回调或启动阶段。
- 阻塞/非阻塞和超时约定。
- 线程安全：thread-safe / reentrant / externally synchronized / not thread-safe。
- 资源所有权：调用者拥有、组件拥有或借用。
- 初始化、重置、关闭和销毁顺序。

## 3. Memory and timing

- 动态分配策略：none / caller-provided / internal heap。
- 静态 RAM 和栈需求（已知时）。
- 缓冲区最大尺寸和容量计算。
- 时间复杂度和实时性限制（有依据时）。
- 是否使用递归、浮点、锁或系统调用。

## 4. Build and tests

- 最低语言标准和编译器要求。
- CMake/Make/目标 RTOS 构建方式。
- Host 单元测试及测试命令。
- 边界条件、错误路径和资源耗尽测试。
- 已验证工具链、OS、CPU 和配置。

## 5. Change policy

公开 API 的不兼容变化必须提高主版本或按项目版本策略处理，并提供迁移说明。修复缺陷和新增向后兼容功能应分别记录。
