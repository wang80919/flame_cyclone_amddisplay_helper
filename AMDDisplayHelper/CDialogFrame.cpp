#include "CDialogFrame.h"
#include <Commctrl.h>
#include <strsafe.h>
#include <imm.h>
#include <thread>
#include "resource.h"
#include "win32Utils/CDisplayTool.h"
#include "win32Utils/CPowerCfgUtils.h"

#define WM_TRAYICON                 (WM_USER + 10)

BEGIN_DLG_MESSAGE_MAP(CDialogFrame, CDialogBase)
    ON_DLG_MESSAGE(WM_INITDIALOG, &CDialogFrame::OnInitDialog)
    ON_DLG_MESSAGE(WM_CLOSE, &CDialogFrame::OnClose)
    ON_DLG_MESSAGE(WM_WINDOWPOSCHANGING, &CDialogFrame::OnWindowPosChinging)
    ON_DLG_MESSAGE(WM_RBUTTONUP, &CDialogFrame::OnRButtonUp)
    ON_DLG_MESSAGE(WM_TRAYICON, &CDialogFrame::OnTrayIcon)
    ON_DLG_MESSAGE(WM_HSCROLL, &CDialogFrame::OnHScroll)
    ON_DLG_MESSAGE(WM_DISPLAYCHANGE, &CDialogFrame::OnDisplayChange)
    ON_DLG_COMMAND(IDC_BUTTON_DEFAULT, &CDialogFrame::OnButtonDefault)
    ON_DLG_COMMAND(IDC_BUTTON_HDR, &CDialogFrame::OnButtonHDR)
    ON_DLG_COMMAND(IDC_CHECK_TEMP, &CDialogFrame::OnCheckTemp)
    ON_DLG_COMMAND(IDC_CHECK_HDR, &CDialogFrame::OnCheckHDR)
    ON_DLG_COMMAND_RANGE(IDC_RADIO_PRIMARY_INTERNAL, IDC_RADIO_PRIMARY_EXTERNAL, &CDialogFrame::OnDisplayPrimary)
    ON_DLG_COMMAND_RANGE(IDC_RADIO_INTERNAL, IDC_RADIO_EXTERNAL, &CDialogFrame::OnDisplayMode)
    ON_DLG_COMMAND_RANGE(IDC_RADIO_LT, IDC_RADIO_BR, &CDialogFrame::OnDisplayPos)
END_DLG_MESSAGE_MAP()

LRESULT CDialogFrame::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
    m_hHideWnd = CreateWindow(
        _T("Static"), _T("Hide"), WS_CHILDWINDOW, 0, 0, 0, 0,
        m_hWnd, NULL, GetModuleHandle(NULL), NULL);
    if (m_hHideWnd)
    {
        ::SetParent(m_hHideWnd, NULL);
    }

    m_hMainMenu = ::GetMenu(m_hWnd);
    if (m_hMainMenu)
    {
        ShowTrayIcon();
    }

    CHANGEFILTERSTRUCT cfs = { 0 };
    cfs.cbSize = sizeof(cfs);

    //允许低权限进程拖动文件进来
    ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, &cfs);
    ChangeWindowMessageFilterEx(m_hWnd, WM_COPYDATA, MSGFLT_ALLOW, &cfs);
    ChangeWindowMessageFilterEx(m_hWnd, WM_COPYDATA - 1, MSGFLT_ALLOW, &cfs);

    ::SetMenu(m_hWnd, m_hMainMenu);

    HICON hIconSmall = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN));
    HICON hIconBig = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN));
    if (hIconBig)
    {
        ::SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIconBig);
    }

    if (hIconSmall)
    {
        ::SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSmall);
    }

    m_AMDDisplayHelper.Initialize();

    int brightness = m_cfg.GetInt(_T("config"), _T("brightness"), 0);
    ::SendMessage(GetItem(IDC_SLIDER_BRIGHTNESS), TBM_SETRANGEMIN, TRUE, -100);
    ::SendMessage(GetItem(IDC_SLIDER_BRIGHTNESS), TBM_SETRANGEMAX, TRUE, 100);
    ::SendMessage(GetItem(IDC_SLIDER_BRIGHTNESS), TBM_SETPOS, TRUE, brightness);
    SetItemInt(IDC_STATIC_BRIGHTNESS, brightness);
    m_AMDDisplayHelper.SetColorBrightness(brightness);

    int contrast = m_cfg.GetInt(_T("config"), _T("contrast"), 100);
    ::SendMessage(GetItem(IDC_SLIDER_CONSTRAST), TBM_SETRANGEMIN, TRUE, 0);
    ::SendMessage(GetItem(IDC_SLIDER_CONSTRAST), TBM_SETRANGEMAX, TRUE, 200);
    ::SendMessage(GetItem(IDC_SLIDER_CONSTRAST), TBM_SETPOS, TRUE, contrast);
    SetItemInt(IDC_STATIC_CONSTRAST, contrast);
    m_AMDDisplayHelper.SetColorContrast(contrast);

    int saturation = m_cfg.GetInt(_T("config"), _T("saturation"), 100);
    ::SendMessage(GetItem(IDC_SLIDER_SATURATION), TBM_SETRANGEMIN, TRUE, 0);
    ::SendMessage(GetItem(IDC_SLIDER_SATURATION), TBM_SETRANGEMAX, TRUE, 200);
    ::SendMessage(GetItem(IDC_SLIDER_SATURATION), TBM_SETPOS, TRUE, saturation);
    SetItemInt(IDC_STATIC_SATURATION, saturation);
    m_AMDDisplayHelper.SetColorSaturation(saturation);

    CheckButton(IDC_CHECK_TEMP, m_AMDDisplayHelper.GetColorTemperatureState());
    EnableWindow(GetItem(IDC_SLIDER_TEMP), m_AMDDisplayHelper.GetColorTemperatureState());
    int temp_value = m_cfg.GetInt(_T("config"), _T("temp_value"), 6500) / 100;
    ::SendMessage(GetItem(IDC_SLIDER_TEMP), TBM_SETRANGEMIN, TRUE, 40);
    ::SendMessage(GetItem(IDC_SLIDER_TEMP), TBM_SETRANGEMAX, TRUE, 100);
    ::SendMessage(GetItem(IDC_SLIDER_TEMP), TBM_SETPOS, TRUE, temp_value);
    SetItemInt(IDC_STATIC_TEMP, temp_value * 100);
    m_AMDDisplayHelper.SetColorTemperature(temp_value * 100, m_AMDDisplayHelper.GetColorTemperatureState());

    int hue = m_cfg.GetInt(_T("config"), _T("hue"), 0);
    ::SendMessage(GetItem(IDC_SLIDER_HUE), TBM_SETRANGEMIN, TRUE, -30);
    ::SendMessage(GetItem(IDC_SLIDER_HUE), TBM_SETRANGEMAX, TRUE, 30);
    ::SendMessage(GetItem(IDC_SLIDER_HUE), TBM_SETPOS, TRUE, hue);
    SetItemInt(IDC_STATIC_HUE, hue);
    m_AMDDisplayHelper.SetColorHue(hue);

    int gamma = m_cfg.GetInt(_T("config"), _T("gamma"), 128);
    ::SendMessage(GetItem(IDC_SLIDER_GAMMA), TBM_SETRANGEMIN, TRUE, 0);
    ::SendMessage(GetItem(IDC_SLIDER_GAMMA), TBM_SETRANGEMAX, TRUE, 255);
    ::SendMessage(GetItem(IDC_SLIDER_GAMMA), TBM_SETPOS, TRUE, gamma);
    SetItemInt(IDC_STATIC_GAMMA, gamma);
    SetDisplayGamma(gamma);

    UpdateDisplayStatus();

    return LRESULT(TRUE);
}

LRESULT CDialogFrame::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
    UpdateDisplayStatus();
    return LRESULT(TRUE);
}

void CDialogFrame::UpdateDisplayStatus()
{
    CheckButton(IDC_CHECK_HDR, CDisplayTool::IsHDREnabled());
    EnableWindow(GetItem(IDC_CHECK_HDR), CDisplayTool::IsHDRSupported());

    DWORD dwMode = CDisplayTool::GetDisplayMode();
    switch (dwMode)
    {
    case SDC_TOPOLOGY_INTERNAL:
        CheckRadioButton(IDC_RADIO_INTERNAL, IDC_RADIO_EXTERNAL, IDC_RADIO_INTERNAL);
        break;
    case SDC_TOPOLOGY_CLONE:
        CheckRadioButton(IDC_RADIO_INTERNAL, IDC_RADIO_EXTERNAL, IDC_RADIO_CLONE);
        break;
    case SDC_TOPOLOGY_EXTEND:
        CheckRadioButton(IDC_RADIO_INTERNAL, IDC_RADIO_EXTERNAL, IDC_RADIO_EXTEND);
        break;
    case SDC_TOPOLOGY_EXTERNAL:
        CheckRadioButton(IDC_RADIO_INTERNAL, IDC_RADIO_EXTERNAL, IDC_RADIO_EXTERNAL);
        break;
    }

    CDisplayTool::ePrimaryMonitor mainMonitor = CDisplayTool::GetPrimaryMonitor();
    switch (mainMonitor)
    {
    case CDisplayTool::ePrimaryMonitor::eIntrenal:
        CheckRadioButton(IDC_RADIO_PRIMARY_INTERNAL, IDC_RADIO_PRIMARY_EXTERNAL, IDC_RADIO_PRIMARY_INTERNAL);
        break;
    case CDisplayTool::ePrimaryMonitor::eExternal:
        CheckRadioButton(IDC_RADIO_PRIMARY_INTERNAL, IDC_RADIO_PRIMARY_EXTERNAL, IDC_RADIO_PRIMARY_EXTERNAL);
        break;
    }

    CDisplayTool::eAlignType alignType = CDisplayTool::GetSecondaryMonitorPos();
    switch (alignType)
    {
    case CDisplayTool::eAlignType::eLT:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_LT);
        break;
    case CDisplayTool::eAlignType::eLC:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_LC);
        break;
    case CDisplayTool::eAlignType::eLB:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_LB);
        break;
    case CDisplayTool::eAlignType::eRT:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_RT);
        break;
    case CDisplayTool::eAlignType::eRC:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_RC);
        break;
    case CDisplayTool::eAlignType::eRB:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_RB);
        break;
    case CDisplayTool::eAlignType::eTL:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_TL);
        break;
    case CDisplayTool::eAlignType::eTC:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_TC);
        break;
    case CDisplayTool::eAlignType::eTR:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_TR);
        break;
    case CDisplayTool::eAlignType::eBL:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_BL);
        break;
    case CDisplayTool::eAlignType::eBC:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_BC);
        break;
    case CDisplayTool::eAlignType::eBR:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_BR);
        break;
    default:
        CheckRadioButton(IDC_RADIO_LT, IDC_RADIO_BR, IDC_RADIO_BR);
        CheckButton(IDC_RADIO_BR, false);
        break;
    }
}

LRESULT CDialogFrame::OnHScroll(WPARAM wParam, LPARAM lParam)
{
    int nPos = ::SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);
    if ((HWND)lParam == GetItem(IDC_SLIDER_BRIGHTNESS))
    {
        m_AMDDisplayHelper.SetColorBrightness(nPos);
        SetItemInt(IDC_STATIC_BRIGHTNESS, nPos);
        m_cfg.SetInt(_T("config"), _T("brightness"), nPos);
    }

    if ((HWND)lParam == GetItem(IDC_SLIDER_CONSTRAST))
    {
        m_AMDDisplayHelper.SetColorContrast(nPos);
        SetItemInt(IDC_STATIC_CONSTRAST, nPos);
        m_cfg.SetInt(_T("config"), _T("contrast"), nPos);
    }

    if ((HWND)lParam == GetItem(IDC_SLIDER_SATURATION))
    {
        m_AMDDisplayHelper.SetColorSaturation(nPos);
        SetItemInt(IDC_STATIC_SATURATION, nPos);
        m_cfg.SetInt(_T("config"), _T("saturation"), nPos);
    }

    if ((HWND)lParam == GetItem(IDC_SLIDER_HUE))
    {
        m_AMDDisplayHelper.SetColorHue(nPos);
        SetItemInt(IDC_STATIC_HUE, nPos);
        m_cfg.SetInt(_T("config"), _T("hue"), nPos);
    }

    if ((HWND)lParam == GetItem(IDC_SLIDER_TEMP))
    {
        m_AMDDisplayHelper.SetColorTemperature(nPos * 100, IsButtonChecked(IDC_CHECK_TEMP));
        SetItemInt(IDC_STATIC_TEMP, nPos * 100);
        m_cfg.SetInt(_T("config"), _T("temp_value"), nPos * 100);
    }

    if ((HWND)lParam == GetItem(IDC_SLIDER_GAMMA))
    {
        SetDisplayGamma(nPos);
        SetItemInt(IDC_STATIC_GAMMA, nPos);
        m_cfg.SetInt(_T("config"), _T("gamma"), nPos);
    }

    return LRESULT(FALSE);
}

LRESULT CDialogFrame::OnDisplayPrimary(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd)
{
    CheckRadioButton(IDC_RADIO_PRIMARY_INTERNAL, IDC_RADIO_PRIMARY_EXTERNAL, wID);

    switch (wID)
    {
    case IDC_RADIO_PRIMARY_INTERNAL:
        CDisplayTool::SetPrimaryMonitor(CDisplayTool::ePrimaryMonitor::eIntrenal);
        break;
    case IDC_RADIO_PRIMARY_EXTERNAL:
        CDisplayTool::SetPrimaryMonitor(CDisplayTool::ePrimaryMonitor::eExternal);
        break;
    }


    return LRESULT(FALSE);
}

LRESULT CDialogFrame::OnDisplayMode(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd)
{
    CheckRadioButton(IDC_RADIO_INTERNAL, IDC_RADIO_EXTERNAL, wID);

    switch (wID)
    {
    case IDC_RADIO_INTERNAL:
        CDisplayTool::SetInternalMode();
        break;
    case IDC_RADIO_CLONE:
        CDisplayTool::SetCloneMode();
        break;
    case IDC_RADIO_EXTEND:
        CDisplayTool::SetExtendMode();
        break;
    case IDC_RADIO_EXTERNAL:
        CDisplayTool::SetExternalMode();
        break;
    }

    return LRESULT(TRUE);
}

LRESULT CDialogFrame::OnDisplayPos(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd)
{
    switch (wID)
    {
    case IDC_RADIO_LT:
        CDisplayTool::SetSecondaryMonitorPos(CDisplayTool::eAlignType::eLeftTop);
        break;
    case IDC_RADIO_LC:
        CDisplayTool::SetSecondaryMonitorPos(CDisplayTool::eAlignType::eLeftCenter);
        break;
    case IDC_RADIO_LB:
        CDisplayTool::SetSecondaryMonitorPos(CDisplayTool::eAlignType::eLeftBottom);
        break;
    case IDC_RADIO_RT:
        CDisplayTool::SetSecondaryMonitorPos(CDisplayTool::eAlignType::eRightTop);
        break;
    case IDC_RADIO_RC:
        CDisplayTool::SetSecondaryMonitorPos(CDisplayTool::eAlignType::eRightCenter);
        break;
    case IDC_RADIO_RB:
        CDisplayTool::SetSecondaryMonitorPos(CDisplayTool::eAlignType::eRightBottom);
        break;
    case IDC_RADIO_TL:
        CDisplayTool::SetSecondaryMonitorPos(CDisplayTool::eAlignType::eTopLeft);
        break;
    case IDC_RADIO_TC:
        CDisplayTool::SetSecondaryMonitorPos(CDisplayTool::eAlignType::eTopCenter);
        break;
    case IDC_RADIO_TR:
        CDisplayTool::SetSecondaryMonitorPos(CDisplayTool::eAlignType::eTopRight);
        break;
    case IDC_RADIO_BL:
        CDisplayTool::SetSecondaryMonitorPos(CDisplayTool::eAlignType::eBottomLeft);
        break;
    case IDC_RADIO_BC:
        CDisplayTool::SetSecondaryMonitorPos(CDisplayTool::eAlignType::eBottomCenter);
        break;
    case IDC_RADIO_BR:
        CDisplayTool::SetSecondaryMonitorPos(CDisplayTool::eAlignType::eBottomRight);
        break;
    }

    return LRESULT(TRUE);
}

LRESULT CDialogFrame::OnButtonDefault(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd)
{
    int brightness = 0;
    ::SendMessage(GetItem(IDC_SLIDER_BRIGHTNESS), TBM_SETRANGEMIN, TRUE, -100);
    ::SendMessage(GetItem(IDC_SLIDER_BRIGHTNESS), TBM_SETRANGEMAX, TRUE, 100);
    ::SendMessage(GetItem(IDC_SLIDER_BRIGHTNESS), TBM_SETPOS, TRUE, brightness);
    SetItemInt(IDC_STATIC_BRIGHTNESS, brightness);
    m_AMDDisplayHelper.SetColorBrightness(brightness);
    m_cfg.SetInt(_T("config"), _T("brightness"), brightness);

    int saturation = 100;
    ::SendMessage(GetItem(IDC_SLIDER_SATURATION), TBM_SETRANGEMIN, TRUE, 0);
    ::SendMessage(GetItem(IDC_SLIDER_SATURATION), TBM_SETRANGEMAX, TRUE, 200);
    ::SendMessage(GetItem(IDC_SLIDER_SATURATION), TBM_SETPOS, TRUE, saturation);
    SetItemInt(IDC_STATIC_SATURATION, saturation);
    m_AMDDisplayHelper.SetColorSaturation(saturation);
    m_cfg.SetInt(_T("config"), _T("saturation"), saturation);

    int contrast = 100;
    ::SendMessage(GetItem(IDC_SLIDER_CONSTRAST), TBM_SETRANGEMIN, TRUE, 0);
    ::SendMessage(GetItem(IDC_SLIDER_CONSTRAST), TBM_SETRANGEMAX, TRUE, 200);
    ::SendMessage(GetItem(IDC_SLIDER_CONSTRAST), TBM_SETPOS, TRUE, contrast);
    SetItemInt(IDC_STATIC_CONSTRAST, contrast);
    m_AMDDisplayHelper.SetColorContrast(contrast);
    m_cfg.SetInt(_T("config"), _T("contrast"), contrast);

    int temp_value = 65;
    ::SendMessage(GetItem(IDC_SLIDER_TEMP), TBM_SETRANGEMIN, TRUE, 40);
    ::SendMessage(GetItem(IDC_SLIDER_TEMP), TBM_SETRANGEMAX, TRUE, 100);
    ::SendMessage(GetItem(IDC_SLIDER_TEMP), TBM_SETPOS, TRUE, temp_value);
    SetItemInt(IDC_STATIC_TEMP, temp_value * 100);
    m_cfg.SetInt(_T("config"), _T("temp_value"), temp_value * 100);
    m_AMDDisplayHelper.SetColorTemperature(temp_value * 100, m_AMDDisplayHelper.GetColorTemperatureState());

    int hue = 0;
    ::SendMessage(GetItem(IDC_SLIDER_HUE), TBM_SETRANGEMIN, TRUE, -30);
    ::SendMessage(GetItem(IDC_SLIDER_HUE), TBM_SETRANGEMAX, TRUE, 30);
    ::SendMessage(GetItem(IDC_SLIDER_HUE), TBM_SETPOS, TRUE, hue);
    SetItemInt(IDC_STATIC_HUE, hue);
    m_AMDDisplayHelper.SetColorHue(hue);
    m_cfg.SetInt(_T("config"), _T("hue"), hue);

    int gamma = 128;
    ::SendMessage(GetItem(IDC_SLIDER_GAMMA), TBM_SETRANGEMIN, TRUE, 0);
    ::SendMessage(GetItem(IDC_SLIDER_GAMMA), TBM_SETRANGEMAX, TRUE, 255);
    ::SendMessage(GetItem(IDC_SLIDER_GAMMA), TBM_SETPOS, TRUE, gamma);
    SetItemInt(IDC_STATIC_GAMMA, gamma);
    SetDisplayGamma(gamma);
    m_cfg.SetInt(_T("config"), _T("gamma"), gamma);

    return LRESULT(TRUE);
}

LRESULT CDialogFrame::OnButtonHDR(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd)
{
    // 禁用开关: 当我的电脑使用电池运行时关闭HDR
    CPowerCfgUtils::SetVideoAdvancedColorQualityBias(&ALL_POWERSCHEMES_GUID, 1, 1);
    CPowerCfgUtils::SetActiveScheme(nullptr);

    int brightness = 0;
    ::SendMessage(GetItem(IDC_SLIDER_BRIGHTNESS), TBM_SETRANGEMIN, TRUE, -100);
    ::SendMessage(GetItem(IDC_SLIDER_BRIGHTNESS), TBM_SETRANGEMAX, TRUE, 100);
    ::SendMessage(GetItem(IDC_SLIDER_BRIGHTNESS), TBM_SETPOS, TRUE, brightness);
    SetItemInt(IDC_STATIC_BRIGHTNESS, brightness);
    m_AMDDisplayHelper.SetColorBrightness(brightness);
    m_cfg.SetInt(_T("config"), _T("brightness"), brightness);

    int saturation = 130;
    ::SendMessage(GetItem(IDC_SLIDER_SATURATION), TBM_SETRANGEMIN, TRUE, 0);
    ::SendMessage(GetItem(IDC_SLIDER_SATURATION), TBM_SETRANGEMAX, TRUE, 200);
    ::SendMessage(GetItem(IDC_SLIDER_SATURATION), TBM_SETPOS, TRUE, saturation);
    SetItemInt(IDC_STATIC_SATURATION, saturation);
    m_AMDDisplayHelper.SetColorSaturation(saturation);
    m_cfg.SetInt(_T("config"), _T("saturation"), saturation);

    int contrast = 150;
    ::SendMessage(GetItem(IDC_SLIDER_CONSTRAST), TBM_SETRANGEMIN, TRUE, 0);
    ::SendMessage(GetItem(IDC_SLIDER_CONSTRAST), TBM_SETRANGEMAX, TRUE, 200);
    ::SendMessage(GetItem(IDC_SLIDER_CONSTRAST), TBM_SETPOS, TRUE, contrast);
    SetItemInt(IDC_STATIC_CONSTRAST, contrast);
    m_AMDDisplayHelper.SetColorContrast(contrast);
    m_cfg.SetInt(_T("config"), _T("contrast"), contrast);

    int temp_value = 65;
    ::SendMessage(GetItem(IDC_SLIDER_TEMP), TBM_SETRANGEMIN, TRUE, 40);
    ::SendMessage(GetItem(IDC_SLIDER_TEMP), TBM_SETRANGEMAX, TRUE, 100);
    ::SendMessage(GetItem(IDC_SLIDER_TEMP), TBM_SETPOS, TRUE, temp_value);
    SetItemInt(IDC_STATIC_TEMP, temp_value * 100);
    m_cfg.SetInt(_T("config"), _T("temp_value"), temp_value * 100);
    m_AMDDisplayHelper.SetColorTemperature(temp_value * 100, m_AMDDisplayHelper.GetColorTemperatureState());

    int hue = 0;
    ::SendMessage(GetItem(IDC_SLIDER_HUE), TBM_SETRANGEMIN, TRUE, -30);
    ::SendMessage(GetItem(IDC_SLIDER_HUE), TBM_SETRANGEMAX, TRUE, 30);
    ::SendMessage(GetItem(IDC_SLIDER_HUE), TBM_SETPOS, TRUE, hue);
    SetItemInt(IDC_STATIC_HUE, hue);
    m_AMDDisplayHelper.SetColorHue(hue);
    m_cfg.SetInt(_T("config"), _T("hue"), hue);

    int gamma = 0;
    ::SendMessage(GetItem(IDC_SLIDER_GAMMA), TBM_SETRANGEMIN, TRUE, 0);
    ::SendMessage(GetItem(IDC_SLIDER_GAMMA), TBM_SETRANGEMAX, TRUE, 255);
    ::SendMessage(GetItem(IDC_SLIDER_GAMMA), TBM_SETPOS, TRUE, gamma);
    SetItemInt(IDC_STATIC_GAMMA, gamma);
    SetDisplayGamma(gamma);
    m_cfg.SetInt(_T("config"), _T("gamma"), gamma);

    return LRESULT(TRUE);
}

LRESULT CDialogFrame::OnCheckTemp(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd)
{
    bool fColorTemp = IsButtonChecked(wID);
    EnableWindow(GetItem(IDC_SLIDER_TEMP), fColorTemp);
    m_AMDDisplayHelper.SetColorTemperatureState(fColorTemp);

    return LRESULT(TRUE);
}

LRESULT CDialogFrame::OnCheckHDR(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd)
{
    if (IsButtonChecked(wID))
    {
        // 禁用开关: 当我的电脑使用电池运行时关闭HDR
        CPowerCfgUtils::SetVideoAdvancedColorQualityBias(&ALL_POWERSCHEMES_GUID, 1, 1);
        CPowerCfgUtils::SetActiveScheme(nullptr);
    }

    CDisplayTool::HDREnable(IsButtonChecked(wID));
    return LRESULT(TRUE);
}

bool CDialogFrame::SetDisplayGamma(uint8_t val)
{
    bool fResult = false;

    EnumDisplayMonitors(NULL, NULL, [](HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)->BOOL {
        if (0 == lprcMonitor->left && 0 == lprcMonitor->top)
        {
            uint8_t val = *(uint8_t*)dwData;
            HDC hDC = NULL;

            do
            {
                // 获取屏幕设备上下文（Device Context）
                hDC = ::GetDC(GetDesktopWindow());
                if (!hDC)
                {
                    break;
                }

                // 定义一个256色的数组，分别对应红、绿、蓝三个颜色通道
                WORD GammaArray[3][256];

                // 获取当前的伽马校正数组
                if (!::GetDeviceGammaRamp(hDC, GammaArray))
                {
                    break;
                }

                float gamma = (float)(128 - val) / 256.0f + 1.0f;
                for (int i = 0; i < 256; i++)
                {
                    // 归一化输入值（0-255 -> 0.0-1.0）
                    float normalizedValue = i / 255.0f;

                    // 计算伽马校正后的亮度值
                    float gammaCorrectedValue = pow(normalizedValue, gamma);

                    // 将亮度值映射到16位范围（0-65535）
                    WORD rampValue = static_cast<WORD>(gammaCorrectedValue * 65535);

                    // 设置红、绿、蓝三个通道的值
                    GammaArray[0][i] = GammaArray[1][i] = GammaArray[2][i] = rampValue;
                }

                // 设置新的伽马校正数组
                ::SetDeviceGammaRamp(hDC, GammaArray);

            } while (false);

            if (hDC)
            {
                ReleaseDC(NULL, hDC);
            }
        }

        return true;

        }, (LPARAM)& val);

    return fResult;
}

LRESULT CDialogFrame::OnWindowPosChinging(WPARAM wParam, LPARAM lParam)
{
    LPWINDOWPOS lpWindowPos = (LPWINDOWPOS)lParam;
    lpWindowPos->flags |= SWP_NOSIZE;
    return LRESULT(TRUE);
}

LRESULT CDialogFrame::OnClose(WPARAM wParam, LPARAM lParam)
{
    m_AMDDisplayHelper.Uninitialize();
    return (LRESULT)FALSE;
}

dlg_msg LRESULT CDialogFrame::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);
    POINT pt = { xPos, yPos };
    ClientToScreen(m_hWnd, &pt);
    CreatePopMenu(pt.x, pt.y, false);
    return LRESULT(TRUE);
}

LRESULT CDialogFrame::OnCommand(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd)
{
    LONG_PTR style = GetWindowLongPtr(GetItem(wID), GWL_STYLE);

    if ((BS_RADIOBUTTON == (BS_RADIOBUTTON & style)) || (BS_AUTORADIOBUTTON == (BS_AUTORADIOBUTTON & style)))
    {
        if (wNotify == 0)
        {
            CheckRadioButton(wStart, wEnd, wID);
        }
    }

    return (LRESULT)FALSE;
}

LRESULT CDialogFrame::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
    if (WM_LBUTTONDBLCLK == lParam)
    {
        ShowForeground(m_hWnd);
    }

    if (NULL != m_hHideWnd)
    {
        ::SetForegroundWindow(m_hHideWnd);
    }

    if (WM_RBUTTONUP == lParam)
    {
        HWND hForegroundWindow = GetForegroundWindow();
        if (hForegroundWindow == m_hHideWnd || hForegroundWindow == m_hWnd)
        {
            POINT pt = { 0 };
            ::GetCursorPos(&pt);
            CreatePopMenu(pt.x, pt.y, false);
        }
    }

    return FALSE;
}

bool CDialogFrame::ShowForeground(HWND hWnd)
{
    HWND hForeWnd = ::GetForegroundWindow();
    DWORD dwForeID = ::GetWindowThreadProcessId(hForeWnd, NULL);
    DWORD dwCurID = ::GetCurrentThreadId();
    ::AttachThreadInput(dwCurID, dwForeID, TRUE);
    BOOL isSuc = ::SetForegroundWindow(hWnd);
    ::AttachThreadInput(dwCurID, dwForeID, FALSE);

    if (isSuc)
    {
        //return true;
    }

    if (!::IsWindowVisible(hWnd))
    {
        ::ShowWindow(hWnd, SW_SHOW);
    }

    if (IsIconic(hWnd))
    {
        ::ShowWindow(hWnd, SW_SHOWNORMAL);
    }

    return ::SetForegroundWindow(hWnd);
}

void CDialogFrame::ShowTrayIcon()
{
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = m_hWnd;
    nid.uID = 0;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN));
    StringCchCopy(nid.szTip, _countof(nid.szTip), _T("Demo"));
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void CDialogFrame::DeleteTrayIcon()
{
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(nid);
    nid.hWnd = m_hWnd;
    nid.uID = 0;
    nid.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

VOID CDialogFrame::CreatePopMenu(int xPos, int yPos, bool isTop)
{
    MENUITEMINFO mii = { 0 };
    TCHAR szBuf[MAX_PATH] = { 0 };

    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_TYPE | MIIM_STATE | MIIM_SUBMENU | MIIM_ID;
    mii.fType = MFT_STRING | MFT_OWNERDRAW;
    mii.dwTypeData = szBuf;

    if (!m_hWnd)
    {
        return;
    }

    if (NULL == m_hPopMenu)
    {
        m_hPopMenu = CreatePopupMenu();
        int nCount = ::GetMenuItemCount(m_hMainMenu);
        for (int i = 0; i < nCount; i++)
        {
            mii.cch = _countof(szBuf);
            ::GetMenuItemInfo(m_hMainMenu, i, TRUE, &mii);
            ::InsertMenuItem(m_hPopMenu, i, TRUE, &mii);
        }
    }
    else
    {
        int nCount = ::GetMenuItemCount(m_hMainMenu);
        for (int i = 0; i < nCount; i++)
        {
            mii.cch = _countof(szBuf);
            ::GetMenuItemInfo(m_hMainMenu, i, TRUE, &mii);
            ::SetMenuItemInfo(m_hPopMenu, i, TRUE, &mii);
        }
    }

    TrackPopupMenuEx(m_hPopMenu, TPM_LEFTALIGN | (isTop ? TPM_BOTTOMALIGN : TPM_TOPALIGN),
        xPos, yPos, m_hWnd, NULL);
}
