#include "game_view_state.hpp"

#include <format>
#include <stdexcept>
#include <utility>

#include <imgui.h>

#include "animation.hpp"
#include "card_wrapper.hpp"
#include "constants.hpp"
#include "panel.hpp"
#include "text_renderer.hpp"
#include "tile_wrapper.hpp"
#include "game_view.hpp"

#include "../model/decks/card.hpp"
#include "../model/decks/active_card_with_action.hpp"
#include "../model/decks/availability.hpp"
#include "../model/boards/tile_type.hpp"

namespace view
{
#pragma region GameViewState

GameViewState::GameViewState( GameView& view ) : _view( view ) {}
GameViewState::~GameViewState() {}

void GameViewState::Enter() {}

void GameViewState::Update( float delta ) {}
void GameViewState::Render() {}
void GameViewState::RenderGUI() {}

std::string GameViewState::GetEndButtonText() { return "End Generation"; }
CardWrapper::Visual GameViewState::GetCardUnderPlayLineVisual( CardWrapper* card ) { throw std::logic_error( "GameViewState::GetCardUnderPlayLineVisual: card drag is not supported in this state!" ); }
CardWrapper::Visual GameViewState::GetCardOverPlayLineVisual( CardWrapper* card ) { throw std::logic_error( "GameViewState::GetCardOverPlayLineVisual: card drag is not supported in this state!" ); }

bool GameViewState::CanClickMenuButton() { return true; }
bool GameViewState::CanClickEndButton() { return false; }
bool GameViewState::CanOpenPanels() { return true; }
bool GameViewState::CanHoverHand() { return false; }
bool GameViewState::CanDragCardsOut() { return false; }
bool GameViewState::CanPlayCard( CardWrapper* card ) { return false; }

bool GameViewState::CanUseSellPatentsSP() { return false; }
bool GameViewState::CanUsePowerPlantSP() { return false; }
bool GameViewState::CanUseAsteroidSP() { return false; }
bool GameViewState::CanUseAquiferSP() { return false; }
bool GameViewState::CanUseGreenerySP() { return false; }
bool GameViewState::CanUseCitySP() { return false; }
bool GameViewState::CanConvertPlants() { return false; }
bool GameViewState::CanConvertHeat() { return false; }

void GameViewState::PlayCard( int index_in_hand ) {
    throw std::logic_error( "GameViewState::PlayCard: GameView was in an invalid state!" );
}

void GameViewState::ToggleToBuyCard( int index ) {
    throw std::logic_error( "GameViewState::ToggleToBuyCard: GameView was in an invalid state!" );
}

void GameViewState::ClickedEndButton() {
    if ( !CanClickEndButton() )
        throw std::logic_error( "GameViewState::ClickedEndButton: button could not be clicked in this state!" );

    DoClickedEndButton();
}

void GameViewState::ClickedOnTile( TileWrapper& tile ) {}
void GameViewState::ClickedMisc( int index ) {}

void GameViewState::Model_OnResearchConfirmed( std::array<bool, model::RESEARCH_CARD_NUM> selected ) {
    throw std::logic_error( "GameViewState::Model_OnResearchConfirmed: GameView was in an invalid state!" );
}

void GameViewState::DoClickedEndButton() {}

#pragma endregion GameViewState

#pragma region ResearchState

ResearchVState::ResearchVState( GameView& view, std::array<const model::decks::Card*, model::RESEARCH_CARD_NUM> cards )
    : GameViewState( view ), _cards(), _to_buy() {
    for ( int i = 0; i < model::RESEARCH_CARD_NUM; ++i ) {
        _cards[ i ] = new CardWrapper( cards[ i ] );
        _to_buy[ i ] = true;
    }
}
ResearchVState::~ResearchVState() {
    if ( !_non_boughts ) {
        for ( CardWrapper* card : _cards )
            delete card;
    } else {
        for ( CardWrapper* card : *_non_boughts )
            delete card;
    }
}

void ResearchVState::Enter() {
    static const float spacing = 0.35f;
    static const float length = spacing * (model::RESEARCH_CARD_NUM - 1);
    static const float start_x = 0.0f - length / 2.0f;
    static const float y = 0.0f;

    _view._generation = _view._model->get_generation();

    for ( int i = 0; i < model::RESEARCH_CARD_NUM; ++i ) {
        glm::vec2 start_pos = CARD_DRAW_POS_START;
        start_pos.x += spacing * i;
        _cards[ i ]->pos.SetAnim( start_pos, glm::vec2( start_x + spacing * i, y ), CARD_DRAW_IN_DURATION );
        _cards[ i ]->scale.SetAnim( RESEARCH_CARD_SCALE, RESEARCH_CARD_SCALE, CARD_DRAW_IN_DURATION );
        _cards[ i ]->rotate.SetAnim( CARD_DRAW_ROTATE_START, RESEARCH_CARD_ROTATE, CARD_DRAW_IN_DURATION );
    }

    for ( CardWrapper* card : _cards ) {
        card->state = CardWrapper::State::DRAWING;
        card->visual = CardWrapper::Visual::NONE;
        card->SetEase( glm::quarticEaseOut<float> );
    }
}

void ResearchVState::Update( float delta ) {
    _elapsed += delta;
    static const float delay = 0.5f;

    if ( !_non_boughts ) {
        for ( int i = 0; i < model::RESEARCH_CARD_NUM; ++i ) {
            if ( _elapsed >= i * delay )
                _cards[ i ]->Update( delta );
        }

        if ( !_in_animation_over && _elapsed >= CARD_DRAW_IN_DURATION + model::RESEARCH_CARD_NUM * delay ) {
            _in_animation_over = true;
            for ( CardWrapper* card : _cards )
                card->visual = CardWrapper::Visual::ACTION_HIGHLIGHT;
        }
    } else {
        for ( CardWrapper* card : *_non_boughts )
            card->Update( delta );

        if ( _elapsed > CARD_DRAW_DOWN_DURATION ) {
            for ( CardWrapper* card : _cards ) {
                card->state = CardWrapper::State::IDLE;
                card->SetDefaultEase();
            }

            _view.RequestInstantStateChange( _view.CreateIdleState() );
        }
    }
}

void ResearchVState::Render() {
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glUseProgram( _view._program_card_id );
    glBindVertexArray( _view._rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, _view._cards_texture.id );
    glUniform1i( ul( "image" ), 0 );

    for ( int i = 0; i < model::RESEARCH_CARD_NUM; ++i ) {
        if ( _in_animation_over && !_non_boughts )
            _view.SetStencilRef( STENCIL_STARTING_RESEARCH + i );

        _view.RenderCard( *_cards[ i ], -i );

        if ( _in_animation_over && !_non_boughts )
            _view.SetStencilRef();
    }

    glBindTexture( GL_TEXTURE_2D, 0 );

    glBindVertexArray( 0 );
    glUseProgram( 0 );

    glDisable( GL_BLEND );

    if ( _in_animation_over && !_non_boughts )
        TextRenderer::RenderTextCentered(
            std::format( "Current Research Cost: {}", _view._model->GetTotalCost() ),
            BASE_HINT_POS.x, BASE_HINT_POS.y,
            BASE_TEXT_SCALE,
            LIGHT_TEXT_COLOR
        );
}

std::string ResearchVState::GetEndButtonText() {
    return "Confirm";
}

bool ResearchVState::CanClickEndButton() { return _in_animation_over && !_non_boughts; }
bool ResearchVState::CanHoverHand() { return true; }

void ResearchVState::ToggleToBuyCard( int index ) {
    _view._model->ToggleToBuyCard( index );
    _to_buy[ index ] = !_to_buy[ index ];
    _cards[ index ]->visual = _to_buy[index] ? CardWrapper::Visual::ACTION_HIGHLIGHT
                                             : CardWrapper::Visual::FADED;
}

void ResearchVState::Model_OnResearchConfirmed( std::array<bool, model::RESEARCH_CARD_NUM> selected ) {
    _non_boughts = std::vector<CardWrapper*>();
    size_t index = _view._hand.size();

    for ( int i = 0; i < model::RESEARCH_CARD_NUM; ++i ) {
        CardWrapper* card = _cards[ i ];
        card->visual = CardWrapper::Visual::NONE;
        card->SetEase( glm::quarticEaseIn<float> );

        if ( selected[ i ] ) {
            _view._hand.push_back( card );
        } else {
            card->pos.y.UpdateAnim( CARD_DRAW_UP_Y, CARD_DRAW_DOWN_DURATION );
            _non_boughts->push_back( card );
        }
    }

    _view.RefreshHandPositions();
    while ( index < _view._hand.size() ) {
        _view._hand[ index ]->GoToBase( CARD_DRAW_DOWN_DURATION );
        ++index;
    }

    _elapsed = 0.0f;
}

void ResearchVState::DoClickedEndButton() {
    _view._model->ConfirmPurchases();
}

#pragma endregion ResearchState

#pragma region IdleState

IdleVState::IdleVState( GameView& view ) : GameViewState( view ) {}
IdleVState::~IdleVState() {}

void IdleVState::Update( float delta ) {
    for ( CardWrapper* card : _view._hand ) {
        if ( card->state != CardWrapper::State::DRAGGING )
            card->visual = CanDragCardsOut() && (*card)->CanBePlayed() ?
                CardWrapper::Visual::HIGHLIGHT : CardWrapper::Visual::NONE;
    }
}

bool IdleVState::CanClickEndButton() { return _view._model->InIdleState() || _view._model->InPostLastGenerationState() && _view._model->CanEndTurn(); }
bool IdleVState::CanHoverHand() { return true; }
bool IdleVState::CanDragCardsOut() { return _view._model->CanPlayCards(); }
bool IdleVState::CanPlayCard( CardWrapper* card ) { return (*card)->CanBePlayed(); }

CardWrapper::Visual IdleVState::GetCardUnderPlayLineVisual( CardWrapper* card ) {
    return (*card)->CanBePlayed() ? CardWrapper::Visual::HIGHLIGHT
                                  : CardWrapper::Visual::NONE;
}

CardWrapper::Visual IdleVState::GetCardOverPlayLineVisual( CardWrapper* card ) {
    return CardWrapper::Visual::ACTION_HIGHLIGHT;
}

bool IdleVState::CanUseSellPatentsSP() { return _view._model->InIdleState(); }
bool IdleVState::CanUsePowerPlantSP() { return _view._model->CanUsePowerPlantSP(); }
bool IdleVState::CanUseAsteroidSP() { return _view._model->CanUseAsteroidSP(); }
bool IdleVState::CanUseAquiferSP() { return _view._model->CanUseAquiferSP(); }
bool IdleVState::CanUseGreenerySP() { return _view._model->CanUseGreenerySP(); }
bool IdleVState::CanUseCitySP() { return _view._model->CanUseCitySP(); }
bool IdleVState::CanConvertPlants() { return _view._model->CanConvertPlantsToGreenery(); }
bool IdleVState::CanConvertHeat() { return _view._model->CanConvertHeatToTemperature(); }

void IdleVState::ClickedMisc( int index ) {
    if ( _view._panel != Panel::ACTION )
        throw std::logic_error( "IdleState::ClickedMisc: actions panel was not shown!" );

    const model::decks::Card* card = **_view._action_cards[ index ];
    if ( _view._model->CanUseActions() &&
         _view._model->ActionStatus( card ) == model::decks::Availability::CAN_BE_USED ) {
        _view._panel = Panel::NONE;
        _view._model->UseAction( card );
    }
}

void IdleVState::PlayCard( int index_in_hand ) {
    _view._model->PlayCard( **_view._hand[ index_in_hand ] );

    _view._hand.erase( _view._hand.begin() + index_in_hand );
    _view.RefreshHandPositions();
}

void IdleVState::DoClickedEndButton() {
    _view._panel = Panel::NONE;

    _view._animation_queue.push( new TextAnimation(
        DEFAULT_LOCKOUT_DURATION,
        ATTRIBUTE_CHANGED_DURATION,
        "Performing Production Phase...",
        BASE_HINT_POS, BASE_HINT_POS,
        glm::vec4( LIGHT_TEXT_COLOR, 1.0f ),
        glm::vec4( LIGHT_TEXT_COLOR, 0.0f ),
        BASE_TEXT_SCALE
    ) );

    _view._model->EndTurn();
}

#pragma endregion IdleState

#pragma region SellState

SellVState::SellVState( GameView& view ) : GameViewState( view ) {}
SellVState::~SellVState() {}

void SellVState::Enter() {
    for ( CardWrapper* card : _view._hand )
        card->visual = CardWrapper::Visual::SELL_HIGHLIGHT;
}

void SellVState::Render() {
    TextRenderer::RenderTextCentered(
        std::format( "Selling Patents" ),
        BASE_HINT_POS.x, BASE_HINT_POS.y,
        BASE_TEXT_SCALE,
        LIGHT_TEXT_COLOR
    );
}

std::string SellVState::GetEndButtonText() { return "Done"; }

CardWrapper::Visual SellVState::GetCardUnderPlayLineVisual( CardWrapper* card ) {
    return CardWrapper::Visual::SELL_HIGHLIGHT;
}

CardWrapper::Visual SellVState::GetCardOverPlayLineVisual( CardWrapper* card ) {
    return CardWrapper::Visual::FADED;
}

bool SellVState::CanClickEndButton() { return true; }
bool SellVState::CanHoverHand() { return true; }
bool SellVState::CanDragCardsOut() { return true; }
bool SellVState::CanPlayCard( CardWrapper* card ) { return true; }

void SellVState::PlayCard( int index_in_hand ) {
    _view._model->SellCardSP( **_view._hand[ index_in_hand ] );

    _view._hand.erase( _view._hand.begin() + index_in_hand );
    _view.RefreshHandPositions();
}

void SellVState::DoClickedEndButton() {
    _view.RequestInstantStateChange( _view.CreateIdleState() );
}

#pragma endregion SellState

#pragma region PlacementConfirmationState

PlacementConfirmationVState::PlacementConfirmationVState( GameView& view, model::boards::TileType tile_type, std::vector<std::pair<int, int>> valid_positions )
    : GameViewState( view ), _tile_type( tile_type ), _valid_positions( valid_positions ) {}

PlacementConfirmationVState::~PlacementConfirmationVState() {}

void PlacementConfirmationVState::Enter() {
    ColorBordersSelectable();
}

void PlacementConfirmationVState::Render() {
    std::string name;
    switch ( _tile_type ) {
        case model::boards::TileType::OCEAN:
            name = "OCEAN";
            break;
        case model::boards::TileType::GREENERY:
            name = "GREENERY";
            break;
        case model::boards::TileType::CITY:
            name = "CITY";
            break;
        default:
            throw std::logic_error( "PlacementConfirmationState::Render: invalid tile type!" );
    }
    TextRenderer::RenderTextCentered(
        std::format( "Placing tile: {}", name ),
        BASE_HINT_POS.x, BASE_HINT_POS.y,
        BASE_TEXT_SCALE,
        LIGHT_TEXT_COLOR
    );
}

bool PlacementConfirmationVState::CanHoverHand() {
    return true;
}

void PlacementConfirmationVState::ClickedOnTile( TileWrapper& tile ) {
    if ( tile.selectable ) {
        ColorBordersIdle();
        auto [q, r] = tile->get_indices();
        _view._model->TilePlacementConfirmed( q, r );
        _view.RequestInstantStateChange( _view.CreateIdleState() );
    }
}

void PlacementConfirmationVState::ColorBordersIdle() {
    for ( TileWrapper& tile : _view._tiles ) {
        tile.selectable = false;
        if ( tile->get_type() == model::boards::TileType::RESERVED_FOR_OCEAN )
            tile.border_color = TILE_BORDER_COLOR_FOR_OCEAN;
        else if ( tile->get_type() == model::boards::TileType::RESERVED_FOR_NOCTIS )
            tile.border_color = TILE_BORDER_COLOR_FOR_NOCTIS;
        else
            tile.border_color = TILE_BORDER_COLOR_IDLE;
    }
}

void PlacementConfirmationVState::ColorBordersSelectable() {
    for ( TileWrapper& tile : _view._tiles )
        tile.border_color = TILE_BORDER_COLOR_NON_SELECTABLE;

    for ( auto& [q, r] : _valid_positions ) {
        _view._indexable_tiles[ r ][ q ]->selectable = true;
        _view._indexable_tiles[ r ][ q ]->border_color = TILE_BORDER_COLOR_SELECTABLE;
    }
}

#pragma endregion PlacementConfirmationState

#pragma region PaymentConfirmationState

PaymentConfirmationVState::PaymentConfirmationVState( GameView& view, int amount, model::Resource resource, int resource_value )
    : GameViewState( view ), _amount( amount ), _resource_value( resource_value ) {
    switch ( resource ) {
        case model::Resource::STEEL:
            _resource = "STEEL";
            break;
        case model::Resource::TITANIUM:
            _resource = "TITANIUM";
            break;
        default:
            throw std::logic_error( "PaymentConfirmationVState::ctor: resource type was not supported!" );
    }

    int credits = _view._model->get_local_player()->GetResource( model::Resource::CREDIT );
    int resources = _view._model->get_local_player()->GetResource( resource );

    _max_credit = glm::min( credits, _amount );
    _min_resource = CalculateResourceNeeded( _max_credit, _amount, _resource_value );

    if ( resources * _resource_value <= _amount ) {
        _max_resource = resources;
        _current_resource = _max_resource;
        _min_credit = _amount - resources * _resource_value;
        _current_credit = _min_credit;
    } else {
        _current_resource = glm::max( _amount / _resource_value, _min_resource );
        _max_resource = _current_resource;
        _current_credit = glm::min( _amount - _current_resource * _resource_value, _max_credit );
        _min_credit = 0;
        if ( _current_credit != 0 )
            ++_max_resource;
    }
}

PaymentConfirmationVState::~PaymentConfirmationVState() {}

void PaymentConfirmationVState::Enter() {
    for ( CardWrapper* card : _view._hand )
        card->visual = CardWrapper::Visual::NONE;
}

void PaymentConfirmationVState::RenderGUI() {
    ImGui::SetNextWindowSize( ImVec2( 560, 0 ) );
    if ( ImGui::Begin( "Confirm Payment", NULL, ImGuiWindowFlags_NoCollapse ) ) {
        ImGui::SetWindowFontScale( 1.5f );
        int total = _current_credit + _current_resource * _resource_value;
        ImGui::Text( "Cost to pay: %i", _amount );
        ImGui::Text( "Can pay with: %s", _resource.c_str() );
        ImGui::Text( "Value of your resources: %i", _resource_value );
        ImGui::Separator();
        ImGui::Text( "%i + %i * %i = %i", _current_credit, _current_resource, _resource_value, total );
        if ( ImGui::SliderInt( "Credit", &_current_credit, _min_credit, _max_credit ) ) {
            _current_resource = CalculateResourceNeeded( _current_credit, _amount, _resource_value );
        }
        if ( ImGui::SliderInt( "Resource", &_current_resource, _min_resource, _max_resource ) ) {
            _current_credit = CalculateCreditNeeded( _current_resource, _amount, _resource_value );
        }

        if ( total > _amount ) {
            static const ImVec4 color( NEGATIVE_TEXT_COLOR.r, NEGATIVE_TEXT_COLOR.g, NEGATIVE_TEXT_COLOR.b, 1.0f );
            ImGui::TextColored( color, "WARNING! You are paying more than you need!" );
        }
        if ( total < _amount )
            throw std::logic_error( "PaymentConfirmationState::RenderGUI: total payment was lover than the cost!" );

        if ( ImGui::Button( "Confirm Payment", ImVec2( 180, 30 ) ) ) {
            _view._model->PaymentConfirmed( _current_credit, _current_resource );
            _view.RequestInstantStateChange( _view.CreateIdleState() );
        }
    }
    ImGui::End();
}

bool PaymentConfirmationVState::CanHoverHand() { return true; }

int PaymentConfirmationVState::CalculateResourceNeeded( int credit, int amount, int resource_value ) {
    return (glm::max( amount - credit, 0 ) + resource_value - 1) / resource_value;
}

int PaymentConfirmationVState::CalculateCreditNeeded( int resource, int amount, int resource_value ) {
    return glm::max( amount - resource * resource_value, 0 );
}

#pragma endregion PaymentConfirmationState

#pragma region PostLastGenerationState

PostLastGenerationVState::PostLastGenerationVState( GameView& view ) : GameViewState( view ) {}
PostLastGenerationVState::~PostLastGenerationVState() {}

#pragma endregion PostLastGenerationState

#pragma region GameOverState

GameOverVState::GameOverVState( GameView& view ) : GameViewState( view ) {}
GameOverVState::~GameOverVState() {}

bool GameOverVState::CanHoverHand() { return true; }

#pragma endregion GameOverState
}
