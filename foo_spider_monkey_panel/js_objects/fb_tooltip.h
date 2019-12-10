#pragma once

#include <js_objects/object_base.h>
#include <utils/gdi_helpers.h>

#include <panel_tooltip_param.h>

#include <optional>
#include <memory>
#include <string>

class JSObject;
struct JSContext;
struct JSClass;

namespace mozjs
{

class JsFbTooltip
    : public JsObjectBase<JsFbTooltip>
{
public:
    static constexpr bool HasProto = true;
    static constexpr bool HasGlobalProto = false;
    static constexpr bool HasProxy = false;
    static constexpr bool HasPostCreate = false;

    static const JSClass JsClass;
    static const JSFunctionSpec* JsFunctions;
    static const JSPropertySpec* JsProperties;
    static const JsPrototypeId PrototypeId;

public:
    ~JsFbTooltip() override;

    static std::unique_ptr<JsFbTooltip> CreateNative( JSContext* cx, HWND hParentWnd, smp::panel::PanelTooltipParam& p_param_ptr );
    static size_t GetInternalSize( HWND hParentWnd, const smp::panel::PanelTooltipParam& p_param_ptr );

public:
    void Activate();
    void Deactivate();
    uint32_t GetDelayTime( uint32_t type );
    void SetDelayTime( uint32_t type, int32_t time );
    void SetMaxWidth( uint32_t width );
    void TrackPosition( int x, int y );

public:
    std::wstring get_Text();
    void put_Text( const std::wstring& text );
    void put_TrackActivate( bool activate );

private:
    JsFbTooltip( JSContext* cx, HWND hParentWnd, smp::panel::PanelTooltipParam& p_param_ptr );

private:
    [[maybe_unused]] JSContext* pJsCtx_ = nullptr;

    HWND hTooltipWnd_;
    HWND hParentWnd_;
    smp::panel::PanelTooltipParam& panelTooltipParam_;
    std::wstring tipBuffer_;
    smp::gdi::unique_gdi_ptr<HFONT> pFont_;
    std::unique_ptr<TOOLINFO> toolInfo_;
};

} // namespace mozjs
