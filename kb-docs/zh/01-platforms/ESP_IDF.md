# ESP-IDF 工程体系

## 定位

ESP-IDF 是 Espressif SoC 的官方开发框架。其工程以 component 为基本构建单元，使用 CMake 构建、Kconfig/menuconfig 配置，并集成 FreeRTOS、网络、无线、存储和外设驱动等能力。

## 典型项目结构

```text
my_project/
├── CMakeLists.txt
├── sdkconfig                   # 项目配置，可按团队策略纳入版本管理
├── dependencies.lock           # 使用 Component Manager 时的依赖锁定
├── main/
│   ├── CMakeLists.txt
│   └── app_main.c
├── components/
│   └── my_component/
│       ├── CMakeLists.txt
│       ├── include/
│       └── src/
├── managed_components/         # 工具生成/管理，通常不手工修改
└── partitions.csv              # 可选：分区表
```

## 核心机制

- Project：一个固件应用及其配置、分区和 Bootloader 工程。
- Component：可单独编译和声明依赖的模块。
- `idf_component_register()`：登记源文件、公开/私有头文件路径和依赖。
- `sdkconfig`：菜单配置生成的工程配置。
- Component Manager：管理外部组件依赖；锁定文件用于提高依赖可重复性。
- `app_main()`：应用入口，通常由框架启动流程调用。

## 架构建议

不要把全部产品代码放进 `main/`。把传感器、协议、业务服务、数据模型和设备管理划分为组件；`main/` 只负责应用组装与启动。公开依赖使用 `REQUIRES`，仅实现内部使用的依赖使用 `PRIV_REQUIRES`。

## 构建流程

```bash
idf.py set-target <chip>
idf.py menuconfig
idf.py build
idf.py -p <port> flash monitor
```

## 注意事项

- ESP-IDF 大版本可能影响组件 API、CMake 逻辑及依赖版本。
- `managed_components` 由组件管理器生成，不建议直接改动其中内容；需维护本地修改时，应将组件纳入项目管理方式。
- `sdkconfig.defaults` 可用于提供默认配置；量产工程需要记录最终配置和芯片目标。
- 分区表、NVS、OTA 和 Bootloader 配置应作为产品设计的一部分，不应留到集成末期。

## 官方入口

- Repository: https://github.com/espressif/esp-idf
- Build System: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/build-system.html
- Examples: https://github.com/espressif/esp-idf/tree/master/examples
