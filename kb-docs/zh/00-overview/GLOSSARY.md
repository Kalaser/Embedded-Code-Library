# 常用术语

| 术语 | 定义 | 与其他概念的边界 |
|---|---|---|
| SoC | 集成 CPU、存储控制器及外设的芯片 | 芯片级，不等同开发板 |
| Board | SoC 与电源、时钟、存储、外设等组成的硬件平台 | 板级信息由 BSP 描述 |
| BSP | Board Support Package，板级支持包 | 负责板卡特有初始化和资源映射 |
| HAL | Hardware Abstraction Layer，硬件抽象层 | 提供较稳定的外设访问 API，不等于驱动框架 |
| Driver | 某类硬件或设备的控制实现 | 处理具体外设协议和状态 |
| Middleware | 可复用中间件，如 USB、TCP/IP、FS、GUI | 通常位于 OS/Driver 与应用之间 |
| OSAL | OS Abstraction Layer，操作系统适配层 | 仅抽象确有移植需求的 OS 原语 |
| RTOS | 实时操作系统 | 有调度与实时行为约束，不保证自动满足硬实时 |
| Component | 可构建、配置和依赖管理的模块 | 组件边界应由 API 和构建系统共同表达 |
| Application | 面向产品或用户场景的程序 | 组合组件实现用例 |
| Devicetree | 描述硬件拓扑和设备资源的数据结构/语言 | 不替代驱动代码和 Kconfig |
| Kconfig | 配置选项与依赖表达机制 | 控制功能编译与配置，不等同运行时配置 |
| Linker Script | 指定代码/数据放入目标内存区域的链接规则 | 与启动代码和芯片内存布局密切相关 |
| OTA | Over-the-Air 更新 | 更新传输方式；可靠升级还需要镜像校验和回滚机制 |
