# Embedded Framework & Project Template Knowledge Base

**版本：v1.0.0**  
**更新时间：2026-11-25**  
**定位：嵌入式软件框架、平台工程、架构模式与可复用项目模板知识库**

本仓库只解决一件事：**帮助工程师理解优秀嵌入式框架如何组织代码、不同平台工程如何构建，以及如何基于成熟模式创建可维护的固件和嵌入式应用。**

它不是 AI Skill 集合，不包含此前的 POSIX Skill、C 编码规范 Skill 或其他 Skill 包；也不试图重新实现 NuttX、Zephyr、FreeRTOS 等成熟项目。

## 内容边界

| 内容 | 本仓库是否包含 | 说明 |
|---|---:|---|
| 主流嵌入式平台与框架介绍 | 是 | 说明平台定位、工程结构、构建方式及架构思想 |
| 上游优秀开源项目索引 | 是 | 提供官方仓库、文档、源码阅读入口和适用场景 |
| 多平台工程模板 | 是 | 提供可复制的项目骨架和关键构建文件 |
| 通用架构设计参考 | 是 | 分层、组件化、设备管理、事件驱动、传输抽象等 |
| 完整第三方框架源码镜像 | 否 | 使用上游仓库和固定版本/Commit 管理，避免复制后失去更新与许可证追踪 |
| C 语言编码规范或 POSIX 专题 Skill | 否 | 不属于本知识库范围 |
| 绑定某一家芯片厂商的量产 BSP | 否 | 需根据具体 SoC、板卡、SDK 和许可单独建立 |

## 快速入口

- [知识库索引](INDEX.md)
- [上游版本状态快照](UPSTREAM_STATUS.md)
- [平台选型矩阵](00-overview/PLATFORM_SELECTION.md)
- [架构知识地图](00-overview/ARCHITECTURE_MAP.md)
- [构建系统与配置体系](02-architecture/BUILD_SYSTEMS_AND_CONFIGURATION.md)
- [上游项目目录](CATALOG.md)
- [平台指南](01-platforms/README.md)
- [架构模式](02-architecture/README.md)
- [通用组件与框架](03-components/README.md)
- [工程模板](04-templates/README.md)
- [源码阅读路线](05-reference-projects/READING_ROUTES.md)

## 仓库结构

```text
Embedded-Framework-Knowledge-Base/
├── README.md
├── INDEX.md
├── CATALOG.md
├── SOURCES_AND_LICENSES.md
├── CHANGELOG.md
├── 00-overview/          # 平台选型、知识地图、术语
├── 01-platforms/          # 不同 MCU / RTOS / Embedded Linux 平台
├── 02-architecture/      # 可复用的架构设计思想
├── 03-components/        # USB、TCP/IP、GUI、文件系统等组件
├── 04-templates/         # 可复制的多平台工程模板
├── 05-reference-projects/ # 优秀上游项目的源码阅读路线
└── assets/               # 架构图等辅助资源
```

## 使用方式

1. **先确定目标平台**：MCU 裸机、FreeRTOS、Zephyr、NuttX、RT-Thread、ESP-IDF，或嵌入式 Linux。
2. **阅读该平台指南**：先理解构建系统、启动流程、配置机制和平台边界。
3. **选择最接近的模板**：不要把一种 RTOS 的构建文件直接套到另一种平台。
4. **对照参考项目**：从上游官方示例中确认 API、目录结构和版本要求。
5. **按目标硬件验证**：板级时钟、启动文件、Linker Script、外设驱动、内存布局及中断优先级均需实测。
6. **记录上游版本**：产品项目应固定 release tag 或 commit SHA，并记录本地补丁。

## 重要说明

- 本仓库中的平台示例分为“可独立构建的 Host 示例”和“需要目标 SDK/BSP 的平台模板”。每个模板 README 都会标明构建前置条件。
- 模板代码是起点，不代表已经在所有芯片和开发板上验证。
- 官方文档的 `latest` 链接会随上游变化。产品项目应锁定 SDK release、工具链和依赖版本。
- 复用第三方代码前必须检查对应仓库、子目录和文件的许可证与 NOTICE 要求。

## 推荐研究顺序

1. STM32Cube / Pico SDK：理解裸机工程、HAL/BSP 和链接构建。
2. FreeRTOS：理解任务、队列、同步与实时调度。
3. Zephyr / NuttX / RT-Thread：理解组件化 RTOS 与设备模型。
4. ESP-IDF：理解以组件为中心的产品工程组织。
5. TinyUSB / lwIP / Nanopb / LittleFS / LVGL：理解可移植中间件的架构。
6. Buildroot / Yocto：理解嵌入式 Linux 的系统集成与产品化构建。
