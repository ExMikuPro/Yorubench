# Yorubench

Yorubench 是一个面向 **STM32 HAL** 的轻量级性能测量库。

它的目标不是做完整测试框架，而是提供一层尽量小、尽量直接的测量能力：

- 绑定计时源
- `Begin / End` 包住一段代码
- 计算耗时、cycle、平均值和吞吐
- 通过用户回调输出结果

---

> **Yoru 系列**
>
> 一组面向 STM32 HAL 的轻量级工具库。各库可独立使用，也可以组合使用。
>
> | 库 | 定位 |
> | --- | --- |
> | [Yorulog](https://github.com/ExMikuPro/Yorulog) | 轻量级 UART 日志库 |
> | [Yorush](https://github.com/ExMikuPro/Yorush) | 轻量级 UART Shell / 命令解析器 |
> | [Yorunvm](https://github.com/ExMikuPro/Yorunvm) | STM32 片上 NVM / Flash 访问辅助库 |
> | [Yorukv](https://github.com/ExMikuPro/Yorukv) | 轻量级 KV 配置库 |
> | [Yorubench](https://github.com/ExMikuPro/Yorubench) | 轻量级性能测量库 |
> | [Yoruassert](https://github.com/ExMikuPro/Yoruassert.git) | 轻量级断言辅助库 |

---

## 当前能力

- 单头文件
- 默认配置可直接包含
- 支持 `soft now` 回调
- 支持 `TIM` 计时
- 支持 `DWT cycle`
- 支持结果结构体输出
- 支持文本输出回调
- 可选内部复用 Yorulog 作为输出路径

---

## 设计原则

- 不负责自动执行被测函数
- 不负责管理测试用例
- 不强依赖 Yorulog
- 用户自己决定测什么

---

## 最小示例

```c
#include "yorubench.h"

static YORUBENCH_HandleTypeDef hBench;
static YORUBENCH_ResultTypeDef result;

YORUBENCH_Init(&hBench);
YORUBENCH_UseTim(&hBench, &htim2, 1000000u);
YORUBENCH_EnableDwtCycles(&hBench, SystemCoreClock);

YORUBENCH_Begin(&hBench);
/* code under test */
YORUBENCH_End(&hBench, &result, "demo", 1u, 0u);
```

### 使用 Yorulog 作为输出

```c
#define YORUBENCH_USE_YORULOG 1
#include "yorubench.h"

YORUBENCH_Init(&hBench);
YORUBENCH_UseTim(&hBench, &htim2, 1000000u);
YORUBENCH_EnableDwtCycles(&hBench, SystemCoreClock);

YORUBENCH_Begin(&hBench);
/* code under test */
YORUBENCH_End(&hBench, &result, "demo", 1u, 0u);
YORUBENCH_Report(&hBench, &result);
```

### 使用外部输出回调

```c
#define YORUBENCH_USE_YORULOG 0
#include "yorubench.h"

YORUBENCH_Init(&hBench);
YORUBENCH_SetWriter(&hBench, my_writer, my_user);
```

---

## 当前 API

- `YORUBENCH_Init()`
- `YORUBENCH_SetWriter()`
- `YORUBENCH_UseSoftNow()`
- `YORUBENCH_UseTim()`
- `YORUBENCH_UseDwt()`
- `YORUBENCH_EnableDwtCycles()`
- `YORUBENCH_Begin()`
- `YORUBENCH_End()`
- `YORUBENCH_Report()`
