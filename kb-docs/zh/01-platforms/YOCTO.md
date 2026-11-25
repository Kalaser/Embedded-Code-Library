# Yocto Project 工程体系

## 定位

Yocto Project 提供基于 OpenEmbedded/BitBake 的嵌入式 Linux 构建体系。它通过 layers、recipes、machine、distro 和 image 元数据组织产品系统，适合多个硬件平台、产品变体和长期维护的发行版工程。

## 典型产品层结构

```text
meta-myproduct/
├── conf/
│   ├── layer.conf
│   ├── machine/myboard.conf
│   └── distro/myproduct.conf
├── recipes-core/
│   └── images/myproduct-image.bb
├── recipes-apps/
│   └── myapp/
│       ├── myapp_1.0.bb
│       └── files/
└── recipes-kernel/
    └── linux/linux-%.bbappend
```

## 关键概念

- Layer：按组织、平台或功能划分元数据集合。
- Recipe（`.bb`）：定义软件源、依赖、配置、编译和安装。
- `.bbappend`：在不直接修改上游 recipe 的前提下扩展行为。
- Machine：定义硬件平台和机器特有配置。
- Distro：定义产品发行版策略。
- Image：定义最终根文件系统包含的内容。
- BitBake：解析元数据并执行任务依赖图。

## 架构建议

- 将 BSP、产品发行版和应用功能放在职责清晰的不同 layer。
- 通过 `BBLAYERS` 和 layer dependencies 明确层关系。
- 尽量使用 `.bbappend` 扩展上游 recipe，避免 fork 上游元数据。
- 通过 SRCREV 固定 Git 源码 revision。
- 维护镜像、工具链、License Manifest 和构建缓存策略。

## 主要风险

- layer compatibility 和依赖版本不匹配。
- recipe 使用浮动分支或未固定 SRCREV。
- `BBMASK`、优先级和 append 覆盖行为不清晰。
- 把产品配置直接塞进通用 BSP layer。
- 忽略 SDK、sysroot、许可证清单和可复现构建验证。

## 官方入口

- Documentation: https://docs.yoctoproject.org/
- Source: https://git.yoctoproject.org/poky/
- Layer Index: https://layers.openembedded.org/
