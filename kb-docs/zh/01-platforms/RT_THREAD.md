# RT-Thread 工程体系

## 定位

RT-Thread 是面向 MCU/IoT 的 RTOS 及组件生态。典型仓库由内核、CPU 架构移植、BSP、组件、示例、工具和软件包机制组成。项目可采用 SCons/Env 或具体 BSP 所提供的 IDE 工程。

## 典型结构

```text
rt-thread/
├── src/                  # Kernel 核心
├── include/              # Kernel headers
├── libcpu/               # CPU 架构移植
├── bsp/                  # 板级支持
├── components/           # FinSH、文件系统、网络等组件
├── examples/             # 示例
├── tools/                # 构建工具
└── packages/             # 软件包通常由包管理/工作区获取
```

## 架构思想

- Kernel 提供线程调度、IPC、定时器和内存管理。
- Device Framework 统一表达常见设备访问方式。
- Components 扩展文件系统、Shell、网络等系统服务。
- Packages 将可选功能与内核解耦，按项目需求组合。
- BSP 统一放置板卡相关时钟、Pin、外设、启动和构建配置。

## 工程实践

- 先从目标 BSP 的 `README.md`、`SConstruct`/`SConscript` 和默认工程配置入手。
- 区分 RT-Thread 内核组件、官方软件包和社区软件包。
- 对 package 版本进行固定，避免构建时自动获取不受控版本。
- 应用应调用稳定的组件/设备接口，不要绕过设备框架直接依赖 BSP 私有结构。
- FinSH 命令可作为设备诊断与工厂测试入口，但需要控制权限和生产版本暴露面。

## 推荐研究

- 官方仓库: https://github.com/RT-Thread/rt-thread
- 文档中心: https://www.rt-thread.io/document/site/
- 源码目录与 BSP 说明: https://github.com/RT-Thread/rt-thread
