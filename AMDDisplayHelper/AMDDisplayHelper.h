#pragma once
#include "AMD/Adl.h"
#include "AMD/GPUScaling.h"

//Define Color Data Struct. This will be shared by Color runtime and UI
//Range data means that the Color feature has min, max, step,expected parameters
struct DisplayColorBasicStruct
{
    int Default = 0;
    int Current = 0;
    int Min = 0;
    int Max = 0;
    int Step = 0;
    bool Support = false;
    bool SourceManual = false;

    DisplayColorBasicStruct(const DisplayColorBasicStruct& input_)
    {
        Default = input_.Default;
        Current = input_.Current;
        Min = input_.Min;
        Max = input_.Max;
        Step = input_.Step;
        Support = input_.Support;
        SourceManual = input_.SourceManual;
    }

    DisplayColorBasicStruct()
    {
    }
};

// AMD 显示辅助类
class AMDDisplayHelper
{
public:

    AMDDisplayHelper();
    ~AMDDisplayHelper();

    bool Initialize();
    void Uninitialize();

    // 色温
    bool GetColorTemperatureState();
    bool SetColorTemperatureState(bool fEnable);
    int GetColorTemperature();
    bool SetColorTemperature(int CurrentVal, bool OnOff);

    // 亮度
    int GetColorBrightness();
    bool SetColorBrightness(int CurrentVal);

    // 色调
    int GetColorHue();
    bool SetColorHue(int CurrentVal);

    // 饱和度
    int GetColorSaturation();
    bool SetColorSaturation(int CurrentVal);

    // 对比度
    int GetColorContrast();
    bool SetColorContrast(int CurrentVal);

    // 虚拟分辨率
    bool GetVirtualResolutionState();
    bool SetVirtualResolutionState(bool fEnable);

    // GPU缩放
    bool GetGpuScalingState();
    GPUSCALINGMODES GetGpuScalingMode();
    bool SetGpuScaling(bool fEnable, GPUSCALINGMODES nMode);

private:

    DisplayColorBasicStruct _GetColorTemperatureMode(const ADLDisplayID displayID);
    DisplayColorBasicStruct _GetColorVal(const ADLDisplayID displayID, int option);
    bool _GetColor(int& CurrentVal, bool OnOff, int option);
    int _GetColor(int option);
    bool _SetColorTemperatureMode(const ADLDisplayID displayID, bool OnOff);
    bool _SetColor(int CurrentVal, bool OnOff, int option);
    int _GetVirtualResolutionState(const ADLDisplayID displayID_);
    bool _SetVirtualResolutionState(const ADLDisplayID displayID_, bool OnOff);

private:
    bool m_fInitAdl;
};

