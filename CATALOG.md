# Embedded Component Catalog

本目录用于选择上游项目和决定复用方式。表中的许可证信息是目录级初步记录，不是法律意见；导入时必须重新检查选定版本的许可证文件、SPDX 头、子模块、BSP 和示例目录。

## 1. RTOS / 系统基础

| 项目 | 上游仓库 | 主要用途 | 目录建议 | 初步许可证记录 |
|---|---|---|---|---|
| Apache NuttX | https://github.com/apache/nuttx | POSIX 风格 RTOS、设备驱动、文件系统、网络 | 作为平台，不复制内核到通用组件层 | Apache-2.0 或兼容许可证；逐文件核查 |
| NuttX Apps | https://github.com/apache/nuttx-apps | NuttX 应用、工具和示例 | 作为应用参考，按应用目录评估 | 仓库/子目录及第三方内容分别核查 |
| Zephyr | https://github.com/zephyrproject-rtos/zephyr | RTOS、设备模型、Devicetree、Work Queue | 作为架构参考或目标平台 | Apache-2.0（仓库元数据） |
| FreeRTOS Kernel | https://github.com/FreeRTOS/FreeRTOS-Kernel | 任务调度、队列、同步、定时器 | 作为 RTOS 内核依赖 | MIT；Demo 可能包含第三方软件 |
| FreeRTOS Distribution | https://github.com/FreeRTOS/FreeRTOS | 内核、移植和 Demo 集合 | 仅提取经审核的目标子目录 | 内部第三方文件可能采用不同许可证 |
| RT-Thread | https://github.com/RT-Thread/rt-thread | RTOS、组件和软件包体系 | 组件架构参考/目标平台 | Apache-2.0；芯片厂商 BSP 另行核查 |
| CMSIS | https://github.com/ARM-software/CMSIS_5 | Cortex 处理器接口、DSP、RTOS API 等 | 仅面向 Arm 平台的适配资产 | Apache-2.0（Arm 文档说明） |

## 2. 工程模板 / 构建参考

| 项目 | 上游仓库 | 主要用途 | 使用方式 | 初步许可证记录 |
|---|---|---|---|---|
| libopencm3-template | https://github.com/libopencm3/libopencm3-template | Cortex-M Makefile 工程模板 | 复制模板结构；检查模板及子模块许可证 | 导入时逐项核查 |
| libopencm3-examples | https://github.com/libopencm3/libopencm3-examples | 外设和板级示例 | 按具体示例和芯片选择 | 逐目录核查 |
| PlatformIO Examples | https://github.com/platformio/platformio-examples | 多平台工程、框架和构建示例 | 作为构建和平台样例目录 | 仓库标记 Apache-2.0；样例依赖仍需核查 |

## 3. 通信 / 协议

| 项目 | 上游仓库 | 主要用途 | 目录建议 | 初步许可证记录 |
|---|---|---|---|---|
| TinyUSB | https://github.com/hathach/tinyusb | USB Device/Host、CDC、HID、MSC 等 | `third_party/tinyusb/`（审核后） | MIT（上游 LICENSE） |
| lwIP | https://github.com/lwip-tcpip/lwip | TCP/IP、UDP、DHCP、DNS 等 | `third_party/lwip/`（审核后） | BSD 系列；确认所选版本和文件 |
| Nanopb | https://github.com/nanopb/nanopb | 面向嵌入式的 Protocol Buffers C 实现 | `third_party/nanopb/`（审核后） | 导入时核验上游 LICENSE 与生成文件 |

## 4. 存储 / 启动 / 更新

| 项目 | 上游仓库 | 主要用途 | 目录建议 | 初步许可证记录 |
|---|---|---|---|---|
| littlefs | https://github.com/littlefs-project/littlefs | Flash 文件系统、掉电恢复 | `third_party/littlefs/`（审核后） | BSD-3-Clause |
| MCUboot | https://github.com/mcu-tools/mcuboot | Secure Boot、固件升级、镜像验证 | `third_party/mcuboot/`（审核后） | Apache-2.0 |

## 5. UI / C++ 容器 / 测试

| 项目 | 上游仓库 | 主要用途 | 目录建议 | 初步许可证记录 |
|---|---|---|---|---|
| LVGL | https://github.com/lvgl/lvgl | 嵌入式 UI、控件、事件和渲染 | `third_party/lvgl/`（审核后） | LVGL Core MIT；LVGL Pro 另有许可 |
| ETL | https://github.com/ETLCPP/etl | 固定容量 C++ 容器和算法 | `third_party/etl/`（审核后） | MIT |
| Unity | https://github.com/ThrowTheSwitch/Unity | C 单元测试断言框架 | `third_party/Unity/`（审核后） | MIT |
| Ceedling | https://github.com/ThrowTheSwitch/Ceedling | C 项目测试构建和 Mock 工作流 | 开发机测试工具 | MIT |

## 6. 按需求选型

| 需求 | 首选研究对象 | 评估重点 |
|---|---|---|
| NuttX 应用模板 | NuttX Apps | 当前 NuttX 版本、Make.defs、Kconfig、应用注册 |
| RTOS 设备模型 | Zephyr / RT-Thread | 对象生命周期、驱动注册、初始化顺序 |
| USB CDC | TinyUSB / NuttX USB | 控制器驱动适配、缓冲区、任务/中断上下文 |
| TCP/IP | lwIP / NuttX Network | Socket 支持、内存池、线程模型、驱动边界 |
| PC 与设备结构化协议 | Nanopb | schema 版本、最大消息、兼容演进、解码限制 |
| Flash 配置存储 | littlefs | 块设备适配、掉电行为、擦写约束 |
| OTA / Secure Boot | MCUboot | Flash 布局、签名策略、回滚和密钥管理 |
| 嵌入式 UI | LVGL | 内存配置、显示/输入驱动、任务线程约束 |
| 无动态堆 C++ 容器 | ETL | 固定容量、C++ 标准、内存预算 |
| C 单元测试 | Unity + Ceedling | Host 编译、Mock、板级集成测试边界 |

## 7. 目录数据

机器可读清单位于 `catalog/components.json`。该文件中的 `status` 默认是 `catalog-only`，只有完成接入工作流后才可提升状态。
