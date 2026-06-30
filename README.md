# Yorubench

Yorubench is a lightweight performance measurement library for **STM32 HAL**.

It is not meant to be a full test framework. Its job is to provide a small and direct measurement layer:

- bind a timing source
- wrap a code block with `Begin / End`
- calculate time, cycles, averages, and throughput
- output results through a user callback

---

> **Yoru Series**
>
> A family of lightweight utility libraries for STM32 HAL. Each library can be used independently or combined as needed.
>
> | Library | Role |
> | --- | --- |
> | [Yorulog](https://github.com/ExMikuPro/Yorulog) | Lightweight UART logger |
> | [Yorush](https://github.com/ExMikuPro/Yorush) | Lightweight UART shell / command parser |
> | [Yorunvm](https://github.com/ExMikuPro/Yorunvm) | STM32 on-chip NVM / Flash access helper |
> | [Yorukv](https://github.com/ExMikuPro/Yorukv) | Lightweight KV configuration library |
> | [Yorubench](https://github.com/ExMikuPro/Yorubench) | Lightweight performance measurement library |
> | [Yoruassert](https://github.com/ExMikuPro/Yoruassert.git) | Lightweight assertion helper |

---

## Current Scope

- single-header library
- usable with default configuration
- supports `soft now` callback
- supports `TIM` timing
- supports `DWT cycle`
- supports result struct output
- supports text writer callback
- can optionally reuse Yorulog as the internal output path

---

## Design Goals

- does not execute test functions automatically
- does not manage test cases
- does not hard-depend on Yorulog
- lets the user decide what to measure

---

## Minimal Example

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

### Using Yorulog as the Output Path

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

### Using an External Writer Callback

```c
#define YORUBENCH_USE_YORULOG 0
#include "yorubench.h"

YORUBENCH_Init(&hBench);
YORUBENCH_SetWriter(&hBench, my_writer, my_user);
```

---

## Current API

- `YORUBENCH_Init()`
- `YORUBENCH_SetWriter()`
- `YORUBENCH_UseSoftNow()`
- `YORUBENCH_UseTim()`
- `YORUBENCH_UseDwt()`
- `YORUBENCH_EnableDwtCycles()`
- `YORUBENCH_Begin()`
- `YORUBENCH_End()`
- `YORUBENCH_Report()`
