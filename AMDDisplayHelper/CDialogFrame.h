#pragma once
#include "CWindow/CDialogBase.h"
#include "Win32Utils/CIniHelper.h"
#include "AMDDisplayHelper.h"

#define HELP_ABOUT_URL "https://gitee.com/flame_cyclone/nsf_information_editor"


#define CONFIG_INI_ROOT                                   _T("config")
#define CONFIG_INI_TOPMOST                                _T("settings_topmost")
#define CONFIG_INI_LANGUAGE                               _T("settings_language")

class CDialogFrame :
    public CDialogBase
{
public:
    LRESULT OnCommand(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd);

protected:

    dlg_msg LRESULT OnInitDialog(WPARAM wParam, LPARAM lParam);
    dlg_msg LRESULT OnClose(WPARAM wParam, LPARAM lParam);
    dlg_msg LRESULT OnRButtonUp(WPARAM wParam, LPARAM lParam);
    dlg_msg LRESULT OnWindowPosChinging(WPARAM wParam, LPARAM lParam);
    dlg_msg LRESULT OnHScroll(WPARAM wParam, LPARAM lParam);
    dlg_msg LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);

    dlg_command LRESULT OnDisplayPrimary(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd);
    dlg_command LRESULT OnDisplayMode(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd);
    dlg_command LRESULT OnDisplayPos(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd);
    dlg_command LRESULT OnButtonDefault(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd);
    dlg_command LRESULT OnButtonHDR(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd);
    dlg_command LRESULT OnCheckTemp(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd);
    dlg_command LRESULT OnCheckHDR(WORD wNotify, WORD wID, HWND hWnd, WORD wStart, WORD wEnd);
    dlg_command LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);

private:

    void UpdateDisplayStatus();
    bool SetDisplayGamma(uint8_t val);

    void ShowTrayIcon();
    void DeleteTrayIcon();
    VOID CreatePopMenu(int xPos, int yPos, bool isTop);
    bool ShowForeground(HWND hWnd);

private:
    HMENU                   m_hPopMenu = NULL;
    HMENU                   m_hMainMenu = NULL;
    HWND                    m_hHideWnd = NULL;

    AMDDisplayHelper        m_AMDDisplayHelper;
    LANGID                  m_langID;
    CIniHelper              m_cfg;
    _tstring                m_strPath;
    bool                    m_fBusy = false;

    DECLARE_DLG_MESSAGE_MAP()
};