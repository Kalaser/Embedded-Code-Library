# 上游框架与工程目录

本目录是研究入口，不是质量排名。优先选择官方组织维护、文档与源码可以相互印证、包含真实示例工程的项目。平台支持和活跃状态会随版本变化，集成前应检查目标 release 的 CI、issue、release notes 和板卡列表。

| 项目 | 类别 | 研究重点 | 上游 |
|---|---|---|---|
| Apache NuttX | RTOS | POSIX 风格接口、驱动、Apps 分离 | https://github.com/apache/nuttx |
| NuttX Apps | 应用集合 | app 目录、Make.defs/Makefile/Kconfig | https://github.com/apache/nuttx-apps |
| Zephyr | RTOS 平台 | Device Model、Devicetree、Kconfig、west | https://github.com/zephyrproject-rtos/zephyr |
| FreeRTOS Kernel/Distribution | RTOS | 调度、同步、Demo、多平台 Port | https://github.com/FreeRTOS/FreeRTOS |
| RT-Thread | RTOS | BSP、组件、FinSH、软件包 | https://github.com/RT-Thread/rt-thread |
| ESP-IDF | Vendor SDK | Component、CMake、sdkconfig | https://github.com/espressif/esp-idf |
| STM32Cube | Vendor SDK | CMSIS、HAL/LL、BSP、Projects | https://github.com/STMicroelectronics/STM32Cube_MCU_Overall_Offer |
| Pico SDK | MCU SDK | CMake、PIO、硬件 API | https://github.com/raspberrypi/pico-sdk |
| TinyUSB | USB | Core/Class/DCD/HCD/OSAL | https://github.com/hathach/tinyusb |
| lwIP | TCP/IP | pbuf、netif、Raw/Socket API | https://github.com/lwip-tcpip/lwip |
| Nanopb | 序列化 | Protobuf C 生成与运行时 | https://github.com/nanopb/nanopb |
| LittleFS | 文件系统 | COW、掉电恢复、磨损均衡 | https://github.com/littlefs-project/littlefs |
| LVGL | GUI | Widget、事件、显示与输入 port | https://github.com/lvgl/lvgl |
| ETL | C++ 库 | 固定容量容器、FSM、消息 | https://github.com/ETLCPP/etl |
| MCUboot | Bootloader | 镜像、分区、升级和回滚 | https://github.com/mcu-tools/mcuboot |
| Unity/Ceedling | 测试 | C 单元测试、Mock 和测试构建 | https://github.com/ThrowTheSwitch |
| CMSIS | Arm 生态 | Core、Device、DSP、RTOS API | https://github.com/ARM-software/CMSIS_6 |
| Buildroot | Linux 构建 | BR2_EXTERNAL、package、rootfs | https://github.com/buildroot/buildroot |
| Yocto/Poky | Linux 构建 | Layer、Recipe、Machine、Image | https://git.yoctoproject.org/poky/ |
| libopencm3 | Bare metal | 轻量外设库与 MCU 端口 | https://github.com/libopencm3/libopencm3 |
| PlatformIO Examples | 工程示例 | 多平台工程配置 | https://github.com/platformio/platformio-examples |

详细机器可读元数据见 [下载机器可读目录](/assets/downloads/CATALOG.json)。许可证说明仅为初筛备注，不能替代具体版本、子目录和依赖的许可证审查。
