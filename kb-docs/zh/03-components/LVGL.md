# LVGL

- 上游：https://github.com/lvgl/lvgl
- 文档：https://docs.lvgl.io/ 或 https://lvgl.io/docs/open/
- 定位：嵌入式图形界面库。

## 平台移植关注点

- Tick：为 LVGL 提供稳定时间基准。
- Display：实现 display flush，将渲染缓冲输出到屏幕。
- Input：读取 Touch/Keypad/Encoder 输入。
- OSAL：RTOS 下的线程、互斥和同步适配。
- Memory：配置内存池/分配器和 buffer 大小。
- GPU/Draw：根据平台能力选择渲染后端。

## GUI 架构建议

```text
Input Driver
    |
Input/Event Adapter
    |
UI Controller / View Model
    |
LVGL Widgets & Styles
    |
Display Adapter / Flush Callback
    |
LCD Controller / Framebuffer
```

UI 对象应由约定的 UI 线程或锁保护；业务线程通过消息/事件通知 UI，不要多个线程随意操作 widget。

## UIKit 风格二次封装时应学习

- Object 与 parent/child 生命周期。
- Event bubbling/callback 语义。
- Flex/Grid 等布局与尺寸计算。
- Style 的继承、优先级和主题。
- Invalidated region 与刷新调度。
- Screen transition 和动画资源管理。

## 移植入口

https://lvgl.io/docs/open/examples/porting
