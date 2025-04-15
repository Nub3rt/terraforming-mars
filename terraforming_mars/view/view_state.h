#pragma once

#include "view_state.fwd.h"
#include "view.fwd.h"

#include <array>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "card_wrapper.h"
#include "tile_wrapper.h"
#include "view.h"

#include "../model/constants.h"
#include "../model/boards/tile_type.h"

namespace view
{
class ViewState
{
public:
    virtual ~ViewState();

    virtual void Enter();

    virtual void Update( float delta );
    virtual void Render();

    virtual std::string GetEndButtonText();

    virtual bool CanClickEndButton();
    virtual bool CanHoverHand();
    virtual bool CanDragCardsOut();
    virtual bool CanPlayCard( CardWrapper* card );
    virtual void PlayCard( int index_in_hand );
    virtual void ToggleToBuyCard( int index );

    void ClickedEndButton();
    virtual void ClickedOnTile( TileWrapper& tile );

    virtual void Model_OnResearchConfirmed( std::array<bool, model::RESEARCH_CARD_NUM> selected );

protected:
    ViewState( View& view );

    virtual void DoClickedEndButton();

    View& _view;
};

class ResearchVState : public ViewState
{
public:
    ResearchVState( View& view, std::array<const model::decks::Card*, model::RESEARCH_CARD_NUM> cards );
    virtual ~ResearchVState();

    void Enter() override;

    void Update( float delta ) override;
    void Render() override;

    std::string GetEndButtonText() override;

    bool CanClickEndButton() override;
    bool CanHoverHand() override;

    void ToggleToBuyCard( int index ) override;

    void Model_OnResearchConfirmed( std::array<bool, model::RESEARCH_CARD_NUM> selected ) override;

protected:
    float _elapsed = 0.0f;
    bool _in_animation_over = false;

    std::array<CardWrapper*, model::RESEARCH_CARD_NUM> _cards;
    std::array<bool, model::RESEARCH_CARD_NUM> _to_buy;

    std::optional<std::vector<CardWrapper*>> _non_boughts;

    void DoClickedEndButton() override;
};

class IdleVState : public ViewState
{
public:
    IdleVState( View& view );
    virtual ~IdleVState();

    void Update( float delta ) override;

    bool CanClickEndButton() override;
    bool CanHoverHand() override;
    bool CanDragCardsOut() override;
    bool CanPlayCard( CardWrapper* card ) override;
    void PlayCard( int index_in_hand ) override;

protected:
    void DoClickedEndButton() override;
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
