#pragma once

#include "game_view.fwd.hpp"
#include "game_view_state.fwd.hpp"

#include <array>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "card_wrapper.hpp"
#include "panel.hpp"
#include "tile_wrapper.hpp"
#include "view.hpp"

#include "../model/constants.hpp"
#include "../model/resource.hpp"
#include "../model/boards/tile_type.hpp"

namespace view
{
class GameViewState
{
public:
    virtual ~GameViewState();

    virtual void Enter();

    virtual void Update( float delta );
    virtual void Render();
    virtual void RenderGUI();

    virtual std::string GetEndButtonText();
    virtual CardWrapper::Visual GetCardUnderPlayLineVisual( CardWrapper* card );
    virtual CardWrapper::Visual GetCardOverPlayLineVisual( CardWrapper* card );

    virtual bool CanClickMenuButton();
    virtual bool CanClickEndButton();
    virtual bool CanOpenPanels();
    virtual bool CanHoverHand();
    virtual bool CanDragCardsOut();
    virtual bool CanPlayCard( CardWrapper* card );
    virtual void PlayCard( int index_in_hand );
    virtual void ToggleToBuyCard( int index );

    virtual bool CanUseSellPatentsSP();
    virtual bool CanUsePowerPlantSP();
    virtual bool CanUseAsteroidSP();
    virtual bool CanUseAquiferSP();
    virtual bool CanUseGreenerySP();
    virtual bool CanUseCitySP();
    virtual bool CanConvertPlants();
    virtual bool CanConvertHeat();

    virtual bool InIdleState();

    void ClickedEndButton();
    virtual void ClickedOnTile( TileWrapper& tile );
    virtual void ClickedMisc( int index );

    virtual void Model_OnResearchConfirmed( std::array<bool, model::RESEARCH_CARD_NUM> selected );

protected:
    GameViewState( GameView& view );

    virtual void DoClickedEndButton();

    GameView& _view;
};

class ResearchState : public GameViewState
{
public:
    ResearchState( GameView& view, std::array<const model::decks::Card*, model::RESEARCH_CARD_NUM> cards );
    virtual ~ResearchState();

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

class IdleState : public GameViewState
{
public:
    IdleState( GameView& view );
    virtual ~IdleState();

    void Update( float delta ) override;

    CardWrapper::Visual GetCardUnderPlayLineVisual( CardWrapper* card ) override;
    CardWrapper::Visual GetCardOverPlayLineVisual( CardWrapper* card ) override;

    bool CanClickEndButton() override;
    bool CanHoverHand() override;
    bool CanDragCardsOut() override;
    bool CanPlayCard( CardWrapper* card ) override;
    void PlayCard( int index_in_hand ) override;

    bool CanUseSellPatentsSP() override;
    bool CanUsePowerPlantSP() override;
    bool CanUseAsteroidSP() override;
    bool CanUseAquiferSP() override;
    bool CanUseGreenerySP() override;
    bool CanUseCitySP() override;
    bool CanConvertPlants() override;
    bool CanConvertHeat() override;

    bool InIdleState() override;

    void ClickedMisc( int index ) override;

protected:
    void DoClickedEndButton() override;
};

class SellState : public GameViewState
{
public:
    SellState( GameView& view );
    virtual ~SellState();

    virtual void Enter();
    virtual void Render();

    virtual std::string GetEndButtonText();
    CardWrapper::Visual GetCardUnderPlayLineVisual( CardWrapper* card ) override;
    CardWrapper::Visual GetCardOverPlayLineVisual( CardWrapper* card ) override;

    bool CanClickEndButton() override;
    bool CanHoverHand() override;
    bool CanDragCardsOut() override;
    bool CanPlayCard( CardWrapper* card ) override;
    void PlayCard( int index_in_hand ) override;

    void DoClickedEndButton() override;
};

class PlacementConfirmationState : public GameViewState
{
public:
    PlacementConfirmationState( GameView& view, model::boards::TileType tile_type, std::vector<std::pair<int, int>> valid_positions );
    virtual ~PlacementConfirmationState();

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

class PaymentConfirmationState : public GameViewState
{
public:
    PaymentConfirmationState( GameView& view, int amount, model::Resource resource, int resource_value );
    virtual ~PaymentConfirmationState();

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

class PostLastGenerationState : public GameViewState
{
public:
    PostLastGenerationState( GameView& view );
    virtual ~PostLastGenerationState();
};

class GameOverState : public GameViewState
{
public:
    GameOverState( GameView& view );
    virtual ~GameOverState();

    bool CanHoverHand() override;
};
}
