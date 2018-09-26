#pragma once

#include <js_engine/js_gc.h>

#include <map>
#include <functional>
#include <mutex>

class js_panel_window;
struct JSContext;

namespace smp
{

class HeartbeatWindow;

}

namespace mozjs
{

class JsContainer;

class JsEngine final
{
public:
    ~JsEngine();
    JsEngine( const JsEngine& ) = delete;
    JsEngine& operator=( const JsEngine& ) = delete;

    static JsEngine& GetInstance();
    void PrepareForExit();

public:
    bool RegisterPanel( js_panel_window& parentPanel, JsContainer& jsContainer );
    void UnregisterPanel( js_panel_window& parentPanel );

public:
    void OnHeartbeat();

private:
    JsEngine();

private:
    bool Initialize();
    void Finalize();

    bool StartHeartbeatThread();
    void StopHeartbeatThread();

private:
    void MaybeRunJobs();

private:
    JSContext * pJsCtx_ = nullptr;

    bool isInitialized_ = false;
    bool shouldShutdown_ = false;

    std::map<HWND, std::reference_wrapper<JsContainer>> registeredContainers_;

    std::unique_ptr<smp::HeartbeatWindow> heartbeatWindow_;
    std::thread heartbeatThread_;
    std::atomic_bool shouldStopHeartbeatThread_ = false;    

    JsGc jsGc_;

    bool areJobsInProgress_ = false;
    uint32_t jobsStartTime_ = 0;
};

}
