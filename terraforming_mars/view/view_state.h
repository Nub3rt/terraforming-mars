#pragma once

#include "view_state.fwd.h"
#include "view.fwd.h"

namespace view
{
class ViewState
{
public:


protected:
    ViewState( View& view );

    View& _view;

};
}
