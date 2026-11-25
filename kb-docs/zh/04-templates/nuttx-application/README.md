# NuttX Application Template

本模板按 NuttX Apps 当前 `examples/hello` 的应用组织方式编写。需要放入与目标 `nuttx` 版本匹配的 `nuttx-apps` 工作区，例如 `apps/examples/ecl_hello/`。

## 文件说明

- `Make.defs`：应用启用时加入 `CONFIGURED_APPS`。
- `Makefile`：通过 NuttX `Application.mk` 声明应用构建信息。
- `Kconfig`：定义应用配置和 task priority/stack size。
- `ecl_hello_main.c`：NuttX NSH/应用任务入口。

## 集成

1. 将目录复制至 `nuttx-apps/examples/ecl_hello/`。
2. 确认该版本 Apps 的顶层 Kconfig/Makefile 能发现此目录（当前仓库模式通常扫描含 Makefile/Make.defs 的目录）。
3. 进入目标 NuttX 配置并启用 `CONFIG_EXAMPLES_ECL_HELLO`。
4. 重新配置、编译 NuttX 和 Apps。

NuttX 内部构建规则可能随版本调整；升级时以对应版本的 `apps/examples/hello` 为准。
