#pragma once

#include <config/parsed_panel_config.h>
#include <ui/ui_itab.h>

#include <resource.h>

#include <qwr/ui_ddx.h>
#include <qwr/ui_option.h>

#include <optional>
#include <vector>

namespace smp::panel
{
class js_panel_window;
}

namespace smp::ui
{

class CDialogConf
    : public CDialogImpl<CDialogConf>
{
public:
    enum class Tab : uint8_t
    {
        script,
        properties,
        package,
        def = script,
    };

public:
    enum
    {
        IDD = IDD_DIALOG_CONF
    };

    BEGIN_MSG_MAP( CDialogConf )
        MSG_WM_INITDIALOG( OnInitDialog )
        MSG_WM_PARENTNOTIFY( OnParentNotify )
        COMMAND_ID_HANDLER_EX( IDOK, OnCloseCmd )
        COMMAND_ID_HANDLER_EX( IDCANCEL, OnCloseCmd )
        COMMAND_ID_HANDLER_EX( IDAPPLY, OnCloseCmd )
        COMMAND_HANDLER_EX( IDC_EDIT_PANEL_NAME, EN_CHANGE, OnDdxUiChange )
        COMMAND_HANDLER_EX( IDC_BUTTON_EDIT_PANEL_NAME, BN_CLICKED, OnStartEditPanelName )
        COMMAND_HANDLER_EX( IDC_BUTTON_COMMIT_PANEL_NAME, BN_CLICKED, OnCommitPanelName )
        MESSAGE_HANDLER( WM_WINDOWPOSCHANGED, OnWindowPosChanged )
        NOTIFY_HANDLER_EX( IDC_TAB_CONF, TCN_SELCHANGE, OnSelectionChanged )
    END_MSG_MAP()

    // TODO: add help button

    CDialogConf( smp::panel::js_panel_window* pParent, CDialogConf::Tab tabId = CDialogConf::Tab::def );

    bool IsCleanSlate() const;

    void OnDataChanged();
    void OnScriptTypeChange();
    bool HasChanged();

    void Apply( bool savePackageData = true );
    void Revert();

    void SwitchTab( CDialogConf::Tab tabId );

private:
    BOOL OnInitDialog( HWND hwndFocus, LPARAM lParam );
    void OnDdxUiChange( UINT uNotifyCode, int nID, CWindow wndCtl );
    LRESULT OnCloseCmd( WORD wNotifyCode, WORD wID, HWND hWndCtl );
    void OnParentNotify( UINT message, UINT nChildID, LPARAM lParam );
    LRESULT OnWindowPosChanged( UINT, WPARAM, LPARAM lp, BOOL& bHandled );
    LRESULT OnSelectionChanged( LPNMHDR pNmhdr );
    void OnStartEditPanelName( UINT uNotifyCode, int nID, CWindow wndCtl );
    void OnCommitPanelName( UINT uNotifyCode, int nID, CWindow wndCtl );
    void DisablePanelNameControls();

    void DoFullDdxToUi();

    void OnDataChangedImpl( bool hasChanged = true );

    void InitializeTabData( CDialogConf::Tab tabId = CDialogConf::Tab::def );
    void ReinitializeTabData();
    void InitializeTabControls();
    void ReinitializeTabControls();
    void CreateChildTab();
    void DestroyChildTab();
    void SetTabIdx( CDialogConf::Tab tabId );

private:
    bool suppressUiDdx_ = true;

    panel::js_panel_window* pParent_ = nullptr;
    config::ParsedPanelSettings oldSettings_;
    config::ParsedPanelSettings localSettings_;

    config::PanelProperties oldProperties_;
    config::PanelProperties localProperties_;

    bool hasChanged_ = false;
    bool isCleanSlate_ = false;

    std::unique_ptr<qwr::ui::IUiDdx> panelNameDdx_;

    const CDialogConf::Tab startingTabId_;
    CTabCtrl cTabs_;
    CDialogImplBase* pcCurTab_ = nullptr;

    size_t activeTabIdx_ = 0;
    std::vector<std::unique_ptr<ITab>> tabs_;
};

} // namespace smp::ui
