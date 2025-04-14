#pragma once

#include "view_state.fwd.h"
#include "view.fwd.h"

#include <utility>
#include <vector>

#include "card_wrapper.h"
#include "tile_wrapper.h"
#include "view.h"

#include "../model/boards/tile_type.h"

namespace view
{
class ViewState
{
public:
    virtual ~ViewState();

    virtual void Enter();
    virtual void Render();

    virtual bool CanHoverHand();
    virtual bool CanDragCardOut( CardWrapper* card );
    virtual void PlayCard( int index_in_hand );

    virtual void ClickedOnTile( TileWrapper& tile );

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

    bool CanDragCardOut( CardWrapper* card ) override;
    void PlayCard( int index_in_hand ) override;
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
    PlacementConfirmationVState( View& view, model::boards::TileType tile_type, std::vector<std::pair<int, int>> valid_positions );
    virtual ~PlacementConfirmationVState();

    void Enter() override;
    void Render() override;

    bool CanHoverHand() override;

    void ClickedOnTile( TileWrapper& tile ) override;

protected:
    model::boards::TileType _tile_type;
    std::vector<std::pair<int, int>> _valid_positions;

    void ColorBordersIdle();
    void ColorBordersSelectable();
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
