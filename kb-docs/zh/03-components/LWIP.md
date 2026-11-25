# lwIP

- 上游：https://github.com/lwip-tcpip/lwip
- 定位：面向资源受限设备的轻量 TCP/IP 协议栈。

## 主要模块

- `netif`：网络接口抽象。
- `pbuf`：网络数据缓冲链。
- `tcp` / `udp`：传输层协议实现。
- `ip` / `ipv6`：网络层。
- `apps`：部分应用协议和服务。
- `port`：OS、编译器、网卡/以太网驱动相关适配通常由集成项目维护。

## 两种常见 API 模型

- Raw API：回调驱动，通常受 TCP/IP 核心线程/上下文约束，资源开销小。
- Netconn / Socket API：更接近阻塞式应用编程，需要相应 OS 支持和配置。

## 接入决策

根据 RTOS、是否有 Ethernet MAC/Wi-Fi driver、是否需要 socket、多连接数和内存预算选择 API。检查 `NO_SYS`、内存池、pbuf pool、TCP window、MSS、超时和线程配置。

## 源码阅读顺序

`src/core/init.c` → `netif` → `pbuf` → `tcp`/`udp` → `src/api` → `contrib/ports`（若所用发行版包含相应端口）。
