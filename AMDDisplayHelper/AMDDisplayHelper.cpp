#include "AMDDisplayHelper.h"
#include <algorithm>
#include <tchar.h>

#pragma comment(lib, "Crypt32.lib")

AMDDisplayHelper::AMDDisplayHelper()
    :
    m_fInitAdl(false)
{

}
AMDDisplayHelper::~AMDDisplayHelper()
{
    if (m_fInitAdl)
    {
        Uninitialize();
    }
}

bool AMDDisplayHelper::Initialize()
{
    // 初始化 ADL
    if (ADL_OK != InitADL())
    {
        return true;
    }

    // 准备API
    PrepareAPI();

    return true;
}

void AMDDisplayHelper::Uninitialize()
{
    if (m_fInitAdl)
    {
        DestroyADL();
    }

    m_fInitAdl = false;
}

int AMDDisplayHelper::_GetVirtualResolutionState(const ADLDisplayID displayID_)
{
    ADLDisplayProperty DisplayProperty;
    memset(&DisplayProperty, 0, sizeof(DisplayProperty));
    DisplayProperty.iSize = sizeof(DisplayProperty);
    DisplayProperty.iPropertyType = ADL_DL_DISPLAYPROPERTY_TYPE_DOWNSCALE;
    DisplayProperty.iDefault = -1;
    DisplayProperty.iCurrent = -1;

    if (ADL_OK == ADL2_Display_Property_Get(ADLApp_context.adlContext, displayID_.iDisplayLogicalAdapterIndex, displayID_.iDisplayLogicalIndex, &DisplayProperty))
    {
        return (DisplayProperty.iCurrent == 1) ? 1 : 0;
    }

    return -1;
}

bool AMDDisplayHelper::_SetVirtualResolutionState(const ADLDisplayID displayID_, bool OnOff)
{
    ADLDisplayProperty DisplayProperty;
    memset(&DisplayProperty, 0, sizeof(DisplayProperty));
    DisplayProperty.iSize = sizeof(DisplayProperty);
    DisplayProperty.iPropertyType = ADL_DL_DISPLAYPROPERTY_TYPE_DOWNSCALE;
    DisplayProperty.iDefault = -1;
    DisplayProperty.iCurrent = -1;

    if ((_GetVirtualResolutionState(displayID_) == 1 ? true : false) == OnOff)
    {
        return true;
    }

    DisplayProperty.iCurrent = OnOff;
    if (ADL_OK == ADL2_Display_Property_Set(ADLApp_context.adlContext, displayID_.iDisplayLogicalAdapterIndex, displayID_.iDisplayLogicalIndex, &DisplayProperty))
    {
        if ((_GetVirtualResolutionState(displayID_) == 1 ? true : false) == OnOff)
        {
            return true;
        }
    }

    return false;
}

DisplayColorBasicStruct AMDDisplayHelper::_GetColorVal(const ADLDisplayID displayID, int option)
{
    DisplayColorBasicStruct ColorBasic;
    //If the color temperature source is manual, get the supported value range.
    if (ADL_OK != ADL2_Display_Color_Get(
        ADLApp_context.adlContext,
        displayID.iDisplayLogicalAdapterIndex,
        displayID.iDisplayLogicalIndex,
        option,
        &(ColorBasic.Current), &(ColorBasic.Default), &(ColorBasic.Min), &(ColorBasic.Max), &(ColorBasic.Step)))
    {
        _tprintf(_T("Failed to get color temperature values\n"));
    }
    return ColorBasic;
}

bool AMDDisplayHelper::_GetColor(int& CurrentVal, bool OnOff, int option)
{
    bool fResult = false;
    std::for_each(std::begin(
        DisplayID),
        std::end(DisplayID),
        [&fResult, this, &OnOff, &CurrentVal, &option](const std::pair<int, ADLDisplayID*>& kv) {
            if (OnOff)
            {
                DisplayColorBasicStruct ColorBasic;
                ADLDisplayID* displayID_ = kv.second;
                if (ADL_OK == ADL2_Display_Color_Get(
                    ADLApp_context.adlContext,
                    displayID_->iDisplayLogicalAdapterIndex,
                    displayID_->iDisplayLogicalIndex,
                    option,
                    &(ColorBasic.Current),
                    &(ColorBasic.Default),
                    &(ColorBasic.Min),
                    &(ColorBasic.Max),
                    &(ColorBasic.Step))
                    )
                {
                    CurrentVal = ColorBasic.Current;
                }
                else
                {
                    _tprintf(_T("Failed to get color values\n"));
                }
            }
        }
    );

    return fResult;
}

int AMDDisplayHelper::_GetColor(int option)
{
    int nResult = 0;
    _GetColor(nResult, true, option);
    return nResult;
}

bool AMDDisplayHelper::_SetColor(int CurrentVal, bool OnOff, int option)
{
    bool fResult = false;
    DisplayColorBasicStruct pColorBasic;
    std::for_each(std::begin(
        DisplayID),
        std::end(DisplayID),
        [&fResult, this, &OnOff, &pColorBasic, &CurrentVal, &option](const std::pair<int, ADLDisplayID*>& kv) {
            if (OnOff)
            {
                pColorBasic = _GetColorVal(*(kv.second), option);
                if (CurrentVal <= pColorBasic.Max && CurrentVal >= pColorBasic.Min)
                {
                    ADLDisplayID* displayID = kv.second;
                    if (ADL_OK == ADL2_Display_Color_Set(ADLApp_context.adlContext, displayID->iDisplayLogicalAdapterIndex, displayID->iDisplayLogicalIndex, option, CurrentVal))
                    {
                        fResult = true;
                    }
                    else
                    {
                        _tprintf(_T("Failed set color options\n"));
                    }
                }
            }
        }
    );

    return fResult;
}

bool AMDDisplayHelper::_SetColorTemperatureMode(const ADLDisplayID displayID, bool OnOff)
{
    if (ADL_OK == ADL2_Display_ColorTemperatureSource_Set(
        ADLApp_context.adlContext, 
        displayID.iDisplayLogicalAdapterIndex, 
        displayID.iDisplayLogicalIndex, 
        (OnOff == false) ? ADL_DISPLAY_COLOR_TEMPERATURE_SOURCE_EDID : ADL_DISPLAY_COLOR_TEMPERATURE_SOURCE_USER)
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}

DisplayColorBasicStruct AMDDisplayHelper::_GetColorTemperatureMode(const ADLDisplayID displayID)
{
    int temperatureSource;
    DisplayColorBasicStruct ColorBasic;
    if (ADL_OK == ADL2_Display_ColorTemperatureSource_Get(
        ADLApp_context.adlContext, 
        displayID.iDisplayLogicalAdapterIndex, 
        displayID.iDisplayLogicalIndex, &temperatureSource)
        )
    {
        //If the color temperature source is manual, get the supported value range.
        if (temperatureSource == ADL_DISPLAY_COLOR_TEMPERATURE_SOURCE_USER)
        {
            ColorBasic.SourceManual = true;
        }
        else
        {
            ColorBasic.SourceManual = false;
        }

        if (ADL_OK != ADL2_Display_Color_Get(
            ADLApp_context.adlContext, 
            displayID.iDisplayLogicalAdapterIndex, 
            displayID.iDisplayLogicalIndex, 
            ADL_DISPLAY_COLOR_TEMPERATURE,
            &(ColorBasic.Current), 
            &(ColorBasic.Default), 
            &(ColorBasic.Min), 
            &(ColorBasic.Max), 
            &(ColorBasic.Step))
            )
        {
            _tprintf(_T("Failed to get color temperature values\n"));
        }
    }
    return ColorBasic;
}

bool AMDDisplayHelper::GetColorTemperatureState()
{
    bool bState = false;
    std::for_each(std::begin(DisplayID), std::end(DisplayID), [this, &bState](const std::pair<int, ADLDisplayID*>& kv) {

        DisplayColorBasicStruct ColorBasic;
        ColorBasic = _GetColorTemperatureMode(*(kv.second));
        if (ColorBasic.SourceManual)
        {
            bState = true;
        }
        });

    return bState;
}

bool AMDDisplayHelper::SetColorTemperatureState(bool fEnable)
{
    bool bResult = false;
    std::for_each(std::begin(DisplayID), std::end(DisplayID),
        [this, &bResult, &fEnable](const std::pair<int, ADLDisplayID*>& kv) {
            bResult = _SetColorTemperatureMode(*(kv.second), fEnable);
        }
    );

    return bResult;
}

int AMDDisplayHelper::GetColorTemperature()
{
    return _GetColor(ADL_DISPLAY_COLOR_TEMPERATURE);
}

bool AMDDisplayHelper::SetColorTemperature(int CurrentVal, bool OnOff)
{
    _SetColor(CurrentVal, OnOff, ADL_DISPLAY_COLOR_TEMPERATURE);
    SetColorTemperatureState(OnOff);

    return true;
}

int AMDDisplayHelper::GetColorBrightness()
{
    return _GetColor(ADL_DISPLAY_COLOR_BRIGHTNESS);
}

bool AMDDisplayHelper::SetColorBrightness(int CurrentVal)
{
    return _SetColor(CurrentVal, true, ADL_DISPLAY_COLOR_BRIGHTNESS);
}

int AMDDisplayHelper::GetColorHue()
{
    return _GetColor(ADL_DISPLAY_COLOR_HUE);
}

bool AMDDisplayHelper::SetColorHue(int CurrentVal)
{
    return _SetColor(CurrentVal, true, ADL_DISPLAY_COLOR_HUE);
}

int AMDDisplayHelper::GetColorSaturation()
{
    return _GetColor(ADL_DISPLAY_COLOR_SATURATION);
}

bool AMDDisplayHelper::SetColorSaturation(int CurrentVal)
{
    return _SetColor(CurrentVal, true, ADL_DISPLAY_COLOR_SATURATION);
}

int AMDDisplayHelper::GetColorContrast()
{
    return _GetColor(ADL_DISPLAY_COLOR_CONTRAST);
}

bool AMDDisplayHelper::SetColorContrast(int CurrentVal)
{
    return _SetColor(CurrentVal, true, ADL_DISPLAY_COLOR_CONTRAST);
}

bool AMDDisplayHelper::GetVirtualResolutionState()
{
    bool fResult = false;
    std::for_each(std::begin(DisplayID), std::end(DisplayID), [this, &fResult](const std::pair<int, ADLDisplayID*>& kv) {
        int nResult = _GetVirtualResolutionState(*(kv.second));
        if (nResult > 0)
        {
            fResult = nResult;
        }

        });

    return fResult;
}

bool AMDDisplayHelper::SetVirtualResolutionState(bool fEnable)
{
    bool fResult = false;
    std::for_each(std::begin(DisplayID), std::end(DisplayID), [this, &fResult, fEnable](const std::pair<int, ADLDisplayID*>& kv) {
        fResult = _SetVirtualResolutionState(*(kv.second), fEnable);
        });

    return fResult;
}

bool AMDDisplayHelper::GetGpuScalingState()
{
    GPUScaling GPUScale(ADLApp_context.adlContext);
    bool fResult = false;

    std::for_each(std::begin(DisplayID), std::end(DisplayID), [&GPUScale, &fResult](const std::pair<int, ADLDisplayID*>& kv) {
        GPUScale.GetGPUScalingEnable(*(kv.second), fResult);
        });

    return fResult;
}

GPUSCALINGMODES AMDDisplayHelper::GetGpuScalingMode()
{
    GPUScaling GPUScale(ADLApp_context.adlContext);
    GPUSCALINGMODES scalingMode = GPUSCALINGMODES::SCALING_PreserveAspectRatio;

    std::for_each(std::begin(DisplayID), std::end(DisplayID), [&GPUScale, &scalingMode](const std::pair<int, ADLDisplayID*>& kv) {
        scalingMode = GPUScale.GetDisplayScalingMode(*(kv.second));
        });

    return scalingMode;
}

bool AMDDisplayHelper::SetGpuScaling(bool fEnable, GPUSCALINGMODES nMode)
{
    bool fResult = false;
    GPUScaling GPUScale(ADLApp_context.adlContext);

    std::for_each(std::begin(DisplayID), std::end(DisplayID), [nMode, &GPUScale, fEnable](const std::pair<int, ADLDisplayID*>& kv) {
        GPUScale.SetGPUScalingEnable(*(kv.second), fEnable);
        GPUScale.SetDisplayScalingMode(*(kv.second), nMode);
        });

    return fResult;
}
