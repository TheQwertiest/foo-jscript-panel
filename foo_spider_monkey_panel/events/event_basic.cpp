#include <stdafx.h>

#include "event_basic.h"

#include <panel/js_panel_window.h>

namespace smp
{

Event_Basic::Event_Basic( EventId id )
    : EventBase( id )
{
}

void Event_Basic::Run()
{
    assert( pTarget_ );

    auto pPanel = pTarget_->GetPanel();
    if ( pPanel )
    {
        pPanel->ExecuteTask( id_ );
    }
}

} // namespace smp
