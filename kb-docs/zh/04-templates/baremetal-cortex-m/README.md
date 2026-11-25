# Cortex-M Bare-metal Application Template

**性质：平台工程骨架，不包含特定 MCU 的 startup、CMSIS Device、Linker Script 或 HAL。**

## 目录

- `app/`：平台无关应用逻辑。
- `platform/`：必须由板级工程实现的初始化和调度接口。
- `CMakeLists.txt` 与 `cmake/toolchain-arm-none-eabi.cmake`：交叉构建入口；仍需提供目标 startup 汇编、Linker Script 和实际 `board_port.c`。

## 集成步骤

1. 选择芯片厂商 SDK 或 CMSIS Device 包。
2. 将 `src/board_port_stub.c.example` 复制/改名为 `src/board_port.c` 并实现接口。
3. 准备芯片对应的 startup 汇编与 Linker Script。
4. 配置 CPU 型号、工具链和链接脚本并构建。
5. 实现 `board_init()`、`board_tick_ms()`、`board_idle()`。
6. 配置 CPU、FPU、ABI、优化等级和 section garbage collection。
7. 将主循环时间片和每个模块最坏执行时间纳入设计。

这里的 `board_idle()` 可按硬件实现 WFI，但必须确保中断/唤醒源和临界区策略正确。

## CMake 构建示例

```bash
cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake \
  -DECL_CPU=cortex-m4 \
  -DECL_STARTUP_SOURCE=/path/to/startup_stm32xxxx.s \
  -DECL_LINKER_SCRIPT=/path/to/STM32xxxx_FLASH.ld
cmake --build build
arm-none-eabi-size build/firmware
```

注意：示例中的 `ECL_CPU`、startup 和 linker script 必须替换成目标芯片实际值。若芯片使用 FPU，按芯片/ABI 增加 `ECL_FPU` 和 `ECL_FLOAT_ABI`。
