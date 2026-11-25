# Apache NuttX 工程体系

## 定位

Apache NuttX 是面向 MCU 和嵌入式系统的 RTOS，强调小型化、POSIX 风格接口、设备文件模型和可配置功能。NuttX Kernel 与 NuttX Apps 是分离仓库，应用通常放在 `apps` 仓库并由 NuttX 配置系统选择。

## 典型仓库关系

```text
nuttx/                 # RTOS 核心、arch、boards、drivers、sched、fs
nuttx-apps/            # 应用、库、命令和示例
```

典型 NuttX 应用目录：

```text
apps/examples/myapp/
├── Makefile
├── Make.defs
├── Kconfig              # 可选：应用配置项
├── myapp_main.c
└── README.md            # 建议保留
```

## 应用集成机制

- `Make.defs` 声明应用是否加入构建。
- `Makefile` 通过 NuttX Apps 的构建规则描述程序名、优先级、栈大小和主源文件。
- `Kconfig` 可定义应用是否启用及相关参数。
- 应用配置从 NuttX 配置系统中选择，最终由 `apps` 构建系统编入应用库或以相应方式安装。

## 架构特点

- 设备可通过文件描述符形式访问，驱动与应用之间可以共享文件 I/O 语义。
- POSIX 风格 API 降低部分系统编程迁移成本，但不能由此推断所有 POSIX 接口在每个 NuttX 配置中都可用。
- NuttX 原生 Task API、驱动私有 API 与 POSIX 标准 API 应明确区分。
- Kernel 和 Apps 分仓库便于独立演进，但实际构建需要版本兼容。

## 配置与验证

- 核对 `nuttx` 与 `apps` 的兼容版本。
- 检查目标 board/configuration 和 `defconfig`。
- 检查应用依赖的 `CONFIG_*` 选项。
- 用 `make menuconfig` 或对应配置工具确认应用已启用。
- 实际交叉编译并在目标板上验证设备节点和驱动能力。

## 推荐研究

- Kernel: https://github.com/apache/nuttx
- Apps: https://github.com/apache/nuttx-apps
- Applications docs: https://nuttx.apache.org/docs/latest/applications/index.html
- Apps README: https://github.com/apache/nuttx-apps/blob/master/README.md
