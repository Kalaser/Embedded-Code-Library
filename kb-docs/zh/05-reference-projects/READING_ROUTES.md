# 优秀项目源码阅读路线

不要从仓库根目录开始随机浏览。按“入口 → 构建 → 配置 → 初始化 → 核心模块 → 平台适配 → 测试”阅读。

## NuttX

1. `nuttx/boards/`：板卡结构和配置。
2. `nuttx/arch/`：CPU 架构端口。
3. `nuttx/drivers/`：设备驱动组织。
4. `nuttx-apps/examples/`：可运行应用和构建入口。
5. `nuttx-apps/README.md`：应用加入构建的规则。

重点：Kernel 与 Apps 分离、应用 Kconfig/Make.defs、设备节点和配置裁剪。

## Zephyr

1. `samples/`：按功能定位可运行样例。
2. `boards/`：板卡支持和 Devicetree。
3. `drivers/`：驱动模型。
4. `subsys/`：系统子系统。
5. `west.yml` / manifest：多仓库 workspace 组成。

重点：DeviceTree、Kconfig、CMake 和驱动设备实例如何协作。

## FreeRTOS

1. `tasks.c` / `queue.c`：调度和队列核心。
2. `portable/`：架构/编译器 Port。
3. `include/FreeRTOS.h`：核心 API。
4. `FreeRTOSConfig.h`：项目配置。
5. 官方 Demo：观察启动、任务、同步和 BSP 集成。

重点：内核通用部分与 Port 的边界，任务栈和中断规则。

## ESP-IDF

1. 根 CMake / `project.cmake` 入口。
2. `components/` 中组件注册。
3. `main/` 作为应用组件。
4. Kconfig 与 `sdkconfig`。
5. `examples/` 中同类应用。

重点：组件依赖声明和构建系统如何组织产品代码。

## TinyUSB

1. `examples/device/cdc_msc`。
2. `hw/bsp/<family>/boards/<board>`。
3. `src/device/usbd.c` 与 class driver。
4. 对应 DCD 实现。
5. OSAL 与测试。

重点：USB Core/Class/Controller 分离、ISR 到主循环/任务的事件递交。

## LittleFS

1. `README.md` 的设计概述。
2. `DESIGN.md` 的 Metadata Pair、COW 和 block allocator。
3. `SPEC.md` 的磁盘格式。
4. `lfs.c` / `lfs.h` 与示例 block device。

重点：在 RAM 受限、Flash 擦写受限且可能掉电的条件下如何组织元数据。

## MCUboot

1. `boot/bootutil/`：通用镜像验证和状态逻辑。
2. `boot/<platform>/`：平台 boot application。
3. `docs/design.md`：分区、镜像和启动流程。
4. `scripts/imgtool.py`：镜像签名/打包工具。

重点：可复用逻辑与最终平台跳转代码分离。

## STM32Cube

1. `Projects/<Board>/Templates`。
2. `Projects/<Board>/Examples`。
3. `Drivers/CMSIS`。
4. `Drivers/STM32xx_HAL_Driver`。
5. `Drivers/BSP` 与 `Middlewares`。

重点：启动文件、时钟配置、HAL/LL、板级驱动和用户代码区。
