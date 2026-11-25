# NuttX API 全量接口使用指南（最新版）

> **数据来源**: Apache NuttX 官方文档 https://nuttx.apache.org/docs/latest/reference/user/  
> **最后更新**: 2026年9月  
> **对应版本**: NuttX latest (Apache 顶级项目)  
> **接口分组**: 15大类，覆盖 180+ 个 C 可调用接口  
> **目标读者**: 固件开发者  

---

## 📋 2009 版 → 最新版 变更摘要

### 新增接口分组（3 个全新大类）

| 新增分组 | 接口 | 说明 |
|----------|------|------|
| **§12 共享内存** | `shmget`/`shmat`/`shmctl`/`shmdt` | 仅 Kernel Build (`CONFIG_BUILD_KERNEL=y`) 可用 |
| **§13 Board IOCTL** | `boardctl` | 非标准接口，绕过设备驱动直接控制板级功能 |
| **§14 日志** | `syslog`/`vsyslog`/`setlogmask` | 标准 SYSLOG 接口 |

### 各分组新增接口一览

| 分组 | 2009版接口数 | 最新版接口数 | 新增接口 |
|------|-------------|-------------|----------|
| Task Control | 7 | 30+ | `task_setcancelstate/type/testcancel`, `fork`/`vfork`/`exec[v/l]`, `posix_spawn` 全家族(17个), `task_spawn`, `waitpid`/`waitid`/`wait`, `atexit`/`on_exit` |
| Task Scheduling | 8 | 9 | `sched_rr_get_interval` + 偶发调度(`CONFIG_SCHED_SPORADIC`) |
| Task Switching | 3 | 3+5 | 原有3个 + `waitpid`/`waitid`/`wait`/`atexit`/`on_exit`（移至此分组） |
| Clocks & Timers | 14 | 17 | `localtime`/`localtime_r`/`gethrtime` |
| Signals | 13 | 13+ | `CONFIG_SIG_DEFAULT` 默认动作支持，Task Group 信号语义 |
| Pthread | 54 | 60+ | `pthread_mutex_timedlock`, `pthread_cleanup_push/pop`, `pthread_yield`, `pthread_attr_setguardsize/getguardsize` |
| File System | 50+ | 60+ | `pread`/`pwrite`, `aio_*` 异步I/O家族, `select` |
| Network | 11 | 11+ | IPv6 支持, DNS 解析器增强 |

### 重大架构变化

| 变化 | 说明 |
|------|------|
| **Task Group 概念** | 任务组模拟 POSIX 进程，主任务+其创建的 pthreads 共享环境变量/文件描述符/socket/pthread键/消息队列 |
| **父子任务关系** | `CONFIG_SCHED_HAVE_PARENT` 启用，`waitpid`/`SIGCHLD` 依赖此设置 |
| **子任务退出状态** | `CONFIG_SCHED_CHILD_STATUS` 保留子任务退出状态，否则 `waitpid` 可能失败 |
| **SMP 支持** | `CONFIG_SMP` + `CONFIG_SMP_NCPUS`，支持多核对称多处理 |
| **信号默认动作** | `CONFIG_SIG_DEFAULT` 启用后，SIGHUP/SIGINT/SIGKILL 等执行异常终止，SIGSTOP/SIGCONT 执行挂起/恢复 |
| **偶发调度** | `CONFIG_SCHED_SPORADIC` 启动 Sporadic 调度策略 |
| **轮转调度时间片** | `CONFIG_RR_INTERVAL` 配置同优先级任务时间片 |
| **看门狗移出用户API** | 2009版的 §2.6 Watchdog Timer 在最新用户API参考中不再单独列出 |

### Watchdog Timer 接口（2009版保留，最新版移入内部）

> ⚠️ 以下接口在 2009 版手册中为 §2.6，在最新官方用户API参考中已不再单独列出。仍可在 NuttX 源码 `include/nuttx/queue.h` 或内部头文件中找到。

| 接口 | 原型 | 说明 |
|------|------|------|
| `wd_create` | `WDOG_ID wd_create(void)` | 分配看门狗 |
| `wd_delete` | `int wd_delete(WDOG_ID wdog)` | 释放看门狗 ⚠️ 不检查使用状态 |
| `wd_start` | `int wd_start(WDOG_ID wdog, int delay, wdentry_t wdentry, int argc, ...)` | 启动定时 |
| `wd_cancel` | `int wd_cancel(WDOG_ID wdog)` | 取消定时 |
| `wd_gettime` | `int wd_gettime(WDOG_ID wdog, struct timespec *ts)` | 获取剩余时间 |

---

## 目录

1. [§1 文档概述与核心概念](#1-文档概述与核心概念)
2. [§2 Task Control 接口（任务控制）](#2-task-control-接口任务控制)
3. [§3 Task Scheduling 接口（任务调度）](#3-task-scheduling-接口任务调度)
4. [§4 Task Control 接口（任务切换/同步/退出钩子）](#4-task-control-接口任务切换同步退出钩子)
5. [§5 Named Message Queue 接口（命名消息队列）](#5-named-message-queue-接口命名消息队列)
6. [§6 Counting Semaphore 接口（计数信号量）](#6-counting-semaphore-接口计数信号量)
7. [§7 Clocks and Timers 接口（时钟与定时器）](#7-clocks-and-timers-接口时钟与定时器)
8. [§8 Signal 接口（信号）](#8-signal-接口信号)
9. [§9 Pthread 接口（POSIX线程）](#9-pthread-接口posix线程)
10. [§10 Environment Variables 接口（环境变量）](#10-environment-variables-接口环境变量)
11. [§11 File System 接口（文件系统）](#11-file-system-接口文件系统)
12. [§12 Network 接口（网络）](#12-network-接口网络)
13. [§13 Shared Memory 接口（共享内存）](#13-shared-memory-接口共享内存)
14. [§14 Board IOCTL 接口（板级控制）](#14-board-ioctl-接口板级控制)
15. [§15 Logging 接口（日志）](#15-logging-接口日志)
16. [§16 OS Data Structures（OS数据结构）](#16-os-data-structuresos数据结构)
17. [§17 关键风险与注意事项汇总](#17-关键风险与注意事项汇总)
18. [§18 POSIX 兼容性偏离汇总](#18-posix-兼容性偏离汇总)
19. [§19 配置宏速查表](#19-配置宏速查表)

---

## 1. 文档概述与核心概念

### 1.1 NuttX 定位

NuttX 是一个实时操作系统（RTOS），强调**标准合规性**和**小体积**。可从 8 位扩展到 64 位微控制器环境。主要遵循的标准是 **POSIX** 和 **ANSI**。对于这些标准未覆盖的功能，采用 Unix 和其他常见 RTOS（如 VxWorks）的 API。

### 1.2 Task vs Pthread

NuttX 是平坦地址空间 OS，不支持 Linux 式进程。但区分 **Task** 和 **Pthread**：

| 维度 | Task | Pthread |
|------|------|--------|
| **文件描述符** | 继承父任务副本（独立），后续互不影响 | 共享父线程的文件描述符 |
| **环境变量** | 继承父任务环境副本（独立） | 共享父线程环境（修改互相可见） |
| **独立性** | 较独立 | 共享更多资源 |
| **创建方式** | `task_create` / `posix_spawn` / `task_spawn` | `pthread_create` |

### 1.3 Task Group（任务组）——最新版新增概念

任务组是 POSIX 进程的模拟：

- 一个**主任务线程** + 其创建的所有 **pthreads** 组成任务组
- 任务组成员**共享**：环境变量、文件描述符、FILE 流、socket、pthread 键、打开的消息队列
- 任务组在**最后一个线程退出时终止**

**关键配置**：

| 配置宏 | 作用 |
|--------|------|
| `CONFIG_SCHED_HAVE_PARENT` | 记住子任务的父任务ID，启用 `SIGCHLD` 和 `waitpid` 的完整语义 |
| `CONFIG_SCHED_CHILD_STATUS` | 保留子任务退出状态，使 `wait`/`waitpid`/`waitid` 可正确获取退出码 |
| `CONFIG_PREALLOC_CHILDSTATUS` | 预分配子状态结构数量，防止内存泄漏 |

⚠️ **警告**：启用 `CONFIG_SCHED_CHILD_STATUS` 后，应用必须通过 `wait()`/`waitpid()` 回收子任务状态，否则会导致内存泄漏。可通过 `SIG_IGN` + `SA_NOCLDWAIT` 抑制保留。

### 1.4 调度策略

| 策略 | 说明 | 配置 |
|------|------|------|
| **SCHED_FIFO**（默认） | 严格优先级调度，同优先级 FIFO | 默认 |
| **SCHED_RR**（轮转） | 同优先级任务通过时间片共享 CPU | `CONFIG_RR_INTERVAL > 0` |
| **SCHED_SPORADIC**（偶发） | 在补充周期内变化线程优先级 | `CONFIG_SCHED_SPORADIC` |

### 1.5 SMP 支持（最新版新增）

| 配置宏 | 作用 |
|--------|------|
| `CONFIG_SMP` | 启用对称多处理 |
| `CONFIG_SMP_NCPUS` | CPU 核心数 |
| `CONFIG_SMP_IDLETHREAD_STACKSIZE` | CPU 1~N-1 的 IDLE 任务栈大小 |

SMP 下引入 **Spinlock**（自旋锁）用于跨 CPU 临界区保护，`g_readytorun` 任务列表分裂为"就绪未分配"和"已分配到 CPU"两个列表。

---

## 2. Task Control 接口（任务控制）

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/01_task_control.html  
> **接口数**: 30+ 个

### 2.1 接口清单

#### 非 POSIX 接口（VxWorks 风格）

| 接口 | 说明 |
|------|------|
| `task_create()` | 创建并激活新任务 |
| `task_delete()` | 删除任务（支持取消点延迟） |
| `task_restart()` | 重启任务 |

#### 取消点扩展（非标准，支持 POSIX Cancellation Points）

| 接口 | 说明 |
|------|------|
| `task_setcancelstate()` | 设置取消状态 |
| `task_setcanceltype()` | 设置取消类型 |
| `task_testcancel()` | 创建取消点 |

#### 标准接口

| 接口 | 说明 |
|------|------|
| `exit()` | 退出当前任务 |
| `_exit()` | 直接退出（不调 atexit/on_exit） |
| `getpid()` | 获取当前任务 ID |
| `sleep()` | 秒级休眠 |
| `usleep()` | 微秒级休眠 |
| `pause()` | 挂起直到信号到达 |

#### fork / vfork / exec 家族

| 接口 | 说明 |
|------|------|
| `fork()` | 创建子进程（仅 Kernel Build） |
| `vfork()` | 创建子进程（共享父地址空间） |
| `exec()` / `execv()` / `execl()` | 替换当前进程映像 |

#### posix_spawn 家族（17 个接口）

| 接口 | 说明 |
|------|------|
| `posix_spawn()` | 按绝对路径生成新进程 |
| `posix_spawnp()` | 按相对路径生成（搜索 PATH 环境变量） |
| `posix_spawn_file_actions_init()` | 初始化文件操作链 |
| `posix_spawn_file_actions_destroy()` | 销毁文件操作链 |
| `posix_spawn_file_actions_addclose()` | 添加关闭文件操作 |
| `posix_spawn_file_actions_adddup2()` | 添加 dup2 操作 |
| `posix_spawn_file_actions_addopen()` | 添加打开文件操作 |
| `posix_spawnattr_init()` | 初始化属性对象 |
| `posix_spawnattr_getflags()` / `setflags()` | 获取/设置标志 |
| `posix_spawnattr_getschedparam()` / `setschedparam()` | 获取/设置调度参数 |
| `posix_spawnattr_getschedpolicy()` / `setschedpolicy()` | 获取/设置调度策略 |
| `posix_spawnattr_getpriority()` / `setpriority()` | 获取/设置优先级 |
| `posix_spawnattr_getsigmask()` / `setsigmask()` | 获取/设置信号掩码 |
| `posix_spawnattr_getstacksize()` / `setstacksize()` | 获取/设置栈大小 |

#### task_spawn（非标准，基于 posix_spawn）

| 接口 | 说明 |
|------|------|
| `task_spawn()` | 类似 `posix_spawn`，但创建任务而非进程 |

### 2.2 核心接口详解

#### task_create

```c
#include <nuttx/config.h>
#include <stdlib.h>

int task_create(char *name, int priority, int stack_size,
                main_t entry, char * const argv[]);
```

**参数**：

| 参数 | 类型 | 说明 |
|------|------|------|
| `name` | `char *` | 任务名称 |
| `priority` | `int` | 优先级（0=最低, 255=最高） |
| `stack_size` | `int` | 栈大小（字节） |
| `entry` | `main_t` | 任务入口函数 |
| `argv` | `char * const[]` | 参数列表，以 NULL 结尾 |

**返回值**：非零任务 ID 或 `-1 (ERROR)`（内存不足或创建失败，**errno 未设置**）

**要点**：
- 参数通过 `strdup` 复制，不依赖调用者字符串生命周期
- 新任务不继承父任务调度特性：默认 `SCHED_FIFO` + 系统默认优先级
- `CONFIG_DEV_CONSOLE` 定义时，stdin/stdout/stderr 被复制
- 入口函数返回时自动调用 `exit()`

**POSIX 兼容性**：**非 POSIX 接口**，VxWorks 风格

#### task_delete

```c
int task_delete(pid_t pid);
```

**返回值**：`0 (OK)` 或 `-1 (ERROR)` + errno

⚠️ **风险**：`task_delete()` 会丢弃任务持有的资源（内存、信号量等），可能导致资源泄漏。如果 `pid` 指向当前运行任务，则重定向到 `exit()`。

**最新版增强**：支持取消点语义 —— 如果取消已禁用或延迟取消已启用，删除将被延迟到下一个取消点。

#### posix_spawn / posix_spawnp

```c
#include <spawn.h>

int posix_spawn(pid_t *pid, const char *path,
                const posix_spawn_file_actions_t *actions,
                const posix_spawnattr_t *attr,
                char *const argv[], char *const envp[]);

int posix_spawnp(pid_t *pid, const char *file,
                 const posix_spawn_file_actions_t *actions,
                 const posix_spawnattr_t *attr,
                 char *const argv[], char *const envp[]);
```

**参数**：

| 参数 | 说明 |
|------|------|
| `pid` | 返回新任务 ID |
| `path`/`file` | `posix_spawn` 用绝对路径；`posix_spawnp` 可用相对路径（搜索 PATH） |
| `actions` | 文件操作链（可为 NULL） |
| `attr` | 属性对象（可为 NULL） |
| `argv` | 参数列表 |
| `envp` | ⚠️ NuttX 中被忽略，新任务无条件继承父任务环境 |

**POSIX 兼容性**：`envp` 被忽略是偏离标准的行为。

---

## 3. Task Scheduling 接口（任务调度）

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/02_task_scheduling.html  
> **接口数**: 9 个

### 3.1 接口清单

| 接口 | 头文件 | 说明 |
|------|--------|------|
| `sched_setparam()` | `<sched.h>` | 设置任务优先级 |
| `sched_getparam()` | `<sched.h>` | 获取任务优先级 |
| `sched_setscheduler()` | `<sched.h>` | 设置调度策略+优先级 |
| `sched_getscheduler()` | `<sched.h>` | 获取调度策略 |
| `sched_yield()` | `<sched.h>` | 让出 CPU |
| `sched_get_priority_max()` | `<sched.h>` | 获取最大优先级 |
| `sched_get_priority_min()` | `<sched.h>` | 获取最小优先级 |
| `sched_rr_get_interval()` | `<sched.h>` | 获取轮转调度时间片 🆕 |

### 3.2 核心接口详解

#### sched_setparam

```c
#include <sched.h>

int sched_setparam(pid_t pid, FAR const struct sched_param *param);
```

**参数**：

| 参数 | 说明 |
|------|------|
| `pid` | 任务 ID，0 表示当前任务 |
| `param` | `sched_param` 结构体，`sched_priority` 成员为优先级值 |

**返回值**：`0 (OK)` 或 `-1 (ERROR)` + errno

| errno | 说明 |
|-------|------|
| `EINVAL` | 参数无效 |
| `EPERM` | 权限不足 |
| `ESRCH` | 找不到任务 |

**优先级范围**：`SCHED_PRIORITY_MIN` ~ `SCHED_PRIORITY_MAX`（0 ~ 255，0=最低, 255=最高）

💡 **Note**：将优先级设为相同值的效果类似 `sched_yield()`：任务被移到同优先级队列末尾。

#### sched_setscheduler

```c
int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);
```

**参数**：

| 参数 | 说明 |
|------|------|
| `pid` | 任务 ID，0 表示当前任务 |
| `policy` | `SCHED_FIFO` 或 `SCHED_RR` |
| `param` | 包含优先级的结构体 |

**返回值**：`0 (OK)` 或 `-1 (ERROR)` + errno（`EINVAL`/`ESRCH`）

#### sched_rr_get_interval 🆕

```c
int sched_rr_get_interval(pid_t pid, struct timespec *interval);
```

获取指定任务的轮转调度时间片间隔。需 `CONFIG_RR_INTERVAL > 0`。

---

## 4. Task Control 接口（任务切换/同步/退出钩子）

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/03_task_control.html  
> **接口数**: 8 个

### 4.1 调度器锁定接口

| 接口 | 头文件 | 说明 |
|------|--------|------|
| `sched_lock()` | `<sched.h>` | 禁止抢占 |
| `sched_unlock()` | `<sched.h>` | 恢复抢占 |
| `sched_lockcount()` | `<sched.h>` | 获取锁定计数 |

**要点**：
- `sched_lock()` 阻止所有任务切换（包括更高优先级任务）
- 支持嵌套，需匹配调用 `sched_unlock()`
- ⚠️ 优先使用信号量而非 `sched_lock()`，后者影响系统响应时间

### 4.2 任务同步接口（等待子任务终止）

| 接口 | 头文件 | 说明 |
|------|--------|------|
| `waitpid()` | `<sys/wait.h>` | 等待指定子任务终止 |
| `waitid()` | `<sys/wait.h>` | 等待子任务状态变化 |
| `wait()` | `<sys/wait.h>` | 等待任意子任务终止 |

**关键配置依赖**：

| 配置 | 不启用时的行为 |
|------|---------------|
| `CONFIG_SCHED_HAVE_PARENT` | `waitpid` 无法区分调用者的子任务 |
| `CONFIG_SCHED_CHILD_STATUS` | 子任务可能在 `wait` 调用前就退出，导致 `wait` 失败 |

⚠️ **变通方案**：如果未启用 `CONFIG_SCHED_CHILD_STATUS`，可用 `sched_lock()` → 启动子任务 → `wait` → `sched_unlock()` 序列保证子任务不会在 wait 之前运行。

### 4.3 任务退出钩子

| 接口 | 头文件 | 说明 |
|------|--------|------|
| `atexit()` | `<stdlib.h>` | 注册正常退出回调 |
| `on_exit()` | `<stdlib.h>` | 注册退出回调（带退出状态参数） |

**要点**：
- 回调在**任务组**终止时执行（最后一个线程退出时）
- `atexit` 注册的函数无参数；`on_exit` 注册的函数接收退出状态码

---

## 5. Named Message Queue 接口（命名消息队列）

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/04_message_queue.html  
> **接口数**: 10 个

### 5.1 接口清单

| 接口 | 头文件 | 说明 |
|------|--------|------|
| `mq_open()` | `<mqueue.h>` | 打开/创建命名消息队列 |
| `mq_close()` | `<mqueue.h>` | 关闭消息队列 |
| `mq_unlink()` | `<mqueue.h>` | 移除消息队列名称 |
| `mq_send()` | `<mqueue.h>` | 发送消息（阻塞） |
| `mq_timedsend()` | `<mqueue.h>` | 定时发送消息 |
| `mq_receive()` | `<mqueue.h>` | 接收消息（阻塞） |
| `mq_timedreceive()` | `<mqueue.h>` | 定时接收消息 |
| `mq_notify()` | `<mqueue.h>` | 注册通知（队列空→非空时发信号） |
| `mq_setattr()` | `<mqueue.h>` | 设置队列属性 |
| `mq_getattr()` | `<mqueue.h>` | 获取队列属性 |

### 5.2 核心接口详解

#### mq_open

```c
#include <mqueue.h>

mqd_t mq_open(const char *mqName, int oflags, ...);
```

**参数**：

| 参数 | 说明 |
|------|------|
| `mqName` | 队列名称 |
| `oflags` | `O_RDONLY` / `O_WRONLY` / `O_RDWR` / `O_CREAT` / `O_EXCL` / `O_NONBLOCK` |
| `mode`（可选） | 权限位（必需但不使用） |
| `attr`（可选） | `mq_attr` 结构体，`mq_maxmsg` 和 `mq_msgsize` 在创建时使用 |

**返回值**：消息队列描述符或 `-1 (ERROR)`

⚠️ **限制**：消息最大大小硬编码为 **22 字节**。

#### mq_notify

```c
int mq_notify(mqd_t mqdes, FAR const struct sigevent *notification);
```

**参数**：`notification` 包含 `sigev_notify`（应为 `SIGEV_SIGNAL`，但实际被忽略）、`sigev_signo`、`sigev_value`

**返回值**：`0 (OK)` 或 `-1 (ERROR)` + errno（`EBADF`/`EBUSY`/`EINVAL`/`ENOMEM`）

**要点**：每个队列只能附加一个通知；通知触发后自动分离。

---

## 6. Counting Semaphore 接口（计数信号量）

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/05_counting_semaphore.html  
> **接口数**: 9 个

### 6.1 接口清单

| 接口 | 头文件 | 说明 |
|------|--------|------|
| `sem_init()` | `<semaphore.h>` | 初始化无名信号量 |
| `sem_destroy()` | `<semaphore.h>` | 销毁无名信号量 |
| `sem_open()` | `<semaphore.h>` | 打开命名信号量 |
| `sem_close()` | `<semaphore.h>` | 关闭命名信号量 |
| `sem_unlink()` | `<semaphore.h>` | 移除命名信号量 |
| `sem_wait()` | `<semaphore.h>` | 等待（阻塞） |
| `sem_trywait()` | `<semaphore.h>` | 尝试等待（非阻塞） |
| `sem_post()` | `<semaphore.h>` | 释放 |
| `sem_getvalue()` | `<semaphore.h>` | 获取计数值 |

### 6.2 优先级继承（Priority Inheritance）

NuttX 通过 `CONFIG_PRIORITY_INHERITANCE` 支持优先级继承，避免优先级反转。

**配置宏**：

| 配置 | 作用 |
|------|------|
| `CONFIG_PRIORITY_INHERITANCE` | 启用优先级继承 |
| `CONFIG_SEM_PREALLOCHOLDERS` | 预分配持有者结构数量（0=禁用或仅互斥使用或≤2线程） |
| `CONFIG_SEM_PREALLOCCONDS` | 预分配条件结构数量 |

**成本**：代码略增 + 约 6-12 字节 × `CONFIG_SEM_PREALLOCHOLDERS` 值

⚠️ **风险**：优先级继承实现复杂，存在边界条件。计数信号量可有多个持有者，管理数据结构开销随持有者数增长。

---

## 7. Clocks and Timers 接口（时钟与定时器）

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/06_clocks_timers.html  
> **接口数**: 17 个

### 7.1 接口清单

| 接口 | 头文件 | 说明 |
|------|--------|------|
| `clock_settime()` | `<time.h>` | 设置指定时钟时间 |
| `clock_gettime()` | `<time.h>` | 获取指定时钟时间 |
| `clock_getres()` | `<time.h>` | 获取时钟分辨率 |
| `mktime()` | `<time.h>` | 将 `struct tm` 转为 `time_t` |
| `gmtime()` | `<time.h>` | `time_t` → `struct tm`（GMT，非可重入） |
| `localtime()` 🆕 | `<time.h>` | `time_t` → `struct tm`（本地时间，非可重入） |
| `asctime()` | `<time.h>` | `struct tm` → 字符串（非可重入） |
| `ctime()` | `<time.h>` | `time_t` → 字符串（非可重入） |
| `gmtime_r()` | `<time.h>` | `gmtime` 可重入版本 |
| `localtime_r()` 🆕 | `<time.h>` | `localtime` 可重入版本 |
| `asctime_r()` | `<time.h>` | `asctime` 可重入版本 |
| `ctime_r()` | `<time.h>` | `ctime` 可重入版本 |
| `timer_create()` | `<time.h>` | 创建每进程定时器 |
| `timer_delete()` | `<time.h>` | 删除定时器 |
| `timer_settime()` | `<time.h>` | 设置定时器时间 |
| `timer_gettime()` | `<time.h>` | 获取定时器剩余时间 |
| `timer_getoverrun()` | `<time.h>` | 获取定时器溢出次数 |
| `gettimeofday()` | `<sys/time.h>` | 获取当前时间 |
| `gethrtime()` 🆕 | `<time.h>` | 获取高分辨率时间（纳秒） |

### 7.2 核心接口详解

#### clock_gettime

```c
#include <time.h>

int clock_gettime(clockid_t clockid, struct timespec *tp);
```

**参数**：`clockid` 通常为 `CLOCK_REALTIME`

**返回值**：`0 (OK)` 或非零错误号

#### timer_create / timer_settime

```c
int timer_create(clockid_t clockid, FAR struct sigevent *evp,
                 timer_t *timerid);
int timer_settime(timer_t timerid, int flags,
                  FAR const struct itimerspec *value,
                  FAR struct itimerspec *ovalue);
```

**要点**：定时器到期时通过 `sigevent` 发送信号通知。

#### gethrtime 🆕

```c
#include <time.h>

hrtime_t gethrtime(void);
```

返回自系统启动以来的纳秒数，高分辨率单调时钟。

---

## 8. Signal 接口（信号）

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/07_signals.html  
> **接口数**: 13 个 + 信号默认动作表

### 8.1 接口清单

| 接口 | 头文件 | 说明 |
|------|--------|------|
| `sigemptyset()` | `<signal.h>` | 清空信号集 |
| `sigfillset()` | `<signal.h>` | 填满信号集 |
| `sigaddset()` | `<signal.h>` | 向信号集添加信号 |
| `sigdelset()` | `<signal.h>` | 从信号集删除信号 |
| `sigismember()` | `<signal.h>` | 测试信号是否在集中 |
| `sigaction()` | `<signal.h>` | 设置信号处理动作 |
| `sigprocmask()` | `<signal.h>` | 设置信号掩码 |
| `sigpending()` | `<signal.h>` | 获取挂起信号 |
| `sigsuspend()` | `<signal.h>` | 挂起等待信号 |
| `sigwaitinfo()` | `<signal.h>` | 等待信号（不阻塞） |
| `sigtimedwait()` | `<signal.h>` | 定时等待信号 |
| `sigqueue()` | `<signal.h>` | 排队发送信号+数据 |
| `kill()` | `<signal.h>` | 发送信号给任务 |

### 8.2 信号默认动作 🆕

**默认行为**：所有信号默认**忽略**（即所有信号默认为实时信号）。启用 `CONFIG_SIG_DEFAULT` 后：

#### 异常终止信号

| 信号 | 默认动作 | 配置宏 |
|------|----------|--------|
| `SIGHUP` | 异常终止 | `CONFIG_SIG_SIGKILL_ACTION` |
| `SIGINT` | 异常终止 | `CONFIG_SIG_SIGKILL_ACTION` |
| `SIGQUIT` | 异常终止 | `CONFIG_SIG_SIGKILL_ACTION` |
| `SIGILL` | 异常终止 | `CONFIG_SIG_SIGKILL_ACTION` |
| `SIGTRAP` | 异常终止 | `CONFIG_SIG_SIGKILL_ACTION` |
| `SIGABRT` | 异常终止 | `CONFIG_SIG_SIGKILL_ACTION` |
| `SIGBUS` | 异常终止 | `CONFIG_SIG_SIGKILL_ACTION` |
| `SIGFPE` | 异常终止 | `CONFIG_SIG_SIGKILL_ACTION` |
| `SIGKILL` | 异常终止 | `CONFIG_SIG_SIGKILL_ACTION` |
| `SIGSEGV` | 异常终止 | `CONFIG_SIG_SIGKILL_ACTION` |
| `SIGTERM` | 异常终止 | `CONFIG_SIG_SIGKILL_ACTION` |
| `SIGUSR1` | 异常终止 | `CONFIG_SIG_SIGUSR1_ACTION` |
| `SIGUSR2` | 异常终止 | `CONFIG_SIG_SIGUSR2_ACTION` |
| `SIGPIPE` | 异常终止 | `CONFIG_SIG_SIGPIPE_ACTION` |
| `SIGALRM` | 异常终止 | `CONFIG_SIG_SIGALRM_ACTION` |
| `SIGVTALRM` | 异常终止 | `CONFIG_SIG_SIGALRM_ACTION` |
| `SIGPROF` | 异常终止 | `CONFIG_SIG_SIGPROF_ACTION` |
| `SIGPOLL` | 异常终止 | `CONFIG_SIG_SIGPOLL_ACTION` |

#### 作业控制信号

| 信号 | 默认动作 | 配置宏 |
|------|----------|--------|
| `SIGSTOP` | 挂起任务 | `CONFIG_SIG_SIGSTOP_ACTION` |
| `SIGTSTP` | 挂起任务 | `CONFIG_SIG_SIGSTOP_ACTION` |
| `SIGTTIN` | 挂起任务 | `CONFIG_SIG_SIGSTOP_ACTION` |
| `SIGTTOU` | 挂起任务 | `CONFIG_SIG_SIGSTOP_ACTION` |
| `SIGCONT` | 恢复任务 | `CONFIG_SIG_SIGSTOP_ACTION` |

#### 默认忽略

| 信号 | 默认动作 | 配置宏 |
|------|----------|--------|
| `SIGURG` | 忽略 | `CONFIG_SIG_SIGURG_ACTION` |

### 8.3 Task Group 信号语义 🆕

最新版引入了 **Task Group** 的信号语义，模拟 POSIX 进程级信号行为：

- 向任务组主任务 ID 发送信号 → 按 POSIX 规则分发到组内线程
- 可通过信号掩码控制哪个线程接收信号
- 线程继承父线程的信号掩码

### 8.4 sigaction

```c
#include <signal.h>

int sigaction(int sig, FAR const struct sigaction *act,
              FAR struct sigaction *oact);
```

**参数**：

| 参数 | 说明 |
|------|------|
| `sig` | 信号编号 |
| `act` | 新处理动作（可为 NULL） |
| `oact` | 保存旧处理动作（可为 NULL） |

**POSIX 兼容性偏离**：
- `sa_handler` 中的特殊值 `SIG_DFL`/`SIG_IGN` 处理依赖 `CONFIG_SIG_DEFAULT`
- `sa_flags` 全部按 `SA_SIGINFO` 处理（忽略其他标志）

---

## 9. Pthread 接口（POSIX线程）

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/08_pthread.html  
> **接口数**: 60+ 个

### 9.1 接口分类清单

#### 线程属性

| 接口 | 说明 |
|------|------|
| `pthread_attr_init()` / `destroy()` | 初始化/销毁属性对象 |
| `pthread_attr_setschedpolicy()` / `getschedpolicy()` | 设置/获取调度策略 |
| `pthread_attr_setschedparam()` / `getschedparam()` | 设置/获取调度参数 |
| `pthread_attr_setinheritsched()` / `getinheritsched()` | 设置/获取继承调度 |
| `pthread_attr_setstacksize()` / `getstacksize()` | 设置/获取栈大小 |
| `pthread_attr_setguardsize()` 🆕 / `getguardsize()` 🆕 | 设置/获取 guard 大小 |

#### 线程控制

| 接口 | 说明 |
|------|------|
| `pthread_create()` | 创建线程 |
| `pthread_detach()` | 分离线程 |
| `pthread_exit()` | 退出线程 |
| `pthread_cancel()` | 取消线程 |
| `pthread_setcancelstate()` | 设置取消状态 |
| `pthread_setcanceltype()` | 设置取消类型 |
| `pthread_testcancel()` | 创建取消点 |
| `pthread_cleanup_push()` 🆕 | 注册清理处理程序 |
| `pthread_cleanup_pop()` 🆕 | 弹出清理处理程序 |
| `pthread_join()` | 等待线程终止 |
| `pthread_yield()` 🆕 | 让出 CPU |
| `pthread_self()` | 获取自身线程 ID |
| `pthread_getschedparam()` | 获取线程调度参数 |
| `pthread_setschedparam()` | 设置线程调度参数 |

#### 线程特定数据

| 接口 | 说明 |
|------|------|
| `pthread_key_create()` | 创建键 |
| `pthread_setspecific()` | 设置线程特定数据 |
| `pthread_getspecific()` | 获取线程特定数据 |
| `pthread_key_delete()` | 删除键 ⚠️ 实际无操作 |

#### 互斥锁

| 接口 | 说明 |
|------|------|
| `pthread_mutexattr_init()` / `destroy()` | 初始化/销毁互斥锁属性 |
| `pthread_mutexattr_getpshared()` / `setpshared()` | 获取/设置进程共享 |
| `pthread_mutexattr_gettype()` / `settype()` | 获取/设置类型 |
| `pthread_mutexattr_getprotocol()` / `setprotocol()` | 获取/设置协议 |
| `pthread_mutex_init()` / `destroy()` | 初始化/销毁互斥锁 |
| `pthread_mutex_lock()` | 加锁（阻塞） |
| `pthread_mutex_timedlock()` 🆕 | 定时加锁 |
| `pthread_mutex_trylock()` | 尝试加锁 |
| `pthread_mutex_unlock()` | 解锁 |

#### 条件变量

| 接口 | 说明 |
|------|------|
| `pthread_condattr_init()` / `destroy()` | 初始化/销毁条件变量属性 |
| `pthread_cond_init()` / `destroy()` | 初始化/销毁条件变量 |
| `pthread_cond_broadcast()` | 唤醒所有等待者 |
| `pthread_cond_signal()` | 唤醒一个等待者 |
| `pthread_cond_wait()` | 等待条件 |
| `pthread_cond_timedwait()` | 定时等待条件 |

#### 屏障（Barriers）

| 接口 | 说明 |
|------|------|
| `pthread_barrierattr_init()` / `destroy()` | 初始化/销毁屏障属性 |
| `pthread_barrierattr_setpshared()` / `getpshared()` | 设置/获取进程共享 |
| `pthread_barrier_init()` / `destroy()` | 初始化/销毁屏障 |
| `pthread_barrier_wait()` | 等待屏障 |

#### 初始化与信号

| 接口 | 说明 |
|------|------|
| `pthread_once()` | 一次性初始化 |
| `pthread_kill()` | 向线程发送信号 |
| `pthread_sigmask()` | 设置线程信号掩码 |

### 9.2 不支持的 Pthread 接口

| 接口 | 原因 |
|------|------|
| `pthread_attr_getscope()` / `setscope()` | 不支持 contention scope |
| `pthread_getcpuclockid()` | 不支持线程 CPU 时间时钟 |
| `pthread_mutex_getprioceiling()` / `setprioceiling()` | 不支持优先级上限 |
| `pthread_mutexattr_getprioceiling()` / `setprioceiling()` | 不支持优先级上限属性 |

### 9.3 核心接口详解

#### pthread_create

```c
#include <pthread.h>

int pthread_create(pthread_t *thread, pthread_attr_t *attr,
                   pthread_startroutine_t startRoutine,
                   pthread_addr_t arg);
```

**要点**：
- 新线程继承父线程的信号掩码和环境变量（共享）
- 默认取消状态为 `PTHREAD_CANCEL_ENABLE`，取消类型为 `PTHREAD_CANCEL_DEFERRED`
- 默认调度策略 `SCHED_FIFO`

#### pthread_mutex_timedlock 🆕

```c
#include <pthread.h>

int pthread_mutex_timedlock(pthread_mutex_t *mutex,
                            FAR const struct timespec *abstime);
```

在指定绝对时间前阻塞加锁，超时返回 `ETIMEDOUT`。

---

## 10. Environment Variables 接口（环境变量）

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/09_env_vars.html  
> **接口数**: 5 个

### 10.1 接口清单

| 接口 | 头文件 | 说明 |
|------|--------|------|
| `getenv()` | `<stdlib.h>` | 获取环境变量值 |
| `putenv()` | `<stdlib.h>` | 设置环境变量（name=value 格式） |
| `clearenv()` | `<stdlib.h>` | 清除所有环境变量 |
| `setenv()` | `<stdlib.h>` | 设置环境变量（分离参数） |
| `unsetenv()` | `<stdlib.h>` | 删除环境变量 |

### 10.2 Task vs Pthread 环境继承

| | Task | Pthread |
|---|------|--------|
| **继承方式** | 复制父任务环境（独立副本） | 共享父线程环境 |
| **后续修改** | 互不影响 | 互相可见 |

### 10.3 禁用

`CONFIG_DISABLE_ENVIRON` 可禁用全部环境变量支持。

---

## 11. File System 接口（文件系统）

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/10_filesystem.html  
> **接口数**: 60+ 个

### 11.1 文件系统概述

NuttX 文件系统是**可选的、可裁剪的**。核心是：
- **伪根文件系统**：内存中的伪文件系统（类似 Linux /proc），不需要存储介质
- **挂载文件系统**：通过 `mount()` 绑定块设备到挂载点
- 支持的文件系统：VFAT 等

### 11.2 接口分类

#### 驱动操作（`fcntl.h`）

| 接口 | 说明 |
|------|------|
| `creat()` | 创建文件 |
| `open()` | 打开文件 |
| `fcntl()` | 文件控制 |

#### 文件描述符操作（`unistd.h`）

| 接口 | 说明 |
|------|------|
| `close()` | 关闭文件 |
| `dup()` | 复制文件描述符 |
| `dup2()` | 复制到指定描述符 |
| `lseek()` | 移动文件偏移 |
| `pread()` 🆕 | 定位读取 |
| `pwrite()` 🆕 | 定位写入 |
| `read()` | 读取文件 |
| `unlink()` | 删除文件 |
| `write()` | 写入文件 |
| `fsync()` | 同步文件到磁盘 |
| `ftruncate()` | 截断文件 |

#### 设备控制（`sys/ioctl.h`）

| 接口 | 说明 |
|------|------|
| `ioctl()` | 设备 I/O 控制 |

#### I/O 多路复用（`poll.h`）

| 接口 | 说明 |
|------|------|
| `poll()` | 等待文件描述符就绪 |
| `select()` 🆕 | 监视多组文件描述符 |

#### 目录操作（`dirent.h`）

| 接口 | 说明 |
|------|------|
| `closedir()` | 关闭目录 |
| `opendir()` | 打开目录 |
| `readdir()` | 读取目录项 |
| `readdir_r()` | 可重入读取目录项 |
| `rewinddir()` | 回卷目录 |
| `seekdir()` | 定位目录 |
| `telldir()` | 获取目录位置 |

#### 文件系统操作（`sys/stat.h` / `sys/mount.h`）

| 接口 | 说明 |
|------|------|
| `stat()` / `fstat()` / `lstat()` | 获取文件状态 |
| `mkdir()` | 创建目录 |
| `rmdir()` | 删除目录 |
| `rename()` | 重命名 |
| `chmod()` | 修改权限 |
| `mount()` | 挂载文件系统 |
| `umount()` | 卸载文件系统 |

#### 标准流操作（`stdio.h`）

| 接口 | 说明 |
|------|------|
| `fopen()` / `fclose()` | 打开/关闭流 |
| `fread()` / `fwrite()` | 读/写流 |
| `fgets()` / `gets()` | 读行 |
| `fputs()` / `puts()` | 写行 |
| `fprintf()` / `printf()` / `sprintf()` / `snprintf()` | 格式化输出 |
| `fscanf()` / `scanf()` / `sscanf()` | 格式化输入 |
| `fflush()` | 刷新流 |
| `fseek()` / `ftell()` / `rewind()` | 流定位 |
| `ferror()` / `feof()` / `clearerr()` | 错误/EOF 处理 |

#### 异步 I/O 🆕（`aio.h`）

| 接口 | 说明 |
|------|------|
| `aio_read()` | 异步读取 |
| `aio_write()` | 异步写入 |
| `aio_fsync()` | 异步同步 |
| `aio_error()` | 获取异步操作错误 |
| `aio_return()` | 获取异步操作返回值 |
| `aio_cancel()` | 取消异步操作 |
| `aio_suspend()` | 等待异步操作完成 |
| `lio_listio()` | 批量异步 I/O |

#### Pipe & FIFO（`unistd.h`）

| 接口 | 说明 |
|------|------|
| `pipe()` | 创建管道 |
| `mkfifo()` | 创建 FIFO |

---

## 12. Network 接口（网络）

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/11_network.html  
> **接口数**: 11+ 个

### 12.1 接口清单

| 接口 | 头文件 | 说明 |
|------|--------|------|
| `socket()` | `<sys/socket.h>` | 创建 socket |
| `bind()` | `<sys/socket.h>` | 绑定地址 |
| `connect()` | `<sys/socket.h>` | 连接 |
| `listen()` | `<sys/socket.h>` | 监听 |
| `accept()` | `<sys/socket.h>` | 接受连接 |
| `send()` | `<sys/socket.h>` | 发送数据 |
| `sendto()` | `<sys/socket.h>` | 发送到指定地址 |
| `recv()` | `<sys/socket.h>` | 接收数据 |
| `recvfrom()` | `<sys/socket.h>` | 从指定地址接收 |
| `setsockopt()` | `<sys/socket.h>` | 设置 socket 选项 |
| `getsockopt()` | `<sys/socket.h>` | 获取 socket 选项 |

### 12.2 最新版增强

| 特性 | 说明 |
|------|------|
| **IPv6 支持** | 支持 `AF_INET6` 地址族 |
| **DNS 解析器** | 轻量级 DNS 解析器，支持多 nameserver，IPv4/IPv6 查询，nameserver 变更回调 |
| **Socket 类型** | `SOCK_STREAM`（TCP）、`SOCK_DGRAM`（UDP）、`SOCK_RAW`（原始套接字） |
| **6LoWPAN** | 支持低速无线个人区域网 |
| **SocketCAN** | 支持 CAN 总线 socket 接口 |
| **NAT** | 网络地址转换支持 |
| **Netlink** | 路由 netlink 支持 |

---

## 13. Shared Memory 接口（共享内存）🆕

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/12_shared_memory.html  
> **接口数**: 4 个  
> **可用条件**: 仅 Kernel Build（`CONFIG_BUILD_KERNEL=y`）

### 13.1 接口清单

| 接口 | 头文件 | 说明 |
|------|--------|------|
| `shmget()` | `<sys/shm.h>` | 获取/创建共享内存段 |
| `shmat()` | `<sys/shm.h>` | 附加共享内存段到地址空间 |
| `shmctl()` | `<sys/shm.h>` | 共享内存控制操作 |
| `shmdt()` | `<sys/shm.h>` | 分离共享内存段 |

### 13.2 核心接口详解

#### shmget

```c
#include <sys/shm.h>

int shmget(key_t key, size_t size, int shmflg);
```

**参数**：

| 参数 | 说明 |
|------|------|
| `key` | 访问键，`IPC_PRIVATE` 创建新段 |
| `size` | 共享内存段最小大小（字节） |
| `shmflg` | `IPC_CREAT` / `IPC_EXCL` 等 |

**返回值**：非负共享内存 ID 或 `-1` + errno

#### shmat

```c
void *shmat(int shmid, FAR const void *shmaddr, int shmflg);
```

**附加规则**：
- `shmaddr == NULL`：系统选择地址
- `shmaddr != NULL` + `SHM_RND`：对齐到 `SHMLBA` 边界
- `shmaddr != NULL` 无 `SHM_RND`：使用指定地址
- `SHM_RDONLY`：只读附加

**POSIX 偏离**：`ipc_perm` 结构不支持 `cuid`/`uid`/`cgid`/`gid` 字段（用户/组 ID 尚未支持）。

---

## 14. Board IOCTL 接口（板级控制）🆕

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/13_boardctl.html  
> **接口数**: 1 个函数 + 15+ 个命令

### 14.1 boardctl 函数

```c
#include <sys/boardctl.h>

int boardctl(unsigned int cmd, uintptr_t arg);
```

**参数**：

| 参数 | 说明 |
|------|------|
| `cmd` | 板级命令，见 `include/sys/boardctl.h` |
| `arg` | 命令参数，类型取决于具体命令 |

**返回值**：`0 (OK)` 或 `-1 (ERROR)` + errno

### 14.2 支持的命令

#### 系统状态控制

| 命令 | 说明 | 配置 |
|------|------|------|
| `BOARDIOC_POWEROFF` | 关机 | `CONFIG_BOARDCTL_POWEROFF` |
| `BOARDIOC_RESET` | 重置板卡 | `CONFIG_BOARDCTL_RESET` |

#### 电源管理

| 命令 | 说明 | 配置 |
|------|------|------|
| `BOARDIOC_PM_CONTROL` | 管理电源状态转换/查询 | `CONFIG_PM` |

#### 板卡信息

| 命令 | 说明 | 配置 |
|------|------|------|
| `BOARDIOC_UNIQUEID` | 获取板卡唯一 ID（序列号/MAC） | 需 `board_uniqueid()` |
| `BOARDIOC_MACADDR` | 获取网络 MAC 地址 | `CONFIG_BOARDCTL_MACADDR` |

#### 文件系统

| 命令 | 说明 | 配置 |
|------|------|------|
| `BOARDIOC_MKRD` | 创建 RAM 磁盘 | `CONFIG_BOARDCTL_MKRD` |
| `BOARDIOC_ROMDISK` | 注册 ROM 磁盘 | `CONFIG_BOARDCTL_ROMDISK` |

#### 符号处理

| 命令 | 说明 | 配置 |
|------|------|------|
| `BOARDIOC_APP_SYMTAB` | 选择应用符号表 | `CONFIG_BOARDCTL_APP_SYMTAB` |
| `BOARDIOC_OS_SYMTAB` | 选择 OS 符号表 | `CONFIG_BOARDCTL_OS_SYMTAB` |

💡 **要点**：`boardctl()` 是用户空间接口。OS 内部可直接调用 `include/nuttx/board.h` 中的 `board_*` 函数。

---

## 15. Logging 接口（日志）🆕

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/14_logging.html  
> **接口数**: 3 个

### 15.1 接口清单

| 接口 | 头文件 | 说明 |
|------|--------|------|
| `syslog()` | `<syslog.h>` | 发送日志消息（printf 格式） |
| `vsyslog()` | `<syslog.h>` | 可变参数版本 |
| `setlogmask()` | `<syslog.h>` | 设置日志掩码 |

### 15.2 优先级

| 优先级宏 | 说明 |
|----------|------|
| `LOG_EMERG` | 系统不可用 |
| `LOG_ALERT` | 必须立即采取行动 |
| `LOG_CRIT` | 严重条件 |
| `LOG_ERR` | 错误条件 |
| `LOG_WARNING` | 警告条件 |
| `LOG_NOTICE` | 正常但重要 |
| `LOG_INFO` | 信息 |
| `LOG_DEBUG` | 调试 |

### 15.3 掩码宏

| 宏 | 说明 |
|----|------|
| `LOG_MASK(p)` | 返回指定优先级的掩码 |
| `LOG_UPTO(p)` | 返回到指定优先级为止的所有掩码 |
| `LOG_ALL` | 所有优先级启用 |

### 15.4 构建模式影响

| 构建模式 | 掩码范围 |
|----------|----------|
| **Flat Build** | 一个全局 SYSLOG 掩码 |
| **Protected Build** | 内核 + 用户空间各一个掩码 |
| **Kernel Build** | 每个用户进程一个掩码 + 内核掩码（符合 POSIX） |

⚠️ `setlogmask()` 不是线程安全的，并发使用行为未定义。

⚠️ `openlog()` 和 `closelog()` 目前不支持。

---

## 16. OS Data Structures（OS数据结构）

> **来源**: https://nuttx.apache.org/docs/latest/reference/user/structures.html

### 16.1 标量类型

| 类型 | 用途 |
|------|------|
| `pid_t` | 任务/进程 ID |
| `size_t` | 大小 |
| `sigset_t` | 信号集 |
| `time_t` | 时间（秒） |

### 16.2 隐藏接口结构

以下结构对应用层是**不透明**的，应作为句柄使用：

| 类型/结构 | 说明 |
|-----------|------|
| `struct tcb_s` | 任务控制块 |
| `mqd_t` | 消息队列描述符 |
| `sem_t` | 信号量 |
| `pthread_key_t` | 线程特定数据键 |

### 16.3 errno 访问

```c
#include <errno.h>

#define errno *__errno()

int *__errno(void);
```

**要点**：
- 每个任务有唯一的、私有的 `errno` 值
- 每个 **pthread** 也有自己的 `errno` 副本，不共享（可能非标准，但促进线程独立性）

### 16.4 用户接口结构

#### main_t —— 任务入口类型

```c
typedef int (*main_t)(int argc, char *argv[]);
```

#### struct sched_param —— 调度参数

```c
struct sched_param {
    int sched_priority;
};
```

#### struct timespec —— 时间

```c
struct timespec {
    time_t tv_sec;   /* 秒 */
    long   tv_nsec;  /* 纳秒 */
};
```

#### struct mq_attr —— 消息队列属性

```c
struct mq_attr {
    size_t   mq_maxmsg;   /* 队列最大消息数 */
    size_t   mq_msgsize;  /* 最大消息大小 */
    unsigned mq_flags;    /* 队列标志 */
    size_t   mq_curmsgs;  /* 当前消息数 */
};
```

#### struct sigaction —— 信号处理动作

```c
struct sigaction {
    union {
        void (*_sa_handler)(int);
        void (*_sa_sigaction)(int, siginfo_t *, void *);
    } sa_u;
    sigset_t sa_mask;
    int      sa_flags;
};

#define sa_handler    sa_u._sa_handler
#define sa_sigaction  sa_u._sa_sigaction
```

#### struct siginfo / siginfo_t —— 信号信息

```c
typedef struct siginfo {
    int           si_signo;
    int           si_code;
    union sigval  si_value;
} siginfo_t;
```

#### union sigval —— 信号值

```c
union sigval {
    int   sival_int;
    void *sival_ptr;
};
```

#### struct sigevent —— 信号事件

```c
struct sigevent {
    int           sigev_signo;
    union sigval  sigev_value;
    int           sigev_notify;
};
```

---

## 17. 关键风险与注意事项汇总

### 17.1 高风险项

| 风险 | 影响 | 建议 |
|------|------|------|
| `task_delete()` 资源泄漏 | 被删除任务持有的内存/信号量等永久丢失 | 尽量用取消点机制；确保任务可自行退出 |
| `CONFIG_SCHED_CHILD_STATUS` 内存泄漏 | 不回收子状态导致内存耗尽 | 必须调用 `wait()`/`waitpid()` 或用 `SIG_IGN`+`SA_NOCLDWAIT` |
| `setlogmask()` 非线程安全 | 并发调用行为未定义 | 加锁保护或在初始化阶段设置 |
| 优先级继承边界条件 | 计数信号量多持有者场景复杂 | 评估是否需要 `CONFIG_SEM_PREALLOCHOLDERS` |
| `pthread_key_delete()` 实际无操作 | 线程特定数据清理不生效 | 不要依赖此接口做资源释放 |

### 17.2 中风险项

| 风险 | 说明 |
|------|------|
| 消息队列最大 22 字节硬编码 | 超过将失败，需检查 `mq_msgsize` |
| `sched_lock()` 阻止所有任务切换 | 影响系统响应时间，优先用信号量 |
| `sigaction` 的 `sa_flags` 全按 `SA_SIGINFO` 处理 | 其他标志被忽略 |
| 信号默认忽略（未启用 `CONFIG_SIG_DEFAULT` 时）| 异常终止信号不会自动终止任务 |
| `posix_spawn` 的 `envp` 被忽略 | 无法通过 `envp` 设置子任务环境 |
| `mq_notify` 仅一次通知 | 需重新注册才能再次接收通知 |
| `sem_timedwait` 使用绝对时间 | 使用前需正确设置系统时钟 |

### 17.3 注意事项

| 事项 | 说明 |
|------|------|
| `gmtime`/`localtime`/`asctime`/`ctime` 非可重入 | 多线程环境使用 `_r` 版本 |
| `errno` 每个 pthread 独立 | 不共享，可能非标准但促进线程独立 |
| `ipc_perm` 不支持用户/组 ID | 共享内存权限检查受限 |
| `boardctl()` 是非标准接口 | 绕过设备驱动层直接控制板级功能 |
| Watchdog 已移出用户 API | 需查内部头文件或架构 API |
| SMP 下需 Spinlock | 临界区保护机制与单核不同 |
| Task Group 行为依赖配置 | 不同配置下 `wait`/`SIGCHLD` 行为不同 |

---

## 18. POSIX 兼容性偏离汇总

| 编号 | 偏离项 | 涉及接口 |
|------|--------|----------|
| 1 | 信号默认忽略（非终止） | `signal` 全部 |
| 2 | `CONFIG_SIG_DEFAULT` 可选启用默认动作 | `SIGHUP`/`SIGINT`/`SIGKILL` 等 |
| 3 | 信号一次处理一个 | `sigaction` |
| 4 | 不支持进程组信号 | `kill` |
| 5 | `sa_flags` 全按 `SA_SIGINFO` 处理 | `sigaction` |
| 6 | 消息最大 22 字节硬编码 | `mq_send`/`mq_receive` |
| 7 | `envp` 被忽略 | `posix_spawn`/`exec` |
| 8 | `errno` 每个 pthread 独立 | `__errno()` |
| 9 | `ipc_perm` 无用户/组 ID | `shmget`/`shmctl` |
| 10 | `setlogmask` 非线程安全 | `setlogmask` |
| 11 | `openlog`/`closelog` 不支持 | 日志 |
| 12 | `pthread_key_delete` 无操作 | 线程特定数据 |
| 13 | 不支持 contention scope | `pthread_attr_setscope` |
| 14 | 不支持线程 CPU 时间时钟 | `pthread_getcpuclockid` |
| 15 | 不支持互斥锁优先级上限 | `pthread_mutex_setprioceiling` |
| 16 | `task_create` 非 POSIX | VxWorks 风格 |
| 17 | `task_delete` 非 POSIX | VxWorks 风格 |
| 18 | `task_restart` 非 POSIX | VxWorks 风格 |
| 19 | `boardctl` 非标准 | 绕过 ioctl |
| 20 | `waitpid` 行为依赖配置 | `CONFIG_SCHED_HAVE_PARENT` |
| 21 | 子任务状态依赖配置 | `CONFIG_SCHED_CHILD_STATUS` |
| 22 | 偶发调度非标准完整实现 | `SCHED_SPORADIC` |
| 23 | `sa_handler` 的 `SIG_DFL`/`SIG_IGN` 依赖配置 | `sigaction` |
| 24 | 轮转调度时间片为常量 | `CONFIG_RR_INTERVAL` |
| 25 | `fork` 仅 Kernel Build | `fork` |
| 26 | 优先级范围 0-255 | `sched_setparam` |
| 27 | `mq_notify` 仅一次 | `mq_notify` |
| 28 | `task_spawn` 非标准 | 基于 `posix_spawn` |
| 29 | `atexit`/`on_exit` 在任务组终止时执行 | 非进程级 |
| 30 | SMP 下临界区需 Spinlock | 不同于单核 |
| 31 | 不支持 `fork()` 在 FLAT/PROTECTED 模式 | 仅 KERNEL 模式 |

---

## 19. 配置宏速查表

### 19.1 调度相关

| 配置宏 | 作用 |
|--------|------|
| `CONFIG_RR_INTERVAL` | 轮转调度时间片（>0 启用） |
| `CONFIG_SCHED_SPORADIC` | 偶发调度策略 |
| `CONFIG_SCHED_HAVE_PARENT` | 记录父任务 ID |
| `CONFIG_SCHED_CHILD_STATUS` | 保留子任务退出状态 |
| `CONFIG_PREALLOC_CHILDSTATUS` | 预分配子状态结构数量 |
| `CONFIG_SMP` | 启用 SMP |
| `CONFIG_SMP_NCPUS` | CPU 核心数 |
| `CONFIG_SMP_IDLETHREAD_STACKSIZE` | SMP IDLE 栈大小 |

### 19.2 同步原语

| 配置宏 | 作用 |
|--------|------|
| `CONFIG_PRIORITY_INHERITANCE` | 优先级继承 |
| `CONFIG_SEM_PREALLOCHOLDERS` | 信号量持有者结构池大小 |
| `CONFIG_SEM_PREALLOCCONDS` | 信号量条件结构池大小 |

### 19.3 信号

| 配置宏 | 作用 |
|--------|------|
| `CONFIG_SIG_DEFAULT` | 启用信号默认动作 |
| `CONFIG_SIG_SIGKILL_ACTION` | SIGHUP/SIGINT/SIGKILL 等异常终止 |
| `CONFIG_SIG_SIGUSR1_ACTION` | SIGUSR1 异常终止 |
| `CONFIG_SIG_SIGUSR2_ACTION` | SIGUSR2 异常终止 |
| `CONFIG_SIG_SIGPIPE_ACTION` | SIGPIPE 异常终止 |
| `CONFIG_SIG_SIGALRM_ACTION` | SIGALRM 异常终止 |
| `CONFIG_SIG_SIGPROF_ACTION` | SIGPROF 异常终止 |
| `CONFIG_SIG_SIGPOLL_ACTION` | SIGPOLL 异常终止 |
| `CONFIG_SIG_SIGSTOP_ACTION` | SIGSTOP/SIGCONT 挂起/恢复 |
| `CONFIG_SIG_SIGURG_ACTION` | SIGURG 忽略 |

### 19.4 构建模式

| 配置宏 | 作用 |
|--------|------|
| `CONFIG_BUILD_FLAT` | FLAT 构建（单一地址空间） |
| `CONFIG_BUILD_PROTECTED` | PROTECTED 构建（内核/用户分离） |
| `CONFIG_BUILD_KERNEL` | KERNEL 构建（多进程，支持 `fork`/`shm`） |
| `CONFIG_DEV_CONSOLE` | 启用 stdin/stdout/stderr 复制 |

### 19.5 其他

| 配置宏 | 作用 |
|--------|------|
| `CONFIG_DISABLE_ENVIRON` | 禁用环境变量 |
| `CONFIG_PM` | 启用电源管理 |
| `CONFIG_BOARDCTL_POWEROFF` | boardctl 关机命令 |
| `CONFIG_BOARDCTL_RESET` | boardctl 重置命令 |
| `CONFIG_BOARDCTL_MKRD` | boardctl RAM 磁盘命令 |
| `CONFIG_BOARDCTL_ROMDISK` | boardctl ROM 磁盘命令 |
| `CONFIG_BOARDCTL_APP_SYMTAB` | boardctl 应用符号表 |
| `CONFIG_BOARDCTL_OS_SYMTAB` | boardctl OS 符号表 |
| `CONFIG_BOARDCTL_UNIQUEID_SIZE` | 唯一 ID 大小 |
| `CONFIG_BOARDCTL_MACADDR` | boardctl MAC 地址 |
| `CONFIG_NET` | 网络支持 |
| `CONFIG_NET_TCP` | TCP/IP 支持 |
| `CONFIG_NET_TCPBACKLOG` | TCP 连接 backlog |

---

> **文档结束**  
> 本文档基于 Apache NuttX 官方最新文档（https://nuttx.apache.org/docs/latest/）编写，覆盖全部 15 大类用户空间 API 接口。与 2009 版手册相比，新增了 3 个全新接口分组、50+ 个新接口，并引入了 Task Group、SMP、信号默认动作、偶发调度等重大架构增强。
