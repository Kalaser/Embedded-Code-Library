# Components

每个组件必须独立声明：职责、API 契约、状态、可移植性等级、内存模型、线程安全、调用上下文、依赖、构建方式、测试结果和已知限制。

新组件使用 `python tools/new_component.py <name>` 生成初始骨架。模板生成后仍需补充 `CMakeLists.txt`、组件元数据和完整测试。
