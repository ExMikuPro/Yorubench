#ifndef YORUBENCH_H
#define YORUBENCH_H

/* =========================================================
 *  Auto-detect and include STM32 HAL header
 * =========================================================
 * Manual override example:
 *   #define YORUBENCH_HAL_HEADER "stm32h7xx_hal.h"
 *   #include "yorubench.h"
 */
#ifndef YORUBENCH_HAL_HEADER
  #if defined(__has_include)
    #if __has_include("stm32h7xx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32h7xx_hal.h"
    #elif __has_include("stm32f7xx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32f7xx_hal.h"
    #elif __has_include("stm32f4xx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32f4xx_hal.h"
    #elif __has_include("stm32g4xx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32g4xx_hal.h"
    #elif __has_include("stm32g0xx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32g0xx_hal.h"
    #elif __has_include("stm32l4xx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32l4xx_hal.h"
    #elif __has_include("stm32l0xx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32l0xx_hal.h"
    #elif __has_include("stm32f1xx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32f1xx_hal.h"
    #elif __has_include("stm32c0xx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32c0xx_hal.h"
    #elif __has_include("stm32u5xx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32u5xx_hal.h"
    #elif __has_include("stm32wbxx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32wbxx_hal.h"
    #elif __has_include("stm32wlxx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32wlxx_hal.h"
    #elif __has_include("stm32xx_hal.h")
      #define YORUBENCH_HAL_HEADER "stm32xx_hal.h"
    #endif
  #endif
#endif

#ifdef YORUBENCH_HAL_HEADER
  #include YORUBENCH_HAL_HEADER
#else
  #error "yorubench.h: Please define YORUBENCH_HAL_HEADER before including yorubench.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =========================================================
 *  Minimal Integer Types (no stdint dependency)
 * ========================================================= */

typedef
#if defined(__UINT8_TYPE__)
__UINT8_TYPE__
#else
unsigned char
#endif
yorubench_u8_t;

typedef
#if defined(__UINT32_TYPE__)
__UINT32_TYPE__
#else
unsigned int
#endif
yorubench_u32_t;

typedef
#if defined(__UINT64_TYPE__)
__UINT64_TYPE__
#else
unsigned long long
#endif
yorubench_u64_t;

/* =========================================================
 *  User Configuration
 * ========================================================= */

#ifndef YORUBENCH_ENABLE
#define YORUBENCH_ENABLE 1
#endif

#ifndef YORUBENCH_ENABLE_SINGLE
#define YORUBENCH_ENABLE_SINGLE 1
#endif

#ifndef YORUBENCH_ENABLE_BATCH
#define YORUBENCH_ENABLE_BATCH 1
#endif

#ifndef YORUBENCH_ENABLE_SOFT_NOW
#define YORUBENCH_ENABLE_SOFT_NOW 1
#endif

#ifndef YORUBENCH_ENABLE_TIM
#define YORUBENCH_ENABLE_TIM 1
#endif

#ifndef YORUBENCH_ENABLE_DWT
#define YORUBENCH_ENABLE_DWT 1
#endif

#ifndef YORUBENCH_USE_YORULOG
#define YORUBENCH_USE_YORULOG 0
#endif

#if YORUBENCH_USE_YORULOG
  #ifndef YORUBENCH_YORULOG_HEADER
    #define YORUBENCH_YORULOG_HEADER "yorulog.h"
  #endif
  #include YORUBENCH_YORULOG_HEADER
#endif

#ifndef YORUBENCH_REPORT_SHOW_FREQ
#define YORUBENCH_REPORT_SHOW_FREQ 1
#endif

#ifndef YORUBENCH_LOCK
#define YORUBENCH_LOCK()   do{}while(0)
#endif

#ifndef YORUBENCH_UNLOCK
#define YORUBENCH_UNLOCK() do{}while(0)
#endif

/* =========================================================
 *  Type Definitions
 * ========================================================= */

typedef enum
{
    YORUBENCH_OK = 0,
    YORUBENCH_ERROR = 1,
    YORUBENCH_INVALID_PARAM = 2,
    YORUBENCH_NOT_READY = 3,
    YORUBENCH_BUSY = 4
} YORUBENCH_StatusTypeDef;

typedef enum
{
    YORUBENCH_SOURCE_NONE = 0,
    YORUBENCH_SOURCE_SOFT_NOW = 1,
    YORUBENCH_SOURCE_TIM = 2,
    YORUBENCH_SOURCE_DWT = 3
} YORUBENCH_SourceTypeDef;

typedef yorubench_u32_t (*YORUBENCH_SoftNowFunc)(void);
typedef void (*YORUBENCH_WriteStrFunc)(void *user, const char *text);

typedef struct
{
    const char *Name;
    yorubench_u32_t Count;
    yorubench_u32_t Bytes;
    yorubench_u32_t Ticks;
    yorubench_u32_t TickHz;
    yorubench_u32_t CpuHz;
    yorubench_u32_t TotalUs;
    yorubench_u32_t TotalCycles;
    yorubench_u32_t PerCallUs;
    yorubench_u32_t PerCallCycles;
    yorubench_u32_t PerByteCycles;
    yorubench_u32_t ThroughputBps;
    yorubench_u8_t Source;
    yorubench_u8_t HasCycles;
} YORUBENCH_ResultTypeDef;

typedef struct
{
    yorubench_u8_t Source;
    yorubench_u8_t Active;
    yorubench_u8_t DwtEnabled;
    yorubench_u32_t TickHz;
    yorubench_u32_t DwtHz;
    yorubench_u32_t StartTick;
    yorubench_u32_t StartCycle;
    YORUBENCH_SoftNowFunc SoftNow;
    TIM_HandleTypeDef *Htim;
    YORUBENCH_WriteStrFunc Writer;
    void *WriterUser;
} YORUBENCH_HandleTypeDef;

/* =========================================================
 *  Internal Helpers
 * ========================================================= */

static inline void yorubench__reset_result_(YORUBENCH_ResultTypeDef *result)
{
    if (!result) return;
    result->Name = (const char *)0;
    result->Count = 0u;
    result->Bytes = 0u;
    result->Ticks = 0u;
    result->TickHz = 0u;
    result->CpuHz = 0u;
    result->TotalUs = 0u;
    result->TotalCycles = 0u;
    result->PerCallUs = 0u;
    result->PerCallCycles = 0u;
    result->PerByteCycles = 0u;
    result->ThroughputBps = 0u;
    result->Source = (yorubench_u8_t)YORUBENCH_SOURCE_NONE;
    result->HasCycles = 0u;
}

static inline yorubench_u32_t yorubench__dwt_now_(void)
{
#if YORUBENCH_ENABLE_DWT && defined(DWT)
    return (yorubench_u32_t)DWT->CYCCNT;
#else
    return 0u;
#endif
}

static inline yorubench_u8_t yorubench__enable_dwt_(void)
{
#if YORUBENCH_ENABLE_DWT && defined(DWT) && defined(CoreDebug) && defined(DWT_CTRL_CYCCNTENA_Msk) && defined(CoreDebug_DEMCR_TRCENA_Msk)
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0u;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    return (yorubench_u8_t)((DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) != 0u);
#else
    return 0u;
#endif
}

static inline yorubench_u32_t yorubench__read_tick_(const YORUBENCH_HandleTypeDef *h)
{
    if (!h) return 0u;

    if (h->Source == (yorubench_u8_t)YORUBENCH_SOURCE_SOFT_NOW) {
        return h->SoftNow ? h->SoftNow() : 0u;
    }

    if (h->Source == (yorubench_u8_t)YORUBENCH_SOURCE_TIM) {
#if YORUBENCH_ENABLE_TIM
        return h->Htim ? (yorubench_u32_t)__HAL_TIM_GET_COUNTER(h->Htim) : 0u;
#else
        return 0u;
#endif
    }

    if (h->Source == (yorubench_u8_t)YORUBENCH_SOURCE_DWT) {
        return yorubench__dwt_now_();
    }

    return 0u;
}

static inline void yorubench__write_cstr_(const YORUBENCH_HandleTypeDef *h, const char *s)
{
#if YORUBENCH_USE_YORULOG
    (void)h;
    if (!s) return;
    YORULOG_PrintRaw(s);
#else
    if (!h || !h->Writer || !s) return;
    h->Writer(h->WriterUser, s);
#endif
}

static inline void yorubench__write_u32_(const YORUBENCH_HandleTypeDef *h, yorubench_u32_t value)
{
    char buf[10];
    unsigned i = 0u;

#if YORUBENCH_USE_YORULOG
    (void)h;
#else
    if (!h || !h->Writer) return;
#endif

    if (value == 0u) {
#if YORUBENCH_USE_YORULOG
        YORULOG_PrintRaw("0");
#else
        h->Writer(h->WriterUser, "0");
#endif
        return;
    }

    while ((value != 0u) && (i < (unsigned)sizeof(buf))) {
        buf[i++] = (char)('0' + (value % 10u));
        value /= 10u;
    }

    while (i != 0u) {
        char out[2];
        out[0] = buf[--i];
        out[1] = '\0';
#if YORUBENCH_USE_YORULOG
        YORULOG_PrintRaw(out);
#else
        h->Writer(h->WriterUser, out);
#endif
    }
}

/* =========================================================
 *  Public API
 * ========================================================= */

static inline void YORUBENCH_Init(YORUBENCH_HandleTypeDef *h)
{
    if (!h) return;
    h->Source = (yorubench_u8_t)YORUBENCH_SOURCE_NONE;
    h->Active = 0u;
    h->DwtEnabled = 0u;
    h->TickHz = 0u;
    h->DwtHz = 0u;
    h->StartTick = 0u;
    h->StartCycle = 0u;
    h->SoftNow = (YORUBENCH_SoftNowFunc)0;
    h->Htim = (TIM_HandleTypeDef *)0;
    h->Writer = (YORUBENCH_WriteStrFunc)0;
    h->WriterUser = (void *)0;
}

#if !YORUBENCH_USE_YORULOG
static inline void YORUBENCH_SetWriter(YORUBENCH_HandleTypeDef *h, YORUBENCH_WriteStrFunc writer, void *user)
{
    if (!h) return;
    h->Writer = writer;
    h->WriterUser = user;
}
#endif

static inline YORUBENCH_StatusTypeDef YORUBENCH_UseSoftNow(YORUBENCH_HandleTypeDef *h, YORUBENCH_SoftNowFunc now_func, yorubench_u32_t tick_hz)
{
#if !YORUBENCH_ENABLE || !YORUBENCH_ENABLE_SOFT_NOW
    (void)h;
    (void)now_func;
    (void)tick_hz;
    return YORUBENCH_NOT_READY;
#else
    if (!h || !now_func || (tick_hz == 0u)) return YORUBENCH_INVALID_PARAM;
    h->Source = (yorubench_u8_t)YORUBENCH_SOURCE_SOFT_NOW;
    h->SoftNow = now_func;
    h->Htim = (TIM_HandleTypeDef *)0;
    h->TickHz = tick_hz;
    return YORUBENCH_OK;
#endif
}

static inline YORUBENCH_StatusTypeDef YORUBENCH_UseTim(YORUBENCH_HandleTypeDef *h, TIM_HandleTypeDef *htim, yorubench_u32_t tick_hz)
{
#if !YORUBENCH_ENABLE || !YORUBENCH_ENABLE_TIM
    (void)h;
    (void)htim;
    (void)tick_hz;
    return YORUBENCH_NOT_READY;
#else
    if (!h || !htim || (tick_hz == 0u)) return YORUBENCH_INVALID_PARAM;
    h->Source = (yorubench_u8_t)YORUBENCH_SOURCE_TIM;
    h->Htim = htim;
    h->SoftNow = (YORUBENCH_SoftNowFunc)0;
    h->TickHz = tick_hz;
    return YORUBENCH_OK;
#endif
}

static inline YORUBENCH_StatusTypeDef YORUBENCH_UseDwt(YORUBENCH_HandleTypeDef *h, yorubench_u32_t cpu_hz)
{
#if !YORUBENCH_ENABLE || !YORUBENCH_ENABLE_DWT
    (void)h;
    (void)cpu_hz;
    return YORUBENCH_NOT_READY;
#else
    if (!h || (cpu_hz == 0u)) return YORUBENCH_INVALID_PARAM;
    if (!yorubench__enable_dwt_()) return YORUBENCH_ERROR;
    h->Source = (yorubench_u8_t)YORUBENCH_SOURCE_DWT;
    h->TickHz = cpu_hz;
    h->DwtEnabled = 1u;
    h->DwtHz = cpu_hz;
    return YORUBENCH_OK;
#endif
}

static inline YORUBENCH_StatusTypeDef YORUBENCH_EnableDwtCycles(YORUBENCH_HandleTypeDef *h, yorubench_u32_t cpu_hz)
{
#if !YORUBENCH_ENABLE || !YORUBENCH_ENABLE_DWT
    (void)h;
    (void)cpu_hz;
    return YORUBENCH_NOT_READY;
#else
    if (!h || (cpu_hz == 0u)) return YORUBENCH_INVALID_PARAM;
    if (!yorubench__enable_dwt_()) return YORUBENCH_ERROR;
    h->DwtEnabled = 1u;
    h->DwtHz = cpu_hz;
    return YORUBENCH_OK;
#endif
}

static inline yorubench_u8_t YORUBENCH_IsReady(const YORUBENCH_HandleTypeDef *h)
{
    if (!h) return 0u;
    return (yorubench_u8_t)(h->Source != (yorubench_u8_t)YORUBENCH_SOURCE_NONE);
}

static inline YORUBENCH_StatusTypeDef YORUBENCH_Begin(YORUBENCH_HandleTypeDef *h)
{
#if !YORUBENCH_ENABLE
    (void)h;
    return YORUBENCH_NOT_READY;
#else
    if (!h) return YORUBENCH_INVALID_PARAM;
    if (!YORUBENCH_IsReady(h)) return YORUBENCH_NOT_READY;
    if (h->Active) return YORUBENCH_BUSY;

    YORUBENCH_LOCK();
    h->StartTick = yorubench__read_tick_(h);
    h->StartCycle = h->DwtEnabled ? yorubench__dwt_now_() : 0u;
    h->Active = 1u;
    YORUBENCH_UNLOCK();
    return YORUBENCH_OK;
#endif
}

static inline YORUBENCH_StatusTypeDef YORUBENCH_End(YORUBENCH_HandleTypeDef *h,
                                                    YORUBENCH_ResultTypeDef *result,
                                                    const char *name,
                                                    yorubench_u32_t count,
                                                    yorubench_u32_t bytes)
{
#if !YORUBENCH_ENABLE
    (void)h;
    (void)result;
    (void)name;
    (void)count;
    (void)bytes;
    return YORUBENCH_NOT_READY;
#else
    yorubench_u32_t end_tick;
    yorubench_u32_t end_cycle;
    yorubench_u32_t delta_tick;
    yorubench_u32_t delta_cycle;
    yorubench_u64_t temp;

    if (!h || !result) return YORUBENCH_INVALID_PARAM;
    if (!h->Active) return YORUBENCH_NOT_READY;

    end_tick = yorubench__read_tick_(h);
    end_cycle = h->DwtEnabled ? yorubench__dwt_now_() : 0u;
    delta_tick = (yorubench_u32_t)(end_tick - h->StartTick);
    delta_cycle = h->DwtEnabled ? (yorubench_u32_t)(end_cycle - h->StartCycle) : 0u;
    h->Active = 0u;

    yorubench__reset_result_(result);
    result->Name = name;
    result->Count = count;
    result->Bytes = bytes;
    result->Ticks = delta_tick;
    result->TickHz = h->TickHz;
    result->CpuHz = h->DwtHz;
    result->Source = h->Source;
    result->HasCycles = h->DwtEnabled ? 1u : 0u;
    result->TotalCycles = delta_cycle;

    if (h->TickHz != 0u) {
        temp = ((yorubench_u64_t)delta_tick * 1000000ull) / (yorubench_u64_t)h->TickHz;
        result->TotalUs = (yorubench_u32_t)temp;
    }

    if ((result->TotalUs == 0u) && (h->Source == (yorubench_u8_t)YORUBENCH_SOURCE_DWT) && (h->TickHz != 0u)) {
        temp = ((yorubench_u64_t)delta_tick * 1000000ull) / (yorubench_u64_t)h->TickHz;
        result->TotalUs = (yorubench_u32_t)temp;
    }

    if (count != 0u) {
        result->PerCallUs = result->TotalUs / count;
        if (h->DwtEnabled) result->PerCallCycles = delta_cycle / count;
    }

    if (bytes != 0u) {
        if ((result->TotalUs != 0u) && (bytes != 0u)) {
            temp = ((yorubench_u64_t)bytes * 1000000ull) / (yorubench_u64_t)result->TotalUs;
            result->ThroughputBps = (yorubench_u32_t)temp;
        }
        if (h->DwtEnabled) result->PerByteCycles = delta_cycle / bytes;
    }

    return YORUBENCH_OK;
#endif
}

static inline YORUBENCH_StatusTypeDef YORUBENCH_Report(const YORUBENCH_HandleTypeDef *h, const YORUBENCH_ResultTypeDef *result)
{
#if !YORUBENCH_ENABLE
    (void)h;
    (void)result;
    return YORUBENCH_NOT_READY;
#else
    if (!h || !result) return YORUBENCH_INVALID_PARAM;
#if !YORUBENCH_USE_YORULOG
    if (!h->Writer) return YORUBENCH_INVALID_PARAM;
#endif

    yorubench__write_cstr_(h, result->Name ? result->Name : "bench");
    yorubench__write_cstr_(h, ": count=");
    yorubench__write_u32_(h, result->Count);
    yorubench__write_cstr_(h, ", bytes=");
    yorubench__write_u32_(h, result->Bytes);
    yorubench__write_cstr_(h, ", ticks=");
    yorubench__write_u32_(h, result->Ticks);
#if YORUBENCH_REPORT_SHOW_FREQ
    if (result->TickHz != 0u) {
        yorubench__write_cstr_(h, ", tick_hz=");
        yorubench__write_u32_(h, result->TickHz);
    }
#endif
    yorubench__write_cstr_(h, ", total_us=");
    yorubench__write_u32_(h, result->TotalUs);

    if (result->Count != 0u) {
        yorubench__write_cstr_(h, ", per_call_us=");
        yorubench__write_u32_(h, result->PerCallUs);
    }

    if (result->Bytes != 0u) {
        yorubench__write_cstr_(h, ", throughput_Bps=");
        yorubench__write_u32_(h, result->ThroughputBps);
    }

    yorubench__write_cstr_(h, "\r\n");

    if (result->HasCycles) {
        yorubench__write_cstr_(h, result->Name ? result->Name : "bench");
        yorubench__write_cstr_(h, " cycles: total=");
        yorubench__write_u32_(h, result->TotalCycles);
#if YORUBENCH_REPORT_SHOW_FREQ
        if (result->CpuHz != 0u) {
            yorubench__write_cstr_(h, ", cpu_hz=");
            yorubench__write_u32_(h, result->CpuHz);
        }
#endif

        if (result->Count != 0u) {
            yorubench__write_cstr_(h, ", per_call=");
            yorubench__write_u32_(h, result->PerCallCycles);
        }

        if (result->Bytes != 0u) {
            yorubench__write_cstr_(h, ", per_byte=");
            yorubench__write_u32_(h, result->PerByteCycles);
        }

        yorubench__write_cstr_(h, "\r\n");
    }

    return YORUBENCH_OK;
#endif
}

#ifdef __cplusplus
}
#endif

#endif /* YORUBENCH_H */
