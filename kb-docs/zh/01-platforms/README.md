# 平台指南

本目录按平台类型介绍真实项目的工程组织方式。每篇指南遵循：定位、仓库结构、启动/构建模型、配置模型、架构思想、适用场景、常见坑、推荐源码阅读入口。

| 平台 | 类型 | 官方项目/文档 | 配套模板 |
|---|---|---|---|
| Cortex-M Bare Metal | MCU 裸机 | CMSIS、芯片厂商 SDK | `baremetal-cortex-m` |
| STM32Cube | MCU SDK | STM32Cube packages | `stm32cube-application` |
| FreeRTOS | RTOS | Kernel / FreeRTOS distribution | `freertos-application` |
| Zephyr | RTOS 平台 | Zephyr Project | `zephyr-application` |
| NuttX | POSIX 风格 RTOS | NuttX + NuttX Apps | `nuttx-application` |
| RT-Thread | RTOS 与组件生态 | RT-Thread | `rt-thread-application` |
| ESP-IDF | SoC SDK/框架 | ESP-IDF | `esp-idf-project` |
| Pico SDK | MCU SDK | Raspberry Pi Pico SDK | `pico-sdk-project` |
| Buildroot | 嵌入式 Linux 构建系统 | Buildroot | `buildroot-external` |
| Yocto | 嵌入式 Linux 构建框架 | Yocto Project | `yocto-layer` |

平台“支持某硬件”的含义可能是官方支持、社区支持或仅存在移植；在产品决策前必须检查具体板卡、驱动和版本状态。
