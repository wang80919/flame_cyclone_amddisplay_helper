#include "CPowerCfgUtils.h"
#pragma comment(lib, "powrprof.lib")

namespace CPowerCfgUtils
{
    DWORD GeValueMin(LPCGUID SubGroup, LPCGUID Setting, LPDWORD OutValue)
    {
        return ::PowerReadValueMin(NULL, SubGroup, Setting, OutValue);
    }

    DWORD GeValueMax(LPCGUID SubGroup, LPCGUID Setting, LPDWORD OutValue)
    {
        return ::PowerReadValueMax(NULL, SubGroup, Setting, OutValue);
    }

    DWORD GetACDefaultValue(LPCGUID Scheme, LPCGUID SubGroup, LPCGUID Setting, LPDWORD Value)
    {
        LPGUID lpActiveScheme = NULL;
        LPCGUID lpScheme = Scheme;
        DWORD dwResult = ERROR_SUCCESS;

        do
        {
            if (nullptr == Scheme)
            {
                //检索当前活动的电源方案
                dwResult = ::PowerGetActiveScheme(NULL, &lpActiveScheme);
                if (ERROR_SUCCESS != dwResult)
                {
                    break;
                }

                lpScheme = lpActiveScheme;
            }

            //读取交流电默认值 (接通电源)
            dwResult = ::PowerReadACDefaultIndex(NULL, lpScheme, SubGroup, Setting, Value);
            if (ERROR_SUCCESS != dwResult)
            {
                break;
            }

        } while (FALSE);

        // 释放内存
        if (lpScheme)
        {
            ::LocalFree(lpActiveScheme);
        }

        return dwResult;
    }

    DWORD GetDCDefaultValue(LPCGUID Scheme, LPCGUID SubGroup, LPCGUID Setting, LPDWORD Value)
    {
        LPGUID lpActiveScheme = NULL;
        LPCGUID lpScheme = Scheme;
        DWORD dwResult = ERROR_SUCCESS;

        do
        {
            if (nullptr == Scheme)
            {
                //检索当前活动的电源方案
                dwResult = ::PowerGetActiveScheme(NULL, &lpActiveScheme);
                if (ERROR_SUCCESS != dwResult)
                {
                    break;
                }

                lpScheme = lpActiveScheme;
            }

            //读取直流电默认值 (使用电池)
            dwResult = ::PowerReadDCDefaultIndex(NULL, lpScheme, SubGroup, Setting, Value);
            if (ERROR_SUCCESS != dwResult)
            {
                break;
            }

        } while (FALSE);

        // 释放内存
        if (lpScheme)
        {
            ::LocalFree(lpActiveScheme);
        }

        return dwResult;
    }

    DWORD GetACValue(LPCGUID Scheme, LPCGUID SubGroup, LPCGUID Setting, LPDWORD Value)
    {
        LPGUID lpActiveScheme = NULL;
        LPCGUID lpScheme = Scheme;
        DWORD dwResult = ERROR_SUCCESS;

        do
        {
            if (nullptr == Scheme)
            {
                //检索当前活动的电源方案
                dwResult = ::PowerGetActiveScheme(NULL, &lpActiveScheme);
                if (ERROR_SUCCESS != dwResult)
                {
                    break;
                }

                lpScheme = lpActiveScheme;
            }

            //读取交流电状态 (接通电源)
            dwResult = ::PowerReadACValueIndex(NULL, lpScheme, SubGroup, Setting, Value);
            if (ERROR_SUCCESS != dwResult)
            {
                break;
            }

        } while (FALSE);

        // 释放内存
        if (lpScheme)
        {
            ::LocalFree(lpActiveScheme);
        }

        return dwResult;
    }

    DWORD GetDCValue(LPCGUID Scheme, LPCGUID SubGroup, LPCGUID Setting, LPDWORD Value)
    {
        LPGUID lpActiveScheme = NULL;
        LPCGUID lpScheme = Scheme;
        DWORD dwResult = ERROR_SUCCESS;

        do
        {
            if (nullptr == Scheme)
            {
                //检索当前活动的电源方案
                dwResult = ::PowerGetActiveScheme(NULL, &lpActiveScheme);
                if (ERROR_SUCCESS != dwResult)
                {
                    break;
                }

                lpScheme = lpActiveScheme;
            }

            //读取直流电状态 (使用电池)
            dwResult = ::PowerReadDCValueIndex(NULL, lpScheme, SubGroup, Setting, Value);
            if (ERROR_SUCCESS != dwResult)
            {
                break;
            }

        } while (FALSE);

        // 释放内存
        if (lpScheme)
        {
            ::LocalFree(lpActiveScheme);
        }

        return dwResult;
    }

    DWORD GetValue(LPCGUID Scheme, LPCGUID SubGroup, LPCGUID Setting, LPDWORD AcValue, LPDWORD DcValue)
    {
        LPGUID lpActiveScheme = NULL;
        LPCGUID lpScheme = Scheme;
        DWORD dwResult = ERROR_SUCCESS;

        do
        {
            if (nullptr == Scheme)
            {
                //检索当前活动的电源方案
                dwResult = ::PowerGetActiveScheme(NULL, &lpActiveScheme);
                if (ERROR_SUCCESS != dwResult)
                {
                    break;
                }

                lpScheme = lpActiveScheme;
            }

            //读取交流电状态 (接通电源)
            dwResult = ::PowerReadACValueIndex(NULL, lpScheme, SubGroup, Setting, AcValue);
            if (ERROR_SUCCESS != dwResult)
            {
                break;
            }

            //读取直流电状态 (使用电池)
            dwResult = ::PowerReadDCValueIndex(NULL, lpScheme, SubGroup, Setting, DcValue);
            if (ERROR_SUCCESS != dwResult)
            {
                break;
            }

        } while (FALSE);

        // 释放内存
        if (lpScheme)
        {
            ::LocalFree(lpActiveScheme);
        }

        return dwResult;
    }

    DWORD SetACValue(LPCGUID Scheme, LPCGUID SubGroup, LPCGUID Setting, DWORD AcValue)
    {
        LPGUID lpActiveScheme = NULL;
        LPCGUID lpScheme = Scheme;
        DWORD dwResult = ERROR_SUCCESS;

        do
        {
            if (nullptr == Scheme)
            {
                //检索当前活动的电源方案
                dwResult = ::PowerGetActiveScheme(NULL, &lpActiveScheme);
                if (ERROR_SUCCESS != dwResult)
                {
                    break;
                }

                lpScheme = lpActiveScheme;
            }

            //交流电状态 (接通电源)
            dwResult = ::PowerWriteACValueIndex(NULL, lpScheme, SubGroup, Setting, AcValue);
            if (ERROR_SUCCESS != dwResult)
            {
                break;
            }

        } while (FALSE);

        // 释放内存
        if (lpScheme)
        {
            ::LocalFree(lpActiveScheme);
        }

        return dwResult;
    }

    DWORD SetDCValue(LPCGUID Scheme, LPCGUID SubGroup, LPCGUID Setting, DWORD DcValue)
    {
        LPGUID lpActiveScheme = NULL;
        LPCGUID lpScheme = Scheme;
        DWORD dwResult = ERROR_SUCCESS;

        do
        {
            if (nullptr == Scheme)
            {
                //检索当前活动的电源方案
                dwResult = ::PowerGetActiveScheme(NULL, &lpActiveScheme);
                if (ERROR_SUCCESS != dwResult)
                {
                    break;
                }

                lpScheme = lpActiveScheme;
            }

            //直流电状态 (使用电池)
            dwResult = ::PowerWriteACValueIndex(NULL, lpScheme, SubGroup, Setting, DcValue);
            if (ERROR_SUCCESS != dwResult)
            {
                break;
            }

        } while (FALSE);

        // 释放内存
        if (lpScheme)
        {
            ::LocalFree(lpActiveScheme);
        }

        return dwResult;
    }

    DWORD SetValue(LPCGUID Scheme, LPCGUID SubGroup, LPCGUID Setting, DWORD AcValue, DWORD DcValue)
    {
        LPGUID lpActiveScheme = NULL;
        LPCGUID lpScheme = Scheme;
        DWORD dwResult = ERROR_SUCCESS;

        do
        {
            if (nullptr == Scheme)
            {
                //检索当前活动的电源方案
                dwResult = ::PowerGetActiveScheme(NULL, &lpActiveScheme);
                if (ERROR_SUCCESS != dwResult)
                {
                    break;
                }

                lpScheme = lpActiveScheme;
            }

            //交流电状态 (接通电源)
            dwResult = ::PowerWriteACValueIndex(NULL, lpScheme, SubGroup, Setting, AcValue);
            if (ERROR_SUCCESS != dwResult)
            {
                break;
            }

            //直流电状态 (使用电池)
            dwResult = ::PowerWriteDCValueIndex(NULL, lpScheme, SubGroup, Setting, DcValue);
            if (ERROR_SUCCESS != dwResult)
            {
                break;
            }

        } while (FALSE);

        // 释放内存
        if (lpScheme)
        {
            ::LocalFree(lpActiveScheme);
        }

        return dwResult;
    }

    DWORD GetAttributes(LPCGUID SubGroup, LPCGUID Setting)
    {
        return ::PowerReadSettingAttributes(SubGroup, Setting);
    }

    DWORD SetAttributes(LPCGUID SubGroup, LPCGUID Setting, DWORD Attributes)
    {
        return ::PowerWriteSettingAttributes(SubGroup, Setting, Attributes);
    }

    DWORD SetActiveScheme(LPCGUID Scheme/* = nullptr*/)
    {
        LPGUID lpActiveScheme = NULL;
        LPCGUID lpScheme = Scheme;
        DWORD dwResult = ERROR_SUCCESS;

        do
        {
            if (nullptr == Scheme)
            {
                //检索当前活动的电源方案
                dwResult = ::PowerGetActiveScheme(NULL, &lpActiveScheme);
                if (ERROR_SUCCESS != dwResult)
                {
                    break;
                }

                lpScheme = lpActiveScheme;
            }

            //设置电源方案处于活动状态
            dwResult = ::PowerSetActiveScheme(NULL, lpScheme);
            if (ERROR_SUCCESS != dwResult)
            {
                break;
            }

        } while (FALSE);

        if (lpActiveScheme)
        {
            ::LocalFree(lpActiveScheme);
        }

        return dwResult;
    }

    DWORD SetVideoValue(LPCGUID Scheme, LPCGUID Setting, DWORD AcValue, DWORD DcValue)
    {
        return SetValue(Scheme, &GUID_VIDEO_SUBGROUP, Setting, AcValue, DcValue);
    }

    DWORD SetVideoPowerdownTimeout(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetVideoValue(Scheme, &GUID_VIDEO_POWERDOWN_TIMEOUT, ACValue, DCValue);
    }

    DWORD SetVideoBrightness(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetVideoValue(Scheme, &GUID_DEVICE_POWER_POLICY_VIDEO_BRIGHTNESS, ACValue, DCValue);
    }

    DWORD SetVideoAdaptiveBrightness(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetVideoValue(Scheme, &GUID_VIDEO_ADAPTIVE_DISPLAY_BRIGHTNESS, ACValue, DCValue);
    }

    DWORD SetVideoConsoleLockTimeout(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetVideoValue(Scheme, &GUID_VIDEO_CONSOLE_LOCK_TIMEOUT, ACValue, DCValue);
    }

    DWORD SetVideoAdvancedColorQualityBias(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetVideoValue(Scheme, &GUID_ADVANCED_COLOR_QUALITY_BIAS, ACValue, DCValue);
    }

    DWORD SetSleepValue(LPCGUID Scheme, LPCGUID Setting, DWORD AcValue, DWORD DcValue)
    {
        return SetValue(Scheme, &GUID_SLEEP_SUBGROUP, Setting, AcValue, DcValue);
    }

    DWORD SetUnattendSleepTimeout(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetSleepValue(Scheme, &GUID_UNATTEND_SLEEP_TIMEOUT, ACValue, DCValue);
    }

    DWORD SetSleepHibernateTimeout(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetSleepValue(Scheme, &GUID_HIBERNATE_TIMEOUT, ACValue, DCValue);
    }

    DWORD SetSleepStandbyTimeout(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetSleepValue(Scheme, &GUID_STANDBY_TIMEOUT, ACValue, DCValue);
    }

    DWORD SetProcessorValue(LPCGUID Scheme, LPCGUID Setting, DWORD AcValue, DWORD DcValue)
    {
        return SetValue(Scheme, &GUID_PROCESSOR_SETTINGS_SUBGROUP, Setting, AcValue, DcValue);
    }

    DWORD SetProcessorHeterogeneousPolicy(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetProcessorValue(Scheme, &GUID_PROCESSOR_HETEROGENEOUS_POLICY, ACValue, DCValue);
    }

    DWORD SetProcessorThreadSchedulingPolicy(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetProcessorValue(Scheme, &GUID_PROCESSOR_THREAD_SCHEDULING_POLICY, ACValue, DCValue);
    }

    DWORD SetProcessorShortThreadSchedulingPolicy(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetProcessorValue(Scheme, &GUID_PROCESSOR_SHORT_THREAD_SCHEDULING_POLICY, ACValue, DCValue);
    }

    DWORD SetProcessorFrequencyLimit(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetProcessorValue(Scheme, &GUID_PROCESSOR_FREQUENCY_LIMIT, ACValue, DCValue);
    }

    DWORD SetProcessorFrequencyLimit1(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetProcessorValue(Scheme, &GUID_PROCESSOR_FREQUENCY_LIMIT_1, ACValue, DCValue);
    }

    DWORD SetProcessorThrottleMin(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetProcessorValue(Scheme, &GUID_PROCESSOR_THROTTLE_MINIMUM, ACValue, DCValue);
    }

    DWORD SetProcessorThrottleMin1(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetProcessorValue(Scheme, &GUID_PROCESSOR_THROTTLE_MINIMUM_1, ACValue, DCValue);
    }

    DWORD SetProcessorThrottleMax(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetProcessorValue(Scheme, &GUID_PROCESSOR_THROTTLE_MAXIMUM, ACValue, DCValue);
    }

    DWORD SetProcessorThrottleMax1(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetProcessorValue(Scheme, &GUID_PROCESSOR_THROTTLE_MAXIMUM_1, ACValue, DCValue);
    }

    DWORD SetProcessorAllowThrottling(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetProcessorValue(Scheme, &GUID_PROCESSOR_ALLOW_THROTTLING, ACValue, DCValue);
    }

    DWORD SetProcessorPerfBoostPolicy(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetProcessorValue(Scheme, &GUID_PROCESSOR_PERF_BOOST_POLICY, ACValue, DCValue);
    }

    DWORD SetProcessorPerfBoostMode(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetProcessorValue(Scheme, &GUID_PROCESSOR_PERF_BOOST_MODE, ACValue, DCValue);
    }

    DWORD SetSystemButtonValue(LPCGUID Scheme, LPCGUID Setting, DWORD AcValue, DWORD DcValue)
    {
        return SetValue(Scheme, &GUID_SYSTEM_BUTTON_SUBGROUP, Setting, AcValue, DcValue);
    }

    DWORD SetPowerButtonAction(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetSystemButtonValue(Scheme, &GUID_POWERBUTTON_ACTION, ACValue, DCValue);
    }

    DWORD SetSleepButtonAction(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetSystemButtonValue(Scheme, &GUID_SLEEPBUTTON_ACTION, ACValue, DCValue);
    }

    DWORD SetLidCloseAction(LPCGUID Scheme, DWORD ACValue, DWORD DCValue)
    {
        return SetSystemButtonValue(Scheme, &GUID_LIDCLOSE_ACTION, ACValue, DCValue);
    }
}