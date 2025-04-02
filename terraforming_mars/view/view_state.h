#pragma once

#include "view_state.fwd.h"
#include "view.fwd.h"

#include "card_wrapper.h"
#include "view.h"

namespace view
{
class ViewState
{
public:
    virtual ~ViewState();

    virtual bool CanHoverHand();
    virtual bool CanDragCardOut( CardWrapper& card );
    virtual void PlayCard( CardWrapper& card );

protected:
    ViewState( View& view );

    View& _view;
};

class ResearchVState : public ViewState
{
public:
    ResearchVState( View& view );
    virtual ~ResearchVState();
};

class IdleVState : public ViewState
{
public:
    IdleVState( View& view );
    virtual ~IdleVState();

    virtual bool CanHoverHand();
    virtual bool CanDragCardOut( CardWrapper& card );
    virtual void PlayCard( CardWrapper& card );
};

class SellVState : public ViewState
{
public:
    SellVState( View& view );
    virtual ~SellVState();
};

class PlacementConfirmationVState : public ViewState
{
public:
    PlacementConfirmationVState( View& view );
    virtual ~PlacementConfirmationVState();

    virtual bool CanHoverHand();
};

class PaymentConfirmationVState : public ViewState
{
public:
    PaymentConfirmationVState( View& view );
    virtual ~PaymentConfirmationVState();
};

class PostLastGenerationVState : public ViewState
{
public:
    PostLastGenerationVState( View& view );
    virtual ~PostLastGenerationVState();
};

class GameOverVState : public ViewState
{
public:
    GameOverVState( View& view );
    virtual ~GameOverVState();
};
}
