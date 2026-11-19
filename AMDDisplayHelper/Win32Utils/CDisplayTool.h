#pragma once
#include <windows.h>
#include <tchar.h>
#include <vector>
#include <map>

namespace CDisplayTool
{
    typedef struct _DISPLAY_CONFIG_INFO
    {
        DISPLAYCONFIG_PATH_INFO pathList;
        std::vector<DISPLAYCONFIG_MODE_INFO> modeList;
    }DISPLAY_CONFIG_INFO;

    // 主显示器类型
    enum ePrimaryMonitor
    {
        eIntrenal,
        eExternal,
    };

    // 对齐类型
    enum eAlignType
    {
        eNone,
        eLeftTop,                   // 副显示器在主显示器 左边 顶部 对齐
        eLeftCenter,                // 副显示器在主显示器 左边 居中 对齐
        eLeftBottom,                // 副显示器在主显示器 左边 底部 对齐
        eTopLeft,                   // 副显示器在主显示器 顶部 左边 对齐
        eTopCenter,                 // 副显示器在主显示器 顶部 居中 对齐
        eTopRight,                  // 副显示器在主显示器 顶部 右边 对齐
        eRightTop,                  // 副显示器在主显示器 右边 顶部 对齐
        eRightCenter,               // 副显示器在主显示器 右边 居中 对齐
        eRightBottom,               // 副显示器在主显示器 右边 底部 对齐
        eBottomLeft,                // 副显示器在主显示器 底部 左边 对齐
        eBottomCenter,              // 副显示器在主显示器 底部 居中 对齐
        eBottomRight,               // 副显示器在主显示器 底部 右边 对齐
        eLT = eLeftTop,             // 副显示器在主显示器 左边 顶部 对齐
        eLC = eLeftCenter,          // 副显示器在主显示器 左边 居中 对齐
        eLB = eLeftBottom,          // 副显示器在主显示器 左边 底部 对齐
        eTL = eTopLeft,             // 副显示器在主显示器 顶部 左边 对齐
        eTC = eTopCenter,           // 副显示器在主显示器 顶部 居中 对齐
        eTR = eTopRight,            // 副显示器在主显示器 顶部 右边 对齐
        eRT = eRightTop,            // 副显示器在主显示器 右边 顶部 对齐
        eRC = eRightCenter,         // 副显示器在主显示器 右边 居中 对齐
        eRB = eRightBottom,         // 副显示器在主显示器 右边 底部 对齐
        eBL = eBottomLeft,          // 副显示器在主显示器 底部 左边 对齐
        eBC = eBottomCenter,        // 副显示器在主显示器 底部 居中 对齐
        eBR = eBottomRight,         // 副显示器在主显示器 底部 右边 对齐
        eLeft = eLeftCenter,        // 副显示器在主显示器 左边 居中 对齐
        eTop = eTopCenter,          // 副显示器在主显示器 顶部 居中 对齐
        eRight = eRightCenter,      // 副显示器在主显示器 右边 居中 对齐
        eBottom = eBottomCenter,    // 副显示器在主显示器 底部 居中 对齐
    };

    // 获取显示模式
    DWORD GetDisplayMode();

    // 是否为内部模式
    LONG IsInternalMode();

    // 是否为复制模式
    LONG IsCloneMode();

    // 是否为扩展显示模式
    LONG IsExtendMode();

    // 是否为外部显示模式
    LONG IsExternalMode();

    // 设置为内部显示模式
    LONG SetInternalMode();

    // 设置为复制模式
    LONG SetCloneMode();

    // 设置为扩展显示模式
    LONG SetExtendMode();

    // 设置为外部显示模式
    LONG SetExternalMode();

    // 获取主显示器
    ePrimaryMonitor GetPrimaryMonitor();

    // 设置主显示器
    bool SetPrimaryMonitor(ePrimaryMonitor primaryMonitor);

    // 获取副显示器位置
    eAlignType GetSecondaryMonitorPos();

    // 设置副显示器位置
    bool SetSecondaryMonitorPos(eAlignType hAlignType = eAlignType::eNone);

    // 是否支持HDR
    bool IsHDRSupported();

    // 是否启用HDR
    bool IsHDREnabled();

    // 启用HDR
    bool HDREnable(bool fEnable);

    // 获取显示配置
    std::vector<DISPLAY_CONFIG_INFO> GetDisplayConfigList();
};

