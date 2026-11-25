# C Component Template

复制本目录到 `components/<component_name>/`，然后：

1. 修改 `component.json` 的职责、版本、可移植性和验证状态。
2. 将 `ecl_component` 替换为稳定组件前缀。
3. 定义参数、返回值、所有权、线程安全和调用上下文契约。
4. 实现源码并增加边界/异常测试。
5. 将子目录加入根 CMakeLists.txt。

默认采用 C99、无动态内存示例和 Host 单元测试。模板不预先实现平台功能。
