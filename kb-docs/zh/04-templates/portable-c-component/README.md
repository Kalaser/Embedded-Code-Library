# Portable C Component Template

这是本模板库中可独立在 Host 构建的最小组件示例。示例组件为固定容量 Ring Buffer，用来展示公共头文件、私有实现、CMake 和单元测试的组织方式。

## 构建

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

## 结构

```text
portable-c-component/
├── CMakeLists.txt
├── include/ecl/ring_buffer.h
├── src/ring_buffer.c
└── tests/test_ring_buffer.c
```

组件本身不创建线程、不分配堆内存，也不隐式加锁。并发保护由调用方负责。
