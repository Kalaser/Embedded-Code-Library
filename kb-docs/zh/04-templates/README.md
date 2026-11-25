# 工程模板库

模板分为两种：

- **Host 可独立构建模板**：不依赖 MCU SDK，可在桌面 Linux/macOS/Windows 的 CMake 环境验证结构和公共组件。
- **目标平台模板**：依赖对应 SDK、RTOS、BSP 或 Yocto/Buildroot workspace；README 会列出前置条件和构建命令。

| 模板 | 平台 | 构建系统 | 当前验证属性 |
|---|---|---|---|
| portable-c-component | 跨平台 C | CMake | Host 可构建/测试 |
| baremetal-cortex-m | Cortex-M | CMake + arm-none-eabi | 框架骨架；需补 startup/linker/BSP |
| stm32cube-application | STM32Cube | CubeIDE/CMake 工程集成 | 应用层源码骨架；需目标 Cube Package |
| freertos-application | FreeRTOS | Vendor CMake/IDE | 需 Kernel Port 与 FreeRTOSConfig |
| zephyr-application | Zephyr | west + CMake | 需 Zephyr SDK/workspace/Board |
| nuttx-application | NuttX | NuttX Apps Make rules | 需匹配的 nuttx + apps workspace |
| rt-thread-application | RT-Thread | SCons/BSP build | 需目标 BSP/Env 或 SCons workspace |
| esp-idf-project | ESP-IDF | idf.py + CMake | 需 ESP-IDF 环境和目标芯片 |
| pico-sdk-project | Pico SDK | CMake | 需 Pico SDK 和目标板配置 |
| buildroot-external | Buildroot | Buildroot Make | 需 Buildroot 源码及 toolchain |
| yocto-layer | Yocto | BitBake | 需兼容的 Poky/OE workspace |
| embedded-linux-cmake | Linux user space | CMake | Host 可构建；目标需交叉工具链/sysroot |

## 复制模板前

1. 复制整个模板目录到产品仓库。
2. 替换项目名、供应商/芯片、板卡和配置项。
3. 补齐 startup、linker、BSP、SDK 路径或平台 manifest。
4. 固定工具链、SDK、依赖 release/commit。
5. 在目标板上验证启动、外设、并发和资源占用。

模板不应被当成完整 BSP 或量产工程。未注明的硬件特定内容必须由产品工程补齐。
