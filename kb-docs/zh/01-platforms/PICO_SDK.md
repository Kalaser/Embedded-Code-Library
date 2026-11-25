# Raspberry Pi Pico SDK 工程体系

## 定位

Pico SDK 为 RP2040/RP2350 等 Raspberry Pi 微控制器提供 C/C++ 开发库和 CMake 工程体系。它不是完整 RTOS；项目可基于裸机主循环，也可叠加其他运行时或 RTOS。

## 典型结构

```text
pico_project/
├── CMakeLists.txt
├── pico_sdk_import.cmake   # 从 SDK 复制/引用的导入脚本
├── src/
│   └── main.c
└── boards/                 # 可选：自定义 board 配置
```

## 关键特点

- CMake 驱动 SDK 导入和目标配置。
- `pico_stdlib` 聚合常用基础能力。
- 输出工具可生成 ELF、BIN、HEX 或 UF2 等格式（取决于目标和配置）。
- PIO 是其重要特色，可通过状态机实现专用 I/O 时序逻辑。
- 双核能力可用于并行工作，但需显式管理共享数据和核间同步。

## 构建

```bash
cmake -S . -B build -DPICO_SDK_PATH=/path/to/pico-sdk
cmake --build build
```

SDK 导入脚本必须在 `project()` 前按官方要求加载。模板不包含 SDK 源码，应通过固定版本的 SDK 工作区提供依赖。

## 推荐研究

- SDK: https://github.com/raspberrypi/pico-sdk
- Examples: https://github.com/raspberrypi/pico-examples
- SDK README: https://github.com/raspberrypi/pico-sdk#readme
