# Yocto Layer Template

本模板用于展示一个独立产品 layer 与简单应用 recipe 的组织。它不能脱离兼容版本的 Poky/OpenEmbedded workspace 单独构建。

## 使用

1. 将 `meta-ecl` 加入 `BBLAYERS` 或通过 `bitbake-layers add-layer`。
2. 确认 `LAYERSERIES_COMPAT` 与当前 Yocto release 匹配。
3. recipe 使用本地 `file://` 源码演示；产品构建应固定源码归档或 Git `SRCREV`。
4. 在 image recipe 或 `IMAGE_INSTALL:append` 中加入 `ecl-demo`。
5. 构建并检查 `license.manifest`。

Yocto 的 override syntax 和 layer compatibility 会随 release 演进，按目标 release 文档验证。
