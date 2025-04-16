#pragma once

#include "view_state.fwd.hpp"
#include "view.fwd.hpp"

#include <array>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "card_wrapper.hpp"
#include "tile_wrapper.hpp"
#include "view.hpp"

#include "../model/constants.hpp"
#include "../model/resource.hpp"
#include "../model/boards/tile_type.hpp"

namespace view
{
class ViewState
{
public:
    virtual ~ViewState();

    virtual void Enter();

    virtual void Update( float delta );
    virtual void Render();
    virtual void RenderGUI();

    virtual std::string GetEndButtonText();
    virtual CardWrapper::Visual GetCardUnderPlayLineVisual( CardWrapper* card );
    virtual CardWrapper::Visual GetCardOverPlayLineVisual( CardWrapper* card );

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

    CardWrapper::Visual GetCardUnderPlayLineVisual( CardWrapper* card ) override;
    CardWrapper::Visual GetCardOverPlayLineVisual( CardWrapper* card ) override;

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
    PaymentConfirmationVState( View& view, int amount, model::Resource resource, int resource_value );
    virtual ~PaymentConfirmationVState();

    void Enter() override;

    void RenderGUI() override;

    bool CanHoverHand() override;

protected:
    int _amount;
    std::string _resource;
    int _resource_value;

    int _min_credit;
    int _max_credit;
    int _min_resource;
    int _max_resource;
    int _current_credit;
    int _current_resource;

    static int CalculateResourceNeeded( int credit, int amount, int resource_value );
    static int CalculateCreditNeeded( int resource, int amount, int resource_value );
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
