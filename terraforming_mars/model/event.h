#pragma once

#include <functional>

namespace model
{
template<typename... Args>
class Event
{
public:
    using Callback = std::function<void( Args... )>;

    inline void SetCallback( Callback callback ) { _callback = callback; }
    void ClearCallback() { _callback = Callback(); }

    void Trigger( Args... params ) const {
        if ( _callback )
            _callback( params... );
    }

private:
    Callback _callback;
};
}
