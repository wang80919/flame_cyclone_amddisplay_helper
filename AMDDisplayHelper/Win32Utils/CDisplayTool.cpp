#include "CDisplayTool.h"
#include <vector>

namespace CDisplayTool
{
    LONG SetDisplayMode(UINT32 flags)
    {
        return ::SetDisplayConfig(0, NULL, 0, NULL, flags);
    }

    LONG SetInternalMode()
    {
        return SetDisplayMode(SDC_TOPOLOGY_INTERNAL | SDC_APPLY);
    }

    LONG SetCloneMode()
    {
        return SetDisplayMode(SDC_TOPOLOGY_CLONE | SDC_APPLY);
    }

    LONG SetExtendMode()
    {
        return SetDisplayMode(SDC_TOPOLOGY_EXTEND | SDC_APPLY);
    }

    LONG SetExternalMode()
    {
        return SetDisplayMode(SDC_TOPOLOGY_EXTERNAL | SDC_APPLY);
    }

    ePrimaryMonitor GetPrimaryMonitor()
    {
        std::vector<DISPLAYCONFIG_PATH_INFO> pathList;
        std::vector<DISPLAYCONFIG_MODE_INFO> modeList;
        UINT32 nPathCount = 0;
        UINT32 nModeInfoCount = 0;

        do
        {
            if (ERROR_SUCCESS != ::GetDisplayConfigBufferSizes(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                &nModeInfoCount)
                )
            {
                break;
            }

            pathList.resize(nPathCount);
            modeList.resize(nModeInfoCount);

            if (ERROR_SUCCESS != ::QueryDisplayConfig(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                pathList.data(),
                &nModeInfoCount,
                modeList.data(),
                nullptr)
                )
            {
                break;
            }

            RECT rtInternal = { 0 };
            RECT rtExternal = { 0 };
            int nInternal = 0;
            int nExternal = 0;

            DISPLAYCONFIG_MODE_INFO* pInternal = nullptr;
            DISPLAYCONFIG_MODE_INFO* pExternal = nullptr;
            DISPLAYCONFIG_ROTATION InternalRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;
            DISPLAYCONFIG_ROTATION ExternalRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;

            for (auto& path : pathList)
            {
                if (DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL == path.targetInfo.outputTechnology)
                {
                    for (auto& mode : modeList)
                    {
                        if (path.sourceInfo.id == mode.id && DISPLAYCONFIG_MODE_INFO_TYPE::DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE == mode.infoType)
                        {
                            rtInternal.left = mode.sourceMode.position.x;
                            rtInternal.top = mode.sourceMode.position.y;
                            rtInternal.right = rtInternal.left + mode.sourceMode.width;
                            rtInternal.bottom = rtInternal.top + mode.sourceMode.height;
                            pInternal = &mode;
                            InternalRotation = path.targetInfo.rotation;
                        }
                    }

                    nInternal++;
                }

                if (DISPLAYCONFIG_OUTPUT_TECHNOLOGY_DISPLAYPORT_EXTERNAL == path.targetInfo.outputTechnology)
                {
                    for (auto& mode : modeList)
                    {
                        if (path.sourceInfo.id == mode.id && DISPLAYCONFIG_MODE_INFO_TYPE::DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE == mode.infoType)
                        {
                            rtExternal.left = mode.sourceMode.position.x;
                            rtExternal.top = mode.sourceMode.position.y;
                            rtExternal.right = rtExternal.left + mode.sourceMode.width;
                            rtExternal.bottom = rtExternal.top + mode.sourceMode.height;
                            pExternal = &mode;
                            ExternalRotation = path.targetInfo.rotation;
                        }
                    }

                    nExternal++;
                }
            }

            if (nInternal && nExternal)
            {
                if (0 == rtInternal.left && 0 == rtInternal.top)
                {
                    return ePrimaryMonitor::eIntrenal;
                }

                if (0 == rtExternal.left && 0 == rtExternal.top)
                {
                    return ePrimaryMonitor::eExternal;
                }
            }

        } while (false);

        return ePrimaryMonitor::eIntrenal;
    }

    bool SetPrimaryMonitor(ePrimaryMonitor primaryMonitor)
    {
        std::vector<DISPLAYCONFIG_PATH_INFO> pathList;
        std::vector<DISPLAYCONFIG_MODE_INFO> modeList;
        UINT32 nPathCount = 0;
        UINT32 nModeInfoCount = 0;

        do
        {
            if (ERROR_SUCCESS != ::GetDisplayConfigBufferSizes(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                &nModeInfoCount)
                )
            {
                break;
            }

            pathList.resize(nPathCount);
            modeList.resize(nModeInfoCount);

            if (ERROR_SUCCESS != ::QueryDisplayConfig(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                pathList.data(),
                &nModeInfoCount,
                modeList.data(),
                nullptr)
                )
            {
                break;
            }

            RECT rtInternal = { 0 };
            RECT rtExternal = { 0 };
            int nInternal = 0;
            int nExternal = 0;

            DISPLAYCONFIG_MODE_INFO* pInternal = nullptr;
            DISPLAYCONFIG_MODE_INFO* pExternal = nullptr;
            DISPLAYCONFIG_ROTATION InternalRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;
            DISPLAYCONFIG_ROTATION ExternalRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;

            for (auto& path : pathList)
            {
                if (DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL == path.targetInfo.outputTechnology)
                {
                    for (auto& mode : modeList)
                    {
                        if (path.sourceInfo.id == mode.id && DISPLAYCONFIG_MODE_INFO_TYPE::DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE == mode.infoType)
                        {
                            rtInternal.left = mode.sourceMode.position.x;
                            rtInternal.top = mode.sourceMode.position.y;
                            rtInternal.right = rtInternal.left + mode.sourceMode.width;
                            rtInternal.bottom = rtInternal.top + mode.sourceMode.height;
                            pInternal = &mode;
                            InternalRotation = path.targetInfo.rotation;
                        }
                    }

                    nInternal++;
                }

                if (DISPLAYCONFIG_OUTPUT_TECHNOLOGY_DISPLAYPORT_EXTERNAL == path.targetInfo.outputTechnology)
                {
                    for (auto& mode : modeList)
                    {
                        if (path.sourceInfo.id == mode.id && DISPLAYCONFIG_MODE_INFO_TYPE::DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE == mode.infoType)
                        {
                            rtExternal.left = mode.sourceMode.position.x;
                            rtExternal.top = mode.sourceMode.position.y;
                            rtExternal.right = rtExternal.left + mode.sourceMode.width;
                            rtExternal.bottom = rtExternal.top + mode.sourceMode.height;
                            pExternal = &mode;
                            ExternalRotation = path.targetInfo.rotation;
                        }
                    }

                    nExternal++;
                }
            }

            if (!(nInternal && nExternal))
            {
                break;
            }

            if (pInternal && pExternal)
            {
                DISPLAYCONFIG_MODE_INFO* pPrimary = pInternal;
                DISPLAYCONFIG_MODE_INFO* pSecondary = pExternal;
                UINT32 PrimaryWidth = 0;
                UINT32 PrimaryHeight = 0;
                UINT32 SecondaryWidth = 0;
                UINT32 SecondaryHeight = 0;
                LPRECT pPrimaryRect = &rtInternal;
                LPRECT pSecondaryRect = &rtExternal;
                DISPLAYCONFIG_ROTATION PrimaryRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;
                DISPLAYCONFIG_ROTATION SecondaryRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;

                // 设置内置显示器为主显示器
                if (ePrimaryMonitor::eIntrenal == primaryMonitor)
                {
                    pPrimary = pInternal;
                    pSecondary = pExternal;
                    pPrimaryRect = &rtInternal;
                    pSecondaryRect = &rtExternal;
                    PrimaryRotation = InternalRotation;
                    SecondaryRotation = ExternalRotation;
                }

                // 设置外部显示器为主显示器
                if (ePrimaryMonitor::eExternal == primaryMonitor)
                {
                    pPrimary = pExternal;
                    pSecondary = pInternal;
                    pPrimaryRect = &rtExternal;
                    pSecondaryRect = &rtInternal;
                    PrimaryRotation = ExternalRotation;
                    SecondaryRotation = InternalRotation;
                }

                // 记录尺寸
                PrimaryWidth = pPrimary->sourceMode.width;
                PrimaryHeight = pPrimary->sourceMode.height;
                SecondaryWidth = pSecondary->sourceMode.width;
                SecondaryHeight = pSecondary->sourceMode.height;

                //设置主屏起始位置
                OffsetRect(pSecondaryRect, 0 - pPrimaryRect->left, 0 - pPrimaryRect->top);
                OffsetRect(pPrimaryRect, 0 - pPrimaryRect->left, 0 - pPrimaryRect->top);

                bool fPrimarySrcLandscape = false;
                bool fSecondaryShowLandscape = false;

                // 检查主显示器为横屏
                if (DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY == PrimaryRotation ||
                    DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE180 == PrimaryRotation
                    )
                {
                    if (PrimaryWidth > PrimaryHeight)
                    {
                        fPrimarySrcLandscape = true;
                    }
                    else
                    {
                        fPrimarySrcLandscape = false;
                    }
                }
                if (DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE90 == PrimaryRotation ||
                    DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE270 == PrimaryRotation
                    )
                {
                    if (PrimaryWidth < PrimaryHeight)
                    {
                        fPrimarySrcLandscape = true;
                    }
                    else
                    {
                        fPrimarySrcLandscape = false;
                    }
                }

                // 检查副显示器为横屏
                if (DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY == SecondaryRotation ||
                    DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE180 == SecondaryRotation
                    )
                {
                    if (SecondaryWidth > SecondaryHeight)
                    {
                        fSecondaryShowLandscape = true;
                    }
                    else
                    {
                        fSecondaryShowLandscape = false;
                    }
                }
                if (DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE90 == SecondaryRotation ||
                    DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE270 == SecondaryRotation
                    )
                {
                    if (SecondaryWidth < SecondaryHeight)
                    {
                        fSecondaryShowLandscape = true;
                    }
                    else
                    {
                        fSecondaryShowLandscape = false;
                    }
                }

                if (fPrimarySrcLandscape)
                {
                    if (PrimaryWidth < PrimaryHeight)
                    {
                        std::swap(PrimaryWidth, PrimaryHeight);
                    }
                }
                else
                {
                    if (PrimaryWidth > PrimaryHeight)
                    {
                        std::swap(PrimaryWidth, PrimaryHeight);
                    }
                }

                if (fSecondaryShowLandscape)
                {
                    if (SecondaryWidth < SecondaryHeight)
                    {
                        std::swap(SecondaryWidth, SecondaryHeight);
                    }
                }
                else
                {
                    if (SecondaryWidth > SecondaryHeight)
                    {
                        std::swap(SecondaryWidth, SecondaryHeight);
                    }
                }

                pPrimaryRect->right = pPrimaryRect->left + PrimaryWidth;
                pPrimaryRect->bottom = pPrimaryRect->top + PrimaryHeight;
                pSecondaryRect->right = pSecondaryRect->left + SecondaryWidth;
                pSecondaryRect->bottom = pSecondaryRect->top + SecondaryHeight;

                pPrimary->sourceMode.position.x = pPrimaryRect->left;
                pPrimary->sourceMode.position.y = pPrimaryRect->top;
                pSecondary->sourceMode.position.x = pSecondaryRect->left;
                pSecondary->sourceMode.position.y = pSecondaryRect->top;

                return ::SetDisplayConfig(
                    pathList.size(), pathList.data(),
                    modeList.size(), modeList.data(),
                    SDC_APPLY | SDC_USE_SUPPLIED_DISPLAY_CONFIG | SDC_SAVE_TO_DATABASE
                );
            }

        } while (false);

        return false;
    }

    eAlignType GetSecondaryMonitorPos()
    {
        std::vector<DISPLAYCONFIG_PATH_INFO> pathList;
        std::vector<DISPLAYCONFIG_MODE_INFO> modeList;
        UINT32 nPathCount = 0;
        UINT32 nModeInfoCount = 0;

        do
        {
            if (ERROR_SUCCESS != ::GetDisplayConfigBufferSizes(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                &nModeInfoCount)
                )
            {
                break;
            }

            pathList.resize(nPathCount);
            modeList.resize(nModeInfoCount);

            if (ERROR_SUCCESS != ::QueryDisplayConfig(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                pathList.data(),
                &nModeInfoCount,
                modeList.data(),
                nullptr)
                )
            {
                break;
            }

            RECT rtInternal = { 0 };
            RECT rtExternal = { 0 };
            int nInternal = 0;
            int nExternal = 0;

            DISPLAYCONFIG_MODE_INFO* pInternal = nullptr;
            DISPLAYCONFIG_MODE_INFO* pExternal = nullptr;
            DISPLAYCONFIG_ROTATION InternalRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;
            DISPLAYCONFIG_ROTATION ExternalRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;

            for (auto& path : pathList)
            {
                if (DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL == path.targetInfo.outputTechnology)
                {
                    for (auto& mode : modeList)
                    {
                        if (path.sourceInfo.id == mode.id && DISPLAYCONFIG_MODE_INFO_TYPE::DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE == mode.infoType)
                        {
                            rtInternal.left = mode.sourceMode.position.x;
                            rtInternal.top = mode.sourceMode.position.y;
                            rtInternal.right = rtInternal.left + mode.sourceMode.width;
                            rtInternal.bottom = rtInternal.top + mode.sourceMode.height;
                            pInternal = &mode;
                            InternalRotation = path.targetInfo.rotation;
                        }
                    }

                    nInternal++;
                }

                if (DISPLAYCONFIG_OUTPUT_TECHNOLOGY_DISPLAYPORT_EXTERNAL == path.targetInfo.outputTechnology)
                {
                    for (auto& mode : modeList)
                    {
                        if (path.sourceInfo.id == mode.id && DISPLAYCONFIG_MODE_INFO_TYPE::DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE == mode.infoType)
                        {
                            rtExternal.left = mode.sourceMode.position.x;
                            rtExternal.top = mode.sourceMode.position.y;
                            rtExternal.right = rtExternal.left + mode.sourceMode.width;
                            rtExternal.bottom = rtExternal.top + mode.sourceMode.height;
                            pExternal = &mode;
                            ExternalRotation = path.targetInfo.rotation;
                        }
                    }

                    nExternal++;
                }
            }

            if (!(nInternal && nExternal))
            {
                break;
            }

            if (pInternal && pExternal)
            {
                DISPLAYCONFIG_MODE_INFO* pPrimary = pInternal;
                DISPLAYCONFIG_MODE_INFO* pSecondary = pExternal;
                UINT32 PrimaryWidth = 0;
                UINT32 PrimaryHeight = 0;
                UINT32 SecondaryWidth = 0;
                UINT32 SecondaryHeight = 0;
                LPRECT pPrimaryRect = &rtInternal;
                LPRECT pSecondaryRect = &rtExternal;
                DISPLAYCONFIG_ROTATION PrimaryRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;
                DISPLAYCONFIG_ROTATION SecondaryRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;

                if (0 == rtInternal.left && 0 == rtInternal.top)
                {
                    pPrimary = pInternal;
                    pSecondary = pExternal;
                    pPrimaryRect = &rtInternal;
                    pSecondaryRect = &rtExternal;
                    PrimaryRotation = InternalRotation;
                    SecondaryRotation = ExternalRotation;
                }
                else
                {
                    pPrimary = pExternal;
                    pSecondary = pInternal;
                    pPrimaryRect = &rtExternal;
                    pSecondaryRect = &rtInternal;
                    PrimaryRotation = ExternalRotation;
                    SecondaryRotation = InternalRotation;
                }

                // 记录尺寸
                PrimaryWidth = pPrimary->sourceMode.width;
                PrimaryHeight = pPrimary->sourceMode.height;
                SecondaryWidth = pSecondary->sourceMode.width;
                SecondaryHeight = pSecondary->sourceMode.height;

                bool fPrimarySrcLandscape = false;
                bool fSecondaryShowLandscape = false;

                // 检查主显示器为横屏
                if (DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY == PrimaryRotation ||
                    DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE180 == PrimaryRotation
                    )
                {
                    if (PrimaryWidth > PrimaryHeight)
                    {
                        fPrimarySrcLandscape = true;
                    }
                    else
                    {
                        fPrimarySrcLandscape = false;
                    }
                }
                if (DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE90 == PrimaryRotation ||
                    DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE270 == PrimaryRotation
                    )
                {
                    if (PrimaryWidth < PrimaryHeight)
                    {
                        fPrimarySrcLandscape = true;
                    }
                    else
                    {
                        fPrimarySrcLandscape = false;
                    }
                }

                // 检查副显示器为横屏
                if (DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY == SecondaryRotation ||
                    DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE180 == SecondaryRotation
                    )
                {
                    if (SecondaryWidth > SecondaryHeight)
                    {
                        fSecondaryShowLandscape = true;
                    }
                    else
                    {
                        fSecondaryShowLandscape = false;
                    }
                }
                if (DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE90 == SecondaryRotation ||
                    DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE270 == SecondaryRotation
                    )
                {
                    if (SecondaryWidth < SecondaryHeight)
                    {
                        fSecondaryShowLandscape = true;
                    }
                    else
                    {
                        fSecondaryShowLandscape = false;
                    }
                }

                if (fPrimarySrcLandscape)
                {
                    if (PrimaryWidth < PrimaryHeight)
                    {
                        std::swap(PrimaryWidth, PrimaryHeight);
                    }
                }
                else
                {
                    if (PrimaryWidth > PrimaryHeight)
                    {
                        std::swap(PrimaryWidth, PrimaryHeight);
                    }
                }

                if (fSecondaryShowLandscape)
                {
                    if (SecondaryWidth < SecondaryHeight)
                    {
                        std::swap(SecondaryWidth, SecondaryHeight);
                    }
                }
                else
                {
                    if (SecondaryWidth > SecondaryHeight)
                    {
                        std::swap(SecondaryWidth, SecondaryHeight);
                    }
                }

                pPrimaryRect->right = pPrimaryRect->left + PrimaryWidth;
                pPrimaryRect->bottom = pPrimaryRect->top + PrimaryHeight;
                pSecondaryRect->right = pSecondaryRect->left + SecondaryWidth;
                pSecondaryRect->bottom = pSecondaryRect->top + SecondaryHeight;

                pPrimary->sourceMode.position.x = pPrimaryRect->left;
                pPrimary->sourceMode.position.y = pPrimaryRect->top;
                pSecondary->sourceMode.position.x = pSecondaryRect->left;
                pSecondary->sourceMode.position.y = pSecondaryRect->top;

                LONG PrimaryHCenter = pPrimaryRect->left + PrimaryWidth / 2;
                LONG PrimaryVCenter = pPrimaryRect->top + PrimaryHeight / 2;
                LONG SecondaryHCenter = pSecondaryRect->left + SecondaryWidth / 2;
                LONG SecondaryVCenter = pSecondaryRect->top + SecondaryHeight / 2;

                const int nPart = 16;

                // 左方
                if (pSecondaryRect->right == pPrimaryRect->left)
                {
                    int nOffset = PrimaryHeight / nPart;

                    if (SecondaryHeight < PrimaryHeight)
                    {
                        if (pSecondaryRect->top <= pPrimaryRect->top + nOffset)
                        {
                            return eAlignType::eLeftTop;
                        }
                        else if (pSecondaryRect->bottom >= pPrimaryRect->bottom - nOffset)
                        {
                            return eAlignType::eLeftBottom;
                        }
                        else
                        {
                            return eAlignType::eLeftCenter;
                        }
                    }
                    else
                    {
                        if (abs(SecondaryVCenter - PrimaryVCenter) <= nOffset)
                        {
                            return eAlignType::eLeftCenter;
                        }
                        else if (SecondaryVCenter - PrimaryVCenter < -nOffset)
                        {
                            return eAlignType::eLeftTop;
                        }
                        else
                        {
                            return eAlignType::eLeftBottom;
                        }
                    }

                }

                // 右方
                if (pSecondaryRect->left == pPrimaryRect->right)
                {
                    int nOffset = PrimaryHeight / nPart;

                    if (SecondaryHeight < PrimaryHeight)
                    {
                        if (pSecondaryRect->top <= pPrimaryRect->top + nOffset)
                        {
                            return eAlignType::eRightTop;
                        }
                        else if (pSecondaryRect->bottom >= pPrimaryRect->bottom - nOffset)
                        {
                            return eAlignType::eRightBottom;
                        }
                        else
                        {
                            return eAlignType::eRightCenter;
                        }
                    }
                    else
                    {
                        if (abs(SecondaryVCenter - PrimaryVCenter) <= nOffset)
                        {
                            return eAlignType::eRightCenter;
                        }
                        else if (SecondaryVCenter - PrimaryVCenter < -nOffset)
                        {
                            return eAlignType::eRightTop;
                        }
                        else
                        {
                            return eAlignType::eRightBottom;
                        }
                    }
                }

                // 顶部
                if (pSecondaryRect->bottom == pPrimaryRect->top)
                {
                    int nOffset = PrimaryWidth / nPart;

                    if (SecondaryWidth < PrimaryWidth)
                    {
                        if (pSecondaryRect->left <= pPrimaryRect->left + nOffset)
                        {
                            return eAlignType::eTopLeft;
                        }
                        else if (pSecondaryRect->right >= pPrimaryRect->right - nOffset)
                        {
                            return eAlignType::eTopRight;
                        }
                        else
                        {
                            return eAlignType::eTopCenter;
                        }
                    }
                    else
                    {
                        if (abs(SecondaryHCenter - PrimaryHCenter) <= nOffset)
                        {
                            return eAlignType::eTopCenter;
                        }
                        else if (SecondaryHCenter - PrimaryHCenter < -nOffset)
                        {
                            return eAlignType::eTopLeft;
                        }
                        else
                        {
                            return eAlignType::eTopRight;
                        }
                    }
                }

                // 底部
                if (pSecondaryRect->top == pPrimaryRect->bottom)
                {
                    int nOffset = PrimaryWidth / nPart;

                    if (SecondaryWidth < PrimaryWidth)
                    {
                        if (pSecondaryRect->left <= pPrimaryRect->left + nOffset)
                        {
                            return eAlignType::eBottomLeft;
                        }
                        else if (pSecondaryRect->right >= pPrimaryRect->right - nOffset)
                        {
                            return eAlignType::eBottomRight;
                        }
                        else
                        {
                            return eAlignType::eBottomCenter;
                        }
                    }
                    else
                    {
                        if (abs(SecondaryHCenter - PrimaryHCenter) <= nOffset)
                        {
                            return eAlignType::eBottomCenter;
                        }
                        else if (SecondaryHCenter - PrimaryHCenter < -nOffset)
                        {
                            return eAlignType::eBottomLeft;
                        }
                        else
                        {
                            return eAlignType::eBottomRight;
                        }
                    }
                }
            }

        } while (false);

        return eAlignType::eNone;
    }

    bool SetSecondaryMonitorPos(eAlignType hAlignType/* = eAlignType::eNone*/)
    {
        std::vector<DISPLAYCONFIG_PATH_INFO> pathList;
        std::vector<DISPLAYCONFIG_MODE_INFO> modeList;
        UINT32 nPathCount = 0;
        UINT32 nModeInfoCount = 0;

        do
        {
            if (ERROR_SUCCESS != ::GetDisplayConfigBufferSizes(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                &nModeInfoCount)
                )
            {
                break;
            }

            pathList.resize(nPathCount);
            modeList.resize(nModeInfoCount);

            if (ERROR_SUCCESS != ::QueryDisplayConfig(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                pathList.data(),
                &nModeInfoCount,
                modeList.data(),
                nullptr)
                )
            {
                break;
            }

            RECT rtInternal = { 0 };
            RECT rtExternal = { 0 };
            int nInternal = 0;
            int nExternal = 0;

            DISPLAYCONFIG_MODE_INFO* pInternal = nullptr;
            DISPLAYCONFIG_MODE_INFO* pExternal = nullptr;
            DISPLAYCONFIG_ROTATION InternalRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;
            DISPLAYCONFIG_ROTATION ExternalRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;

            for (auto& path : pathList)
            {
                if (DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL == path.targetInfo.outputTechnology)
                {
                    for (auto& mode : modeList)
                    {
                        if (path.sourceInfo.id == mode.id && DISPLAYCONFIG_MODE_INFO_TYPE::DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE == mode.infoType)
                        {
                            rtInternal.left = mode.sourceMode.position.x;
                            rtInternal.top = mode.sourceMode.position.y;
                            rtInternal.right = rtInternal.left + mode.sourceMode.width;
                            rtInternal.bottom = rtInternal.top + mode.sourceMode.height;
                            pInternal = &mode;
                            InternalRotation = path.targetInfo.rotation;
                        }
                    }

                    nInternal++;
                }

                if (DISPLAYCONFIG_OUTPUT_TECHNOLOGY_DISPLAYPORT_EXTERNAL == path.targetInfo.outputTechnology)
                {
                    for (auto& mode : modeList)
                    {
                        if (path.sourceInfo.id == mode.id && DISPLAYCONFIG_MODE_INFO_TYPE::DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE == mode.infoType)
                        {
                            rtExternal.left = mode.sourceMode.position.x;
                            rtExternal.top = mode.sourceMode.position.y;
                            rtExternal.right = rtExternal.left + mode.sourceMode.width;
                            rtExternal.bottom = rtExternal.top + mode.sourceMode.height;
                            pExternal = &mode;
                            ExternalRotation = path.targetInfo.rotation;
                        }
                    }

                    nExternal++;
                }
            }

            if (!(nInternal && nExternal))
            {
                break;
            }

            if (pInternal && pExternal)
            {
                DISPLAYCONFIG_MODE_INFO* pPrimary = pInternal;
                DISPLAYCONFIG_MODE_INFO* pSecondary = pExternal;
                UINT32 PrimaryWidth = 0;
                UINT32 PrimaryHeight = 0;
                UINT32 SecondaryWidth = 0;
                UINT32 SecondaryHeight = 0;
                LPRECT pPrimaryRect = &rtInternal;
                LPRECT pSecondaryRect = &rtExternal;
                DISPLAYCONFIG_ROTATION PrimaryRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;
                DISPLAYCONFIG_ROTATION SecondaryRotation = DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY;

                if (0 == rtInternal.left && 0 == rtInternal.top)
                {
                    pPrimary = pInternal;
                    pSecondary = pExternal;
                    pPrimaryRect = &rtInternal;
                    pSecondaryRect = &rtExternal;
                    PrimaryRotation = InternalRotation;
                    SecondaryRotation = ExternalRotation;
                }
                else
                {
                    pPrimary = pExternal;
                    pSecondary = pInternal;
                    pPrimaryRect = &rtExternal;
                    pSecondaryRect = &rtInternal;
                    PrimaryRotation = ExternalRotation;
                    SecondaryRotation = InternalRotation;
                }

                // 记录尺寸
                PrimaryWidth = pPrimary->sourceMode.width;
                PrimaryHeight = pPrimary->sourceMode.height;
                SecondaryWidth = pSecondary->sourceMode.width;
                SecondaryHeight = pSecondary->sourceMode.height;

                bool fPrimarySrcLandscape = false;
                bool fSecondaryShowLandscape = false;

                // 检查主显示器为横屏
                if (DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY == PrimaryRotation ||
                    DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE180 == PrimaryRotation
                    )
                {
                    if (PrimaryWidth > PrimaryHeight)
                    {
                        fPrimarySrcLandscape = true;
                    }
                    else
                    {
                        fPrimarySrcLandscape = false;
                    }
                }
                if (DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE90 == PrimaryRotation ||
                    DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE270 == PrimaryRotation
                    )
                {
                    if (PrimaryWidth < PrimaryHeight)
                    {
                        fPrimarySrcLandscape = true;
                    }
                    else
                    {
                        fPrimarySrcLandscape = false;
                    }
                }

                // 检查副显示器为横屏
                if (DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_IDENTITY == SecondaryRotation ||
                    DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE180 == SecondaryRotation
                    )
                {
                    if (SecondaryWidth > SecondaryHeight)
                    {
                        fSecondaryShowLandscape = true;
                    }
                    else
                    {
                        fSecondaryShowLandscape = false;
                    }
                }
                if (DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE90 == SecondaryRotation ||
                    DISPLAYCONFIG_ROTATION::DISPLAYCONFIG_ROTATION_ROTATE270 == SecondaryRotation
                    )
                {
                    if (SecondaryWidth < SecondaryHeight)
                    {
                        fSecondaryShowLandscape = true;
                    }
                    else
                    {
                        fSecondaryShowLandscape = false;
                    }
                }

                if (fPrimarySrcLandscape)
                {
                    if (PrimaryWidth < PrimaryHeight)
                    {
                        std::swap(PrimaryWidth, PrimaryHeight);
                    }
                }
                else
                {
                    if (PrimaryWidth > PrimaryHeight)
                    {
                        std::swap(PrimaryWidth, PrimaryHeight);
                    }
                }

                if (fSecondaryShowLandscape)
                {
                    if (SecondaryWidth < SecondaryHeight)
                    {
                        std::swap(SecondaryWidth, SecondaryHeight);
                    }
                }
                else
                {
                    if (SecondaryWidth > SecondaryHeight)
                    {
                        std::swap(SecondaryWidth, SecondaryHeight);
                    }
                }

                pPrimaryRect->right = pPrimaryRect->left + PrimaryWidth;
                pPrimaryRect->bottom = pPrimaryRect->top + PrimaryHeight;
                pSecondaryRect->right = pSecondaryRect->left + SecondaryWidth;
                pSecondaryRect->bottom = pSecondaryRect->top + SecondaryHeight;

                // 对齐方式
                switch (hAlignType)
                {
                case eNone:
                {
                }
                break;
                case eLeftTop:
                {
                    pSecondaryRect->right = pPrimaryRect->left;
                    pSecondaryRect->left = pSecondaryRect->right - SecondaryWidth;

                    pSecondaryRect->top = pPrimaryRect->top;
                    pSecondaryRect->bottom = pSecondaryRect->top + SecondaryHeight;
                }
                break;
                case eLeftCenter:
                {
                    pSecondaryRect->right = pPrimaryRect->left;
                    pSecondaryRect->left = pSecondaryRect->right - SecondaryWidth;

                    pSecondaryRect->top = PrimaryHeight / 2 - SecondaryHeight / 2;
                    pSecondaryRect->bottom = pSecondaryRect->top + SecondaryHeight;
                }
                break;
                case eLeftBottom:
                {
                    pSecondaryRect->right = pPrimaryRect->left;
                    pSecondaryRect->left = pSecondaryRect->right - SecondaryWidth;

                    pSecondaryRect->bottom = pPrimaryRect->bottom;
                    pSecondaryRect->top = pSecondaryRect->bottom - SecondaryHeight;
                }
                break;
                case eTopLeft:
                {
                    pSecondaryRect->bottom = pPrimaryRect->top;
                    pSecondaryRect->top = pSecondaryRect->bottom - SecondaryHeight;

                    pSecondaryRect->left = pPrimaryRect->left;
                    pSecondaryRect->right = pSecondaryRect->left + SecondaryWidth;
                }
                break;
                case eTopCenter:
                {
                    pSecondaryRect->bottom = pPrimaryRect->top;
                    pSecondaryRect->top = pSecondaryRect->bottom - SecondaryHeight;

                    pSecondaryRect->left = PrimaryWidth / 2 - SecondaryWidth / 2;
                    pSecondaryRect->right = pSecondaryRect->left + SecondaryWidth;
                }
                break;
                case eTopRight:
                {
                    pSecondaryRect->bottom = pPrimaryRect->top;
                    pSecondaryRect->top = pSecondaryRect->bottom - SecondaryHeight;

                    pSecondaryRect->right = pPrimaryRect->right;
                    pSecondaryRect->left = pSecondaryRect->right - SecondaryWidth;
                }
                break;
                case eRightTop:
                {
                    pSecondaryRect->left = pPrimaryRect->right;
                    pSecondaryRect->right = pSecondaryRect->left + SecondaryWidth;

                    pSecondaryRect->top = pPrimaryRect->top;
                    pSecondaryRect->bottom = pSecondaryRect->top + SecondaryHeight;

                }
                break;
                case eRightCenter:
                {
                    pSecondaryRect->left = pPrimaryRect->right;
                    pSecondaryRect->right = pSecondaryRect->left + SecondaryWidth;

                    pSecondaryRect->top = PrimaryHeight / 2 - SecondaryHeight / 2;
                    pSecondaryRect->bottom = pSecondaryRect->top + SecondaryHeight;
                }
                break;
                case eRightBottom:
                {
                    pSecondaryRect->left = pPrimaryRect->right;
                    pSecondaryRect->right = pSecondaryRect->left + SecondaryWidth;

                    pSecondaryRect->bottom = pPrimaryRect->bottom;
                    pSecondaryRect->top = pSecondaryRect->bottom - SecondaryHeight;
                }
                break;
                case eBottomLeft:
                {
                    pSecondaryRect->top = pPrimaryRect->bottom;
                    pSecondaryRect->bottom = pPrimaryRect->top + SecondaryHeight;

                    pSecondaryRect->left = pPrimaryRect->left;
                    pSecondaryRect->right = pSecondaryRect->left + SecondaryWidth;
                }
                break;
                case eBottomCenter:
                {
                    pSecondaryRect->top = pPrimaryRect->bottom;
                    pSecondaryRect->bottom = pPrimaryRect->top + SecondaryHeight;

                    pSecondaryRect->left = PrimaryWidth / 2 - SecondaryWidth / 2;
                    pSecondaryRect->right = pSecondaryRect->left + SecondaryWidth;
                }
                break;
                case eBottomRight:
                {
                    pSecondaryRect->top = pPrimaryRect->bottom;
                    pSecondaryRect->bottom = pPrimaryRect->top + SecondaryHeight;

                    pSecondaryRect->right = pPrimaryRect->right;
                    pSecondaryRect->left = pSecondaryRect->right - SecondaryWidth;
                }
                break;
                }

                pPrimary->sourceMode.position.x = pPrimaryRect->left;
                pPrimary->sourceMode.position.y = pPrimaryRect->top;
                pSecondary->sourceMode.position.x = pSecondaryRect->left;
                pSecondary->sourceMode.position.y = pSecondaryRect->top;

                return ::SetDisplayConfig(
                    pathList.size(), pathList.data(), 
                    modeList.size(), modeList.data(), 
                    SDC_APPLY | SDC_USE_SUPPLIED_DISPLAY_CONFIG | SDC_SAVE_TO_DATABASE
                );
            }

        } while (false);

        return false;
    }

    DWORD GetDisplayMode()
    {
        std::vector<DISPLAY_CONFIG_INFO> displayConfig = GetDisplayConfigList();
        RECT rtInternal = { 0 };
        RECT rtExternal = { 0 };
        int nInternal = 0;
        int nExternal = 0;

        for (auto& item : displayConfig)
        {
            if (DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL == item.pathList.targetInfo.outputTechnology )
            {
                for (auto& mode : item.modeList)
                {
                    if (DISPLAYCONFIG_MODE_INFO_TYPE::DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE == mode.infoType)
                    {
                        rtInternal.left = mode.sourceMode.position.x;
                        rtInternal.top = mode.sourceMode.position.y;
                        rtInternal.right = rtInternal.left + mode.sourceMode.width;
                        rtInternal.bottom = rtInternal.top + mode.sourceMode.height;
                    }
                }

                nInternal++;
            }

            if (DISPLAYCONFIG_OUTPUT_TECHNOLOGY_DISPLAYPORT_EXTERNAL == item.pathList.targetInfo.outputTechnology)
            {
                for (auto& mode : item.modeList)
                {
                    if (DISPLAYCONFIG_MODE_INFO_TYPE::DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE == mode.infoType)
                    {
                        rtExternal.left = mode.sourceMode.position.x;
                        rtExternal.top = mode.sourceMode.position.y;
                        rtExternal.right = rtExternal.left + mode.sourceMode.width;
                        rtExternal.bottom = rtExternal.top + mode.sourceMode.height;
                    }
                }
                nExternal++;
            }
        }

        if (nInternal && !nExternal)
        {
            return SDC_TOPOLOGY_INTERNAL;
        }

        if (!nInternal && nExternal)
        {
            return SDC_TOPOLOGY_EXTERNAL;
        }

        if (nInternal && nExternal)
        {
            if (0 == memcmp(&rtInternal, &rtExternal, sizeof(RECT)))
            {
                return SDC_TOPOLOGY_CLONE;
            }

            return SDC_TOPOLOGY_EXTEND;
        }

        return 0;
    }

    LONG IsInternalMode()
    {
        return SDC_TOPOLOGY_INTERNAL == GetDisplayMode();
    }

    LONG IsCloneMode()
    {
        return SDC_TOPOLOGY_CLONE == GetDisplayMode();
    }

    LONG IsExtendMode()
    {
        return SDC_TOPOLOGY_EXTEND == GetDisplayMode();
    }

    LONG IsExternalMode()
    {
        return SDC_TOPOLOGY_EXTERNAL == GetDisplayMode();
    }

    std::vector<DISPLAY_CONFIG_INFO> GetDisplayConfigList()
    {
        std::vector<DISPLAY_CONFIG_INFO> result;
        std::vector<DISPLAYCONFIG_PATH_INFO> pathList;
        std::vector<DISPLAYCONFIG_MODE_INFO> modeList;
        UINT32 nPathCount = 0;
        UINT32 nModeInfoCount = 0;

        do
        {
            // 获取路径和模式的数量
            if (ERROR_SUCCESS != ::GetDisplayConfigBufferSizes(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                &nModeInfoCount)
                )
            {
                break;
            }

            pathList.resize(nPathCount);
            modeList.resize(nModeInfoCount);

            // 查询显示配置
            if (ERROR_SUCCESS != ::QueryDisplayConfig(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                pathList.data(),
                &nModeInfoCount,
                modeList.data(),
                nullptr)
                )
            {
                break;
            }

            for (auto& path : pathList)
            {
                DISPLAY_CONFIG_INFO info;
                info.pathList = path;

                for (auto& mode : modeList)
                {
                    if (DISPLAYCONFIG_MODE_INFO_TYPE::DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE == mode.infoType)
                    {
                        if (path.sourceInfo.id == mode.id)
                        {
                            info.modeList.push_back(mode);
                        }
                    }

                    if (DISPLAYCONFIG_MODE_INFO_TYPE::DISPLAYCONFIG_MODE_INFO_TYPE_TARGET == mode.infoType)
                    {
                        if (path.targetInfo.id == mode.id)
                        {
                            info.modeList.push_back(mode);
                        }
                    }
                }

                result.push_back(info);
            }

        } while (false);

        return result;
    }

    bool IsHDRSupported()
    {
        std::vector<DISPLAYCONFIG_PATH_INFO> pathList;
        std::vector<DISPLAYCONFIG_MODE_INFO> modeList;
        UINT32 nPathCount = 0;
        UINT32 nModeInfoCount = 0;

        do
        {
            // 获取路径和模式的数量
            if (ERROR_SUCCESS != ::GetDisplayConfigBufferSizes(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                &nModeInfoCount)
                )
            {
                break;
            }

            pathList.resize(nPathCount);
            modeList.resize(nModeInfoCount);

            // 查询显示配置
            if (ERROR_SUCCESS != ::QueryDisplayConfig(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                pathList.data(),
                &nModeInfoCount,
                modeList.data(),
                nullptr)
                )
            {
                break;
            }

            // 遍历每个路径，检查 HDR 支持
            for (UINT32 i = 0; i < nPathCount; i++) {
                DISPLAYCONFIG_GET_ADVANCED_COLOR_INFO colorInfo = {};
                colorInfo.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_ADVANCED_COLOR_INFO;
                colorInfo.header.size = sizeof(colorInfo);
                colorInfo.header.adapterId = pathList[i].targetInfo.adapterId;
                colorInfo.header.id = pathList[i].targetInfo.id;

                // 获取高级颜色信息
                if (::DisplayConfigGetDeviceInfo(&colorInfo.header) == ERROR_SUCCESS)
                {
                    if (colorInfo.advancedColorSupported)
                    {
                        return true;
                    }
                }
            }

        } while (false);

        return false;
    }

    bool IsHDREnabled()
    {
        std::vector<DISPLAYCONFIG_PATH_INFO> pathList;
        std::vector<DISPLAYCONFIG_MODE_INFO> modeList;
        UINT32 nPathCount = 0;
        UINT32 nModeInfoCount = 0;

        do
        {
            // 获取路径和模式的数量
            if (ERROR_SUCCESS != ::GetDisplayConfigBufferSizes(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                &nModeInfoCount)
                )
            {
                break;
            }

            pathList.resize(nPathCount);
            modeList.resize(nModeInfoCount);

            // 查询显示配置
            if (ERROR_SUCCESS != ::QueryDisplayConfig(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                pathList.data(),
                &nModeInfoCount,
                modeList.data(),
                nullptr)
                )
            {
                break;
            }

            // 遍历每个路径，检查 HDR 支持
            for (UINT32 i = 0; i < nPathCount; i++) {
                DISPLAYCONFIG_GET_ADVANCED_COLOR_INFO colorInfo = {};
                colorInfo.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_ADVANCED_COLOR_INFO;
                colorInfo.header.size = sizeof(colorInfo);
                colorInfo.header.adapterId = pathList[i].targetInfo.adapterId;
                colorInfo.header.id = pathList[i].targetInfo.id;

                // 获取高级颜色信息
                if (ERROR_SUCCESS == ::DisplayConfigGetDeviceInfo(&colorInfo.header))
                {
                    if (colorInfo.advancedColorSupported)
                    {
                        return colorInfo.advancedColorEnabled;
                    }
                }
            }

        } while (false);

        return false;
    }

    bool HDREnable(bool fEnable)
    {
        std::vector<DISPLAYCONFIG_PATH_INFO> pathList;
        std::vector<DISPLAYCONFIG_MODE_INFO> modeList;
        UINT32 nPathCount = 0;
        UINT32 nModeInfoCount = 0;

        do
        {
            // 获取路径和模式的数量
            if (ERROR_SUCCESS != ::GetDisplayConfigBufferSizes(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                &nModeInfoCount)
                )
            {
                break;
            }

            pathList.resize(nPathCount);
            modeList.resize(nModeInfoCount);

            // 查询显示配置
            if (ERROR_SUCCESS != ::QueryDisplayConfig(
                QDC_ONLY_ACTIVE_PATHS,
                &nPathCount,
                pathList.data(),
                &nModeInfoCount,
                modeList.data(),
                nullptr)
                )
            {
                break;
            }

            // 遍历每个路径，检查 HDR 支持
            for (UINT32 i = 0; i < nPathCount; i++) {
                DISPLAYCONFIG_GET_ADVANCED_COLOR_INFO colorInfo = {};
                colorInfo.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_ADVANCED_COLOR_INFO;
                colorInfo.header.size = sizeof(colorInfo);
                colorInfo.header.adapterId = pathList[i].targetInfo.adapterId;
                colorInfo.header.id = pathList[i].targetInfo.id;

                // 获取高级颜色信息
                if (ERROR_SUCCESS == ::DisplayConfigGetDeviceInfo(&colorInfo.header))
                {
                    if (colorInfo.advancedColorSupported)
                    {
                        DISPLAYCONFIG_SET_ADVANCED_COLOR_STATE setColorState = {};
                        setColorState.header.type = DISPLAYCONFIG_DEVICE_INFO_SET_ADVANCED_COLOR_STATE;
                        setColorState.header.size = sizeof(setColorState);
                        setColorState.header.adapterId = pathList[i].targetInfo.adapterId;
                        setColorState.header.id = pathList[i].targetInfo.id;
                        setColorState.enableAdvancedColor = fEnable;
                        LONG lStatus = ::DisplayConfigSetDeviceInfo(&setColorState.header);
                        return ERROR_SUCCESS == lStatus;
                    }
                }
            }

        } while (false);

        return false;
    }
}