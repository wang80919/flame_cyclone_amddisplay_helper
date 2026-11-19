#pragma once

#include <Windows.h>
#include <powrprof.h>

// https://learn.microsoft.com/zh-cn/windows/win32/api/powrprof/?source=recommendations
// https://learn.microsoft.com/zh-cn/windows-hardware/customize/power-settings/configure-power-settings

//节能
//DEFINE_GUID(GUID_MAX_POWER_SAVINGS, 0xA1841308, 0x3541, 0x4FAB, 0xBC, 0x81, 0xF7, 0x15, 0x56, 0xF2, 0x0B, 0x4A);
// 
//高性能
//DEFINE_GUID(GUID_MIN_POWER_SAVINGS, 0x8C5E7FDA, 0xE8BF, 0x4A96, 0x9A, 0x85, 0xA6, 0xE2, 0x3A, 0x8C, 0x63, 0x5C);
// 
//平衡
//DEFINE_GUID(GUID_TYPICAL_POWER_SAVINGS, 0x381B4222, 0xF694, 0x41F0, 0x96, 0x85, 0xFF, 0x5B, 0xB2, 0x60, 0xDF, 0x2E);
// 
//所有电源计划
//DEFINE_GUID(ALL_POWERSCHEMES_GUID, 0x68A1E95E, 0x13EA, 0x41E1, 0x80, 0x11, 0x0C, 0x49, 0x6C, 0xA4, 0x90, 0xB0);

namespace CPowerCfgUtils
{
    // @brief: 获取交流电设置的默认值
    // @param: SubGroup     电源设置的子组
    // @param: Setting      电源设置的标识符
    // @param: OutValue     输出缓冲
    // @ret: DWORD          操作结果
    DWORD GetACDefaultValue(LPCGUID Scheme, LPCGUID SubGroup, LPCGUID Setting, LPDWORD Value);

    // @brief: 获取直流电设置的默认值
    // @param: SubGroup     电源设置的子组
    // @param: Setting      电源设置的标识符
    // @param: OutValue     输出缓冲
    // @ret: DWORD          操作结果
    DWORD GetDCDefaultValue(LPCGUID Scheme, LPCGUID SubGroup, LPCGUID Setting, LPDWORD Value);

    // @brief: 获取电源设置的最小值
    // @param: SubGroup     电源设置的子组
    // @param: Setting      电源设置的标识符
    // @param: OutValue     输出缓冲
    // @ret: DWORD          操作结果
    DWORD GeValueMin(LPCGUID SubGroup, LPCGUID Setting, LPDWORD OutValue);

    // @brief: 获取电源设置的最大值
    // @param: SubGroup     电源设置的子组
    // @param: Setting      电源设置的标识符
    // @param: OutValue     输出缓冲
    // @ret: DWORD          操作结果
    DWORD GeValueMax(LPCGUID SubGroup, LPCGUID Setting, LPDWORD OutValue);

    // @brief: 获取电源设置的最大值
    // @param: SubGroup     电源设置的子组
    // @param: Setting      电源设置的标识符
    // @ret: DWORD          操作结果
    DWORD GetAttributes(LPCGUID SubGroup, LPCGUID Setting);

    // @brief: 设置电源键的电源属性
    // @param: SubGroup     电源设置的子组
    // @param: Setting      电源设置的标识符
    // @param: Attributes   属性( POWER_ATTRIBUTE_HIDE: 隐藏 POWER_ATTRIBUTE_SHOW_AOAC: 显示)
    // @ret: DWORD          操作结果
    DWORD SetAttributes(LPCGUID SubGroup, LPCGUID Setting, DWORD Attributes);

    // @brief: 设置当前活动电源计划
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @ret: bool           操作结果
    DWORD SetActiveScheme(LPCGUID Scheme = nullptr);

    // @brief: 关闭显示器超时时间
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        超时时间(秒)
    // @ret: bool           操作结果
    DWORD SetVideoPowerdownTimeout(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 显示器亮度
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        亮度(0 - 100)
    // @ret: bool           操作结果
    DWORD SetVideoBrightness(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 指定在交互式控制台锁定超时时间
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        超时时间(秒)
    // @ret: bool           操作结果
    DWORD SetVideoConsoleLockTimeout(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 启用自适应亮度
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        状态(0: 关闭 1: 启用)
    // @ret: bool           操作结果
    DWORD SetVideoAdaptiveBrightness(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 显示高级颜色功能的视觉质量(当我的电脑使用电池运行时关闭HDR)
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        操作 (0: 电池使用时间 1: 视觉质量)
    // @ret: bool           操作结果
    DWORD SetVideoAdvancedColorQualityBias(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 无人值守唤醒后重新进入睡眠状态的等待时间（以秒为单位）。
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        超时时间(秒)
    // @ret: bool           操作结果
    DWORD SetUnattendSleepTimeout(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 指定系统被认为处于“空闲”状态后，等待多长时间（以秒为单位）才进入休眠状态（S4）
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        超时时间(秒)
    // @ret: bool           操作结果
    DWORD SetSleepHibernateTimeout(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 使计算机进入睡眠状态
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        超时时间(秒)
    // @ret: bool           操作结果
    DWORD SetSleepStandbyTimeout(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 生效的异类策略
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        异类策略(0 - 4)
    // @ret: DWORD          操作结果
    DWORD SetProcessorHeterogeneousPolicy(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 异类线程调度策列
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        0: 所有处理器, 1: 高性能处理器, 2: 首选高性能处理器
    //                      3: 高效处理器, 4: 首选高效处理器, 5: 自动
    // @ret: DWORD          操作结果
    DWORD SetProcessorThreadSchedulingPolicy(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 异类短运行线程调度策列
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        0: 所有处理器, 1: 高性能处理器, 2: 首选高性能处理器
    //                      3: 高效处理器, 4: 首选高效处理器, 5: 自动
    // @ret: DWORD          操作结果
    DWORD SetProcessorShortThreadSchedulingPolicy(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 处理器最大频率
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        处理器频率(MHz)
    // @ret: DWORD          操作结果
    DWORD SetProcessorFrequencyLimit(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 第 1 类处理器电源效率的处理器最大频率
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        处理器频率(MHz)
    // @ret: DWORD          操作结果
    DWORD SetProcessorFrequencyLimit1(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 最小处理器状态
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        百分比(0 - 100)
    // @ret: DWORD          操作结果
    DWORD SetProcessorThrottleMin(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 第 1 类处理器电源效率的最小处理器状态
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        百分比 (0 - 100)
    // @ret: DWORD          操作结果
    DWORD SetProcessorThrottleMin1(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 最大处理器状态
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        百分比(0 - 100)
    // @ret: DWORD          操作结果
    DWORD SetProcessorThrottleMax(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 第 1 类处理器电源效率的最大处理器状态
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        百分比(0 - 100)
    // @ret: DWORD          操作结果
    DWORD SetProcessorThrottleMax1(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 允许节流状态
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        节流状态(0: 关闭 1: 启动 2: 自动)
    // @ret: DWORD          操作结果
    DWORD SetProcessorAllowThrottling(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 处理器性能增强策略
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        百分比(0 - 100)
    // @ret: DWORD          操作结果
    DWORD SetProcessorPerfBoostPolicy(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 处理器性能提升模式
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        0: 已禁用, 1: 已启用, 2: 高性能, 3: 高效率
    //                      4: 高性能高效率, 5: 积极且有保障, 6: 高效, 积极且有保障
    // @ret: DWORD          操作结果
    DWORD SetProcessorPerfBoostMode(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 按电源按钮时操作
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        操作 (0: 无 1: 睡眠 2: 休眠 3: 关机 4: 关闭显示器)
    // @ret: bool           操作结果
    DWORD SetPowerButtonAction(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 按睡眠按钮时操作
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        操作 (0: 无 1: 睡眠 2: 休眠 3: 关机 4: 关闭显示器)
    // @ret: bool           操作结果
    DWORD SetSleepButtonAction(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

    // @brief: 关闭盖子时操作
    // @param: Scheme       指定修改的电源计划, 为空则指定当前活动的电源计划
    // @param: value        操作 (0: 无 1: 睡眠 2: 休眠 3: 关机 4: 关闭显示器)
    // @ret: bool           操作结果
    DWORD SetLidCloseAction(LPCGUID Scheme, DWORD ACValue, DWORD DCValue);

};