# Embedded Linux CMake Application Template

适用于 Buildroot/Yocto/Linux SDK 提供的交叉工具链和 sysroot。该模板本身也可在 Host 上构建，但 Host 构建不等于目标 rootfs/ABI 验证。

## 构建

```bash
cmake -S . -B build
cmake --build build
```

交叉编译时指定 toolchain file，并确保 sysroot 与目标 rootfs 对应。

## 内容

- CMake 项目。
- 简单用户态应用。
- systemd unit 示例（目标系统使用 systemd 时）。
- signal-safe shutdown 的最小示意；真实应用还应定义守护、重启和日志策略。
