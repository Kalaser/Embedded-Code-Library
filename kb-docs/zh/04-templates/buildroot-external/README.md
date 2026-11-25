# Buildroot External Tree Template

本模板展示如何把产品配置与 Buildroot 上游分离。需要复制到 Buildroot 工作区之外，并按目标 Buildroot 版本检查 external tree API。

## 使用

```bash
make -C /path/to/buildroot BR2_EXTERNAL=/path/to/br2-external-example list-defconfigs
make -C /path/to/buildroot BR2_EXTERNAL=/path/to/br2-external-example example_defconfig
make -C /path/to/buildroot
```

模板 package 使用本地源码路径，便于展示目录关系；产品工程应改为受控源码归档、固定 Git revision 或正式源码发布包。请按实际 Buildroot 版本确认 package infra 和路径变量。
