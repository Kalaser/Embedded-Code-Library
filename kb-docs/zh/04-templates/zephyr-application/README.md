# Zephyr Application Template

## 前置条件

- 已安装 Zephyr SDK 和 west。
- 当前目录位于可用的 Zephyr workspace 中，且执行过 `west update`。
- 替换 `-b <board>` 为目标板的正式 Zephyr board target。

## 构建

```bash
west build -b <board> .
west flash
```

## 工程元素

- `CMakeLists.txt`：定义应用构建。
- `prj.conf`：启用所需 Kconfig 功能。
- `src/main.c`：应用入口。
- `boards/`：可放置板卡 overlay/config，实际节点和 binding 依板卡决定。
