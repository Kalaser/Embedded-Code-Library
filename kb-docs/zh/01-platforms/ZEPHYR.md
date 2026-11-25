# Zephyr 工程体系

## 定位

Zephyr 是一个可配置、模块化的 RTOS 与嵌入式开发平台。应用工程通常通过 CMake 构建，使用 Kconfig 配置软件能力，使用 Devicetree 描述板级硬件资源，并通过 west 管理 workspace、构建和烧录流程。

## 典型应用结构

```text
my_zephyr_app/
├── CMakeLists.txt
├── prj.conf
├── Kconfig                 # 可选：应用自己的配置项
├── boards/                 # 可选：应用 overlay / board-specific config
├── dts/                    # 可选：自定义 binding
├── src/
│   └── main.c
└── modules/                # 通常由 west manifest 管理，不随意复制
```

## 核心机制

- CMake：应用和模块构建。
- Kconfig：选择内核及子系统功能。
- Devicetree：描述硬件设备、总线和资源。
- west：管理多个仓库组成的 workspace，并提供 build/flash/debug 等命令。
- Device Model：通过设备实例和驱动 API 访问硬件。
- Kernel primitives：线程、队列、信号量、Work Queue、Timer 等。

## 架构特点

Zephyr 的重要思想不是单独的内核，而是把内核、驱动、子系统、配置、板卡支持和模块集成到统一工程模型。使用 Zephyr 时应顺应其 DeviceTree/Kconfig/CMake 机制，不要再私自构建一套平行设备注册和配置系统。

## 常见工程风险

- 混淆 Kconfig 与 Devicetree：一个控制功能配置，一个描述硬件拓扑/资源。
- Overlay 写入了错误的节点 label 或 binding。
- `prj.conf` 缺少功能开关，代码存在但目标未编译。
- workspace/module revision 不固定导致不同机器构建结果不同。
- 在 ISR 做过多工作，而不是投递到线程/Work Queue。

## 构建入口

```bash
west init -l .
west update
west build -b <board> .
west flash
```

上述命令假设当前目录已处于正确 west workspace，并已安装相应工具链、SDK 和板卡支持。

## 推荐研究

- 官方仓库: https://github.com/zephyrproject-rtos/zephyr
- 应用开发: https://docs.zephyrproject.org/latest/develop/application/index.html
- Samples: https://github.com/zephyrproject-rtos/zephyr/tree/main/samples
