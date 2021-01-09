#pragma once

#include <com_objects/com_tools.h>
#include <com_objects/drop_target_impl.h>
#include <panel/drop_action_params.h>

namespace smp::com
{

class HostDropTarget
    : public IDropTargetImpl
{
protected:
    virtual void FinalRelease();

public:
    /// @throw qwr::QwrException
    HostDropTarget( HWND hWnd );
    ~HostDropTarget() override = default;

    // IDropTarget
    HRESULT OnDragEnter( IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect ) override;
    HRESULT OnDragLeave() override;
    HRESULT OnDragOver( DWORD grfKeyState, POINTL pt, DWORD* pdwEffect ) override;
    HRESULT OnDrop( IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect ) override;

private:
    void SendDragMessage( DWORD msgId, DWORD grfKeyState, POINTL pt );

private:
    panel::DropActionParams actionParams_;
    IDataObjectPtr pDataObject_ = nullptr;
    DWORD m_fb2kAllowedEffect = DROPEFFECT_NONE;

    BEGIN_COM_QI_IMPL()
        COM_QI_ENTRY_MULTI( IUnknown, IDropTarget )
        COM_QI_ENTRY( IDropTarget )
    END_COM_QI_IMPL()
};

} // namespace smp::com
