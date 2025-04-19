#include "view_state.hpp"

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
#include "view.hpp"

#include "../model/decks/card.hpp"
#include "../model/decks/active_card_with_action.hpp"
#include "../model/decks/availability.hpp"
#include "../model/boards/tile_type.hpp"

namespace view
{
#pragma region ViewState

ViewState::ViewState( View& view ) : _view( view ) {}
ViewState::~ViewState() {}

void ViewState::Enter() {}

void ViewState::Update( float delta ) {}
void ViewState::Render() {}
void ViewState::RenderGUI() {}

std::string ViewState::GetEndButtonText() { return "End Generation"; }
CardWrapper::Visual ViewState::GetCardUnderPlayLineVisual( CardWrapper* card ) { throw std::logic_error( "ViewState::GetCardUnderPlayLineVisual: card drag is not supported in this state!" ); }
CardWrapper::Visual ViewState::GetCardOverPlayLineVisual( CardWrapper* card ) { throw std::logic_error( "ViewState::GetCardOverPlayLineVisual: card drag is not supported in this state!" ); }
Panel ViewState::GetPanelStatus() { return Panel::NONE; }

bool ViewState::CanClickMenuButton() { return true; }
bool ViewState::CanClickEndButton() { return false; }
bool ViewState::CanHoverHand() { return false; }
bool ViewState::CanDragCardsOut() { return false; }
bool ViewState::CanPlayCard( CardWrapper* card ) { return false; }

bool ViewState::CanUseSellPatentsSP() { return false; }
bool ViewState::CanUsePowerPlantSP() { return false; }
bool ViewState::CanUseAsteroidSP() { return false; }
bool ViewState::CanUseAquiferSP() { return false; }
bool ViewState::CanUseGreenerySP() { return false; }
bool ViewState::CanUseCitySP() { return false; }
bool ViewState::CanConvertPlants() { return false; }
bool ViewState::CanConvertHeat() { return false; }

void ViewState::PlayCard( int index_in_hand ) {
    throw std::logic_error( "ViewState::PlayCard: View was in an invalid state!" );
}

void ViewState::ToggleToBuyCard( int index ) {
    throw std::logic_error( "ViewState::ToggleToBuyCard: View was in an invalid state!" );
}

void ViewState::ClickedEndButton() {
    if ( !CanClickEndButton() )
        throw std::logic_error( "ViewState::ClickedEndButton: button could not be clicked in this state!" );

    DoClickedEndButton();
}

void ViewState::ClickedOnTile( TileWrapper& tile ) {}
void ViewState::ClickedAction() {}
void ViewState::ClickedEvent() {}
void ViewState::ClickedAutomated() {}
void ViewState::ClickedEffect() {}
void ViewState::ClickedLeft() {}
void ViewState::ClickedRight() {}
void ViewState::ClickedMisc( int index ) {}

void ViewState::Model_OnResearchConfirmed( std::array<bool, model::RESEARCH_CARD_NUM> selected ) {
    throw std::logic_error( "ViewState::Model_OnResearchConfirmed: View was in an invalid state!" );
}

void ViewState::DoClickedEndButton() {}

#pragma endregion ViewState

#pragma region ResearchState

ResearchVState::ResearchVState( View& view, std::array<const model::decks::Card*, model::RESEARCH_CARD_NUM> cards )
    : ViewState( view ), _cards(), _to_buy() {
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
        if ( _in_animation_over )
            _view.SetStencilRef( STENCIL_STARTING_RESEARCH + i );

        _view.RenderCard( *_cards[ i ], -i );

        if ( _in_animation_over )
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

IdleVState::IdleVState( View& view ) : ViewState( view ) {}
IdleVState::~IdleVState() {}

void IdleVState::Update( float delta ) {
    for ( CardWrapper* card : _view._hand ) {
        if ( card->state != CardWrapper::State::DRAGGING )
            card->visual = CanDragCardsOut() && (*card)->CanBePlayed() ?
                CardWrapper::Visual::HIGHLIGHT : CardWrapper::Visual::NONE;
    }
}

void IdleVState::Render() {
    switch ( _panel ) {
        case Panel::ACTION:
            RenderPanelCards( _view._action_cards, _view._page_nums[ 0 ], true );
            break;
        case Panel::EVENT:
            RenderPanelCards( _view._event_cards, _view._page_nums[ 1 ], false );
            break;
        case Panel::AUTOMATED:
            RenderPanelCards( _view._automated_cards, _view._page_nums[ 2 ], false );
            break;
        case Panel::EFFECT:
            RenderPanelCards( _view._effect_cards, _view._page_nums[ 3 ], false );
            break;
    }
}

bool IdleVState::CanClickEndButton() { return _view._model->InIdleState(); }
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

Panel IdleVState::GetPanelStatus() { return _panel; }

bool IdleVState::CanUseSellPatentsSP() { return _view._model->InIdleState(); }
bool IdleVState::CanUsePowerPlantSP() { return _view._model->InIdleState() && _view._model->CanUsePowerPlantSP(); }
bool IdleVState::CanUseAsteroidSP() { return _view._model->InIdleState() && _view._model->CanUseAsteroidSP(); }
bool IdleVState::CanUseAquiferSP() { return _view._model->InIdleState() && _view._model->CanUseAquiferSP(); }
bool IdleVState::CanUseGreenerySP() { return _view._model->InIdleState() && _view._model->CanUseGreenerySP(); }
bool IdleVState::CanUseCitySP() { return _view._model->InIdleState() && _view._model->CanUseCitySP(); }
bool IdleVState::CanConvertPlants() { return _view._model->InIdleState() && _view._model->CanConvertPlantsToGreenery(); }
bool IdleVState::CanConvertHeat() { return _view._model->InIdleState() && _view._model->CanConvertHeatToTemperature(); }

void IdleVState::ClickedAction() {
    if ( !_view._model->InIdleState() )
        return;

    if ( _panel == Panel::ACTION )
        _panel = Panel::NONE;
    else
        _panel = Panel::ACTION;
}

void IdleVState::ClickedEvent() {
    if ( !_view._model->InIdleState() )
        return;

    if ( _panel == Panel::EVENT )
        _panel = Panel::NONE;
    else
        _panel = Panel::EVENT;
}

void IdleVState::ClickedAutomated() {
    if ( !_view._model->InIdleState() )
        return;

    if ( _panel == Panel::AUTOMATED )
        _panel = Panel::NONE;
    else
        _panel = Panel::AUTOMATED;
}

void IdleVState::ClickedEffect() {
    if ( !_view._model->InIdleState() )
        return;

    if ( _panel == Panel::EFFECT )
        _panel = Panel::NONE;
    else
        _panel = Panel::EFFECT;
}

void IdleVState::ClickedLeft() {
    switch ( _panel ) {
        case Panel::ACTION:
            TurnPageLeft( (int)_view._action_cards.size(), _view._page_nums[ 0 ] );
            break;
        case Panel::EVENT:
            TurnPageLeft( (int)_view._event_cards.size(), _view._page_nums[ 1 ] );
            break;
        case Panel::AUTOMATED:
            TurnPageLeft( (int)_view._automated_cards.size(), _view._page_nums[ 2 ] );
            break;
        case Panel::EFFECT:
            TurnPageLeft( (int)_view._effect_cards.size(), _view._page_nums[ 3 ] );
            break;
    }
}

void IdleVState::ClickedRight() {
    switch ( _panel ) {
        case Panel::ACTION:
            TurnPageRight( (int)_view._action_cards.size(), _view._page_nums[ 0 ] );
            break;
        case Panel::EVENT:
            TurnPageRight( (int)_view._event_cards.size(), _view._page_nums[ 1 ] );
            break;
        case Panel::AUTOMATED:
            TurnPageRight( (int)_view._automated_cards.size(), _view._page_nums[ 2 ] );
            break;
        case Panel::EFFECT:
            TurnPageRight( (int)_view._effect_cards.size(), _view._page_nums[ 3 ] );
            break;
    }
}

void IdleVState::ClickedMisc( int index ) {
    if ( _panel != Panel::ACTION )
        throw std::logic_error( "IdleState::ClickedMisc: actions panel was not shown!" );

    const model::decks::Card* card = *_view._action_cards[ index ];
    if ( _view._model->CanUseActions() &&
         _view._model->ActionStatus( card ) == model::decks::Availability::CAN_BE_USED ) {
        _panel = Panel::NONE;
        _view._model->UseAction( card );
    }
}

void IdleVState::PlayCard( int index_in_hand ) {
    _view._model->PlayCard( **_view._hand[ index_in_hand ] );

    _view._hand.erase( _view._hand.begin() + index_in_hand );
    _view.RefreshHandPositions();
}

void IdleVState::RenderPanelCards( std::vector<CardWrapper>& cards, int page_num, bool actions ) {
    if ( cards.size() == 0 )
        return;

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glUseProgram( _view._program_card_id );
    glBindVertexArray( _view._rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, _view._cards_texture.id );
    glUniform1i( ul( "image" ), 0 );
    glUniform1f( ul( "elapsed" ), _view._elapsed );

    int from = page_num * PANEL_ITEMS;
    int to = glm::min<int>( (int)cards.size(), from + PANEL_ITEMS );

    for ( int i = from; i < to; ++i ) {
        CardWrapper& card = cards[ i ];
        if ( actions ) {
            switch ( _view._model->ActionStatus( *card ) ) {
                case model::decks::Availability::NOT_USABLE:
                    card.visual = CardWrapper::Visual::NONE;
                    break;
                case model::decks::Availability::CAN_BE_USED:
                    card.visual = CardWrapper::Visual::ACTION_HIGHLIGHT;
                    break;
                case model::decks::Availability::USED:
                    card.visual = CardWrapper::Visual::FADED;
                    break;
                default:
                    throw std::logic_error( "IdleVState::RenderPanelCards: Unknown Availability received!" );
            }

            _view.SetStencilRef( _view._stencil_starting_misc + i % PANEL_ITEMS );
        }


        _view.RenderCard( card, -i );
    }

    glDisable( GL_BLEND );


    glUseProgram( _view._program_rectangle_id );

    glBindTexture( GL_TEXTURE_2D, _view._arrow_texture.id );

    static const float arrow_ratio = (float)_view._arrow_texture.height / _view._arrow_texture.width;
    auto [_1, _2, scale] = _view.CalculateSPPosition( 0, 1 );
    scale.x /= arrow_ratio;

    glm::mat4 translate = glm::translate( glm::vec3( 0.725f, 0.0f, PANEL_BASE_Z - 0.1f ) );

    if ( page_num > GetLeftmostPageNum( (int)cards.size() ) ) {
        static const glm::mat4 mirror = glm::rotate( glm::pi<float>(), glm::vec3( 0.0f, 0.0f, 1.0f ) );

        glm::vec3 left_scale = _view._mouse_hover_stencil == STENCIL_LEFT ?
            scale * MOUSE_HOVER_SIZE_MULTIPLIER : scale;

        glm::mat4 arrow_scale = glm::scale( left_scale );
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( mirror * translate * arrow_scale ) );

        _view.SetStencilRef( STENCIL_LEFT );

        glDrawElements( GL_TRIANGLES, _view._rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );

    }

    if ( page_num < GetRightmostPageNum((int) cards.size() ) ) {
        glm::vec3 right_scale = _view._mouse_hover_stencil == STENCIL_RIGHT ?
            scale * MOUSE_HOVER_SIZE_MULTIPLIER : scale;

        glm::mat4 arrow_scale = glm::scale( right_scale );
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( translate * arrow_scale ) );

        _view.SetStencilRef( STENCIL_RIGHT );

        glDrawElements( GL_TRIANGLES, _view._rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
    }


    _view.SetStencilRef();

    glBindTexture( GL_TEXTURE_2D, 0 );

    glBindVertexArray( 0 );
    glUseProgram( 0 );
}

void IdleVState::DoClickedEndButton() {
    _panel = Panel::NONE;

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

void IdleVState::TurnPageLeft( int card_count, int& page_num ) {
    if ( page_num > GetLeftmostPageNum( card_count ) )
        --page_num;
}

void IdleVState::TurnPageRight( int card_count, int& page_num ) {
    if ( page_num < GetRightmostPageNum( card_count ) )
        ++page_num;
}

int IdleVState::GetLeftmostPageNum( int card_count ) {
    return 0;
}

int IdleVState::GetRightmostPageNum( int card_count ) {
    if ( card_count % PANEL_ITEMS == 0 )
        return card_count / PANEL_ITEMS - 1;
    else
        return card_count / PANEL_ITEMS;
}

#pragma endregion IdleState

#pragma region SellState

SellVState::SellVState( View& view ) : ViewState( view ) {}
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

PlacementConfirmationVState::PlacementConfirmationVState( View& view, model::boards::TileType tile_type, std::vector<std::pair<int, int>> valid_positions )
    : ViewState( view ), _tile_type( tile_type ), _valid_positions( valid_positions ) {}

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

PaymentConfirmationVState::PaymentConfirmationVState( View& view, int amount, model::Resource resource, int resource_value )
    : ViewState( view ), _amount( amount ), _resource_value( resource_value ) {
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

PostLastGenerationVState::PostLastGenerationVState( View& view ) : ViewState( view ) {}
PostLastGenerationVState::~PostLastGenerationVState() {}

#pragma endregion PostLastGenerationState

#pragma region GameOverState

GameOverVState::GameOverVState( View& view ) : ViewState( view ) {}
GameOverVState::~GameOverVState() {}

#pragma endregion GameOverState
}
