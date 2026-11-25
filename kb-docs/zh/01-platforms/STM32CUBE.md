# STM32Cube 工程体系

## 定位

STM32Cube 是 ST 面向 STM32 MCU/MPU 的软件生态。MCU Package 通常组合 CMSIS、HAL/LL、BSP、Middleware、Utilities 和板级 Projects/Examples。不同系列的 Package 内容、版本和中间件组合并不完全相同。

## 典型目录

```text
STM32Cube_FW_<series>/
├── Drivers/
│   ├── CMSIS/
│   ├── STM32<series>xx_HAL_Driver/
│   └── BSP/
├── Middlewares/
├── Projects/
│   ├── <Board>/Applications/
│   ├── <Board>/Examples/
│   └── <Board>/Templates/
└── Utilities/
```

## 架构思想

- CMSIS Core/Device 提供 Cortex-M 核心及芯片设备定义相关接口。
- HAL 提供较高层的外设操作抽象；LL 提供更贴近寄存器的轻量接口。
- BSP 面向具体开发板与板载外设。
- Middleware 提供 USB、文件系统、RTOS、网络和图形等中间件（取决于系列和 Package）。
- Projects 按板卡和功能组织示例、应用与演示。

## 应用层推荐组织

不要把全部产品逻辑堆进 CubeMX 生成的 `main.c`。建议保持生成入口与产品逻辑分离：

```text
Core/
├── Inc/       # CubeMX 生成的芯片/外设头文件
└── Src/       # CubeMX 生成的初始化和中断文件
App/
├── app_init.c
├── app_process.c
└── app_service.c
Bsp/
Drivers/
Middlewares/
```

在 `main()` 中完成 HAL、时钟和外设初始化后调用 `app_init()`，再由主循环或 RTOS 任务调用 `app_process()`。

## 使用建议

- 先确认目标 STM32 系列对应的 Cube Package，不要混用不同系列 HAL。
- HAL 与 LL 可按模块选择，但要明确寄存器/状态共享的边界。
- CubeMX 生成代码应视为可再生成产物；用户代码放在规定的 USER CODE 区域或独立 App 文件。
- 对升级、时钟树、DMA、Cache、TrustZone、双核互通等特性，必须按具体系列参考参考手册和官方示例。
- 新系列可能引入 HAL2；不要假设 HAL1 与 HAL2 API 完全一致。

## 推荐模板

[`stm32cube-application`](../04-templates/stm32cube-application/README.md) 是应用层骨架，不包含特定系列的启动代码、HAL 源码或 Linker Script。

## 官方入口

- STM32Cube MCU/MPU Packages: https://www.st.com/en/embedded-software/stm32cube-mcu-mpu-packages.html
- STM32Cube GitHub Offer: https://github.com/STMicroelectronics/STM32Cube_MCU_Overall_Offer
- 示例工程说明: https://wiki.st.com/stm32mcu/wiki/Getting_started_with_STM32Cube_MCU_Package
