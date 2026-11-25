# ESP-IDF Project Template

## 前置条件

安装并导出目标版本 ESP-IDF 环境；模板不包含 ESP-IDF SDK，也不包含芯片专用分区表。

## 构建

```bash
idf.py set-target <chip>
idf.py menuconfig
idf.py build
idf.py -p <port> flash monitor
```

## 组织原则

- `main/` 负责产品启动和应用组装。
- `components/` 存放可复用产品模块。
- 公共依赖通过 `REQUIRES` 声明，私有实现依赖使用 `PRIV_REQUIRES`。
- `sdkconfig.defaults` 放默认配置；构建结果配置按团队策略归档。
- `managed_components/` 是工具管理内容，不作为手工维护源码目录。
