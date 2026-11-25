# Contributing

1. 新组件先通过 `tools/new_component.py` 创建骨架。
2. 明确功能边界，避免与现有组件重复。
3. 遵守 `knowledge/skills/embedded-c-engineering/` 中的规则；如项目另有更严格规范，以项目规范为准。
4. 为公开 API 编写接口契约和示例。
5. 为正常路径、边界值和错误路径编写测试。
6. 更新 `catalog/components.json`、`CATALOG.md` 或对应组件文档。
7. 第三方代码按 `docs/THIRD_PARTY_INTEGRATION.md` 单独审核。
8. 提交前运行：

```bash
python tools/validate_library.py
cmake -S . -B build -DECL_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```
