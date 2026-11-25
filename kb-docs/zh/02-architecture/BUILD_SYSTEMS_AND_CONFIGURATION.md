# 构建系统与配置体系

嵌入式工程通常由多个不同机制协作：构建系统负责编译依赖，配置系统选择功能，硬件描述提供设备资源，workspace/manifest 固定多仓库版本。不要把这些角色混为一谈。

## 主要机制对照

| 平台 | 构建入口 | 配置机制 | 硬件/平台描述 | 多仓库/依赖管理 |
|---|---|---|---|---|
| 裸机/Cortex-M | Make/CMake/IDE | 头文件、Kconfig 或厂商配置 | BSP/CMSIS Device/Linker Script | Git submodule/manifest/锁定 SDK |
| STM32Cube | CubeIDE/Make/CMake | CubeMX `.ioc`、预处理宏 | CMSIS Device、BSP、Pin/Clock init | 固定 Cube Package 和工具版本 |
| FreeRTOS | Make/CMake/IDE | `FreeRTOSConfig.h` | BSP、Port、startup、linker | Kernel tag + Port + Vendor SDK |
| Zephyr | CMake + west | Kconfig | Devicetree + Board | west manifest |
| NuttX | Make + configure | Kconfig/defconfig | board/arch/driver | `nuttx` 与 `apps` 版本配对 |
| RT-Thread | SCons/IDE | Kconfig、RT-Thread 配置 | BSP/SConscript | Env/package 管理 |
| ESP-IDF | CMake + idf.py | Kconfig / sdkconfig | target、board、partition table | Component Manager + dependencies.lock |
| Pico SDK | CMake | CMake cache / board config | Pico board headers/config | 固定 SDK path/tag |
| Buildroot | Make | Kconfig/defconfig | board config、kernel config、rootfs overlay | BR2_EXTERNAL + package source revision |
| Yocto | BitBake | distro/machine/local.conf | machine/ BSP layer | layers、SRCREV、layer revision |

## 关键区别

### 构建系统

描述编译哪些源文件、头文件路径、编译选项、依赖库、链接选项和构建产物。例：CMake、Make、SCons、BitBake。

### 配置系统

决定哪些功能被启用、宏/参数取值和功能依赖。例：Kconfig、`FreeRTOSConfig.h`、`sdkconfig`。配置通常会影响代码是否参与编译及 RAM/Flash 消耗。

### 硬件描述

表达板卡/SoC 拥有哪些设备、资源和连接关系。例：Zephyr Devicetree、CMSIS Device header、BSP board config、Yocto machine config。它不替代驱动实现。

### Workspace 与版本清单

把多个上游仓库、模块和工具版本组合成一致的工作区。例：west manifest、Git submodule、ESP-IDF component lock、Yocto layer revisions。产品构建应避免浮动分支。

## 工程复现要求

每个产品仓库至少记录：

- 主平台/SDK release 和 commit/tag。
- 工具链及版本、CMake/Make/SCons/BitBake 版本。
- 板卡、SoC、CPU/FPU/ABI 和 linker script。
- RTOS/kernel 配置与 defconfig/sdkconfig/FreeRTOSConfig。
- 第三方组件版本和依赖锁定文件。
- 构建命令、产物路径和可复现的 clean build 流程。

## 推荐实践

- 尽量使用平台原生构建机制，不在其外面套一层重复的配置系统。
- 为产品变体建立明确配置 profile，而不是复制多份几乎相同工程。
- 构建选项与运行时配置分离：编译期决定能力，运行时决定行为参数。
- 生成文件与人工维护文件分开，明确哪些文件由 CubeMX、Kconfig、idf.py 或 BitBake 生成。
- CI 构建必须使用锁定依赖和干净 workspace。
