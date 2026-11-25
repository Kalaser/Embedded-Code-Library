# Buildroot 工程体系

## 定位

Buildroot 用于生成嵌入式 Linux 工具链、内核、Bootloader、根文件系统和应用包。它适合产品组成相对明确、希望通过统一配置构建系统镜像的场景。

## 推荐产品化组织：BR2_EXTERNAL

不要长期直接修改 Buildroot 上游仓库。产品项目可使用独立 external tree：

```text
br2-external-myproduct/
├── external.desc
├── external.mk
├── Config.in
├── configs/
│   └── myproduct_defconfig
├── board/
│   └── myproduct/
│       ├── linux.config
│       ├── rootfs-overlay/
│       └── post-build.sh
└── package/
    └── myapp/
        ├── Config.in
        ├── myapp.mk
        └── src/
```

## 架构思想

- Buildroot 上游负责通用构建系统和 package 基础。
- BR2_EXTERNAL 保存产品特有 package、defconfig、board 和脚本。
- package recipe 描述获取、配置、构建、安装和许可证信息。
- rootfs overlay 用于放置产品级静态配置文件；复杂安装逻辑更适合 package/post-build/post-image 流程。

## 典型命令

```bash
make BR2_EXTERNAL=/path/to/br2-external-myproduct myproduct_defconfig
make
```

## 主要风险

- 不记录 Buildroot 版本和外部层 revision。
- 用 rootfs overlay 直接覆盖大量由 package 管理的文件。
- 在 post-build 脚本中隐藏复杂且不可复现的构建行为。
- 未检查 GPL 源码交付、许可证清单和工具链配置。
- 把开发机上的临时文件或绝对路径带入镜像。

## 官方入口

- Manual: https://buildroot.org/downloads/manual/manual.html
- Source: https://github.com/buildroot/buildroot
