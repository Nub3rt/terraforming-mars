#include "view_state.h"

#include <format>
#include <stdexcept>
#include <utility>

#include "card_wrapper.h"
#include "constants.h"
#include "text_renderer.h"
#include "tile_wrapper.h"
#include "view.h"

#include "../model/boards/tile_type.h"

namespace view
{
#pragma region ViewState

ViewState::ViewState( View& view ) : _view( view ) {}
ViewState::~ViewState() {}

void ViewState::Enter() {}

void ViewState::Render() {}

bool ViewState::CanHoverHand() { return true; }
bool ViewState::CanDragCardOut( CardWrapper* card ) { return false; }

void ViewState::PlayCard( int index_in_hand ) {
    throw std::logic_error( "ViewState::PlayCard: View was in an invalid state!" );
}

void ViewState::ClickedOnTile( TileWrapper& tile ) {}

#pragma endregion ViewState

#pragma region ResearchState

ResearchVState::ResearchVState( View& view ) : ViewState( view ) {}
ResearchVState::~ResearchVState() {}

#pragma endregion ResearchState

#pragma region IdleState

IdleVState::IdleVState( View& view ) : ViewState( view ) {}
IdleVState::~IdleVState() {}

bool IdleVState::CanDragCardOut( CardWrapper* card ) {
    return (*card)->CanBePlayed();
}

void IdleVState::PlayCard( int index_in_hand ) {
    _view._model->PlayCard( **_view._hand[ index_in_hand ] );

    _view._hand.erase( _view._hand.begin() + index_in_hand );
    _view.RefreshHandPositions();
}

#pragma endregion IdleState

#pragma region SellState

SellVState::SellVState( View& view ) : ViewState( view ) {}
SellVState::~SellVState() {}

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
        0.0f, 0.9f,
        BASE_TEXT_SCALE,
        BASE_TEXT_COLOR
    );
}

bool PlacementConfirmationVState::CanHoverHand() {
    return false;
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

PaymentConfirmationVState::PaymentConfirmationVState( View& view ) : ViewState( view ) {}
PaymentConfirmationVState::~PaymentConfirmationVState() {}

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
