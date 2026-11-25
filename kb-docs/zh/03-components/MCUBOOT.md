# MCUboot

- 上游：https://github.com/mcu-tools/mcuboot
- 文档：https://docs.mcuboot.com/
- 定位：32-bit MCU 安全启动和固件升级基础设施。

## 架构拆分

MCUboot 将大部分通用启动逻辑放入 bootutil library，各平台的 boot application 负责平台启动、硬件初始化以及最终跳转。这样的拆分让通用逻辑可以进行单元测试。

## 应重点理解

- Flash slot/layout。
- Image header、TLV 和版本信息。
- 签名/哈希验证。
- Swap、overwrite、direct-XIP 等升级模式（以目标配置支持为准）。
- Test/Confirm/Revert 状态。
- Serial Recovery 与管理协议。

## 集成前确认

- Bootloader 与 application 的 Flash layout 一致。
- 目标平台 port 与 flash driver 已验证。
- 签名密钥和生产密钥管理流程可靠。
- 断电、损坏镜像、版本回退和升级中断都有测试。
- 具体 RTOS/SoC 的集成方式按对应官方指南执行。
