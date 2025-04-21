#include "tile_wrapper.hpp"

#include <stdexcept>

#include "constants.hpp"

#include "../model/boards/tile.hpp"
#include "../model/boards/tile_type.hpp"


namespace view
{
TileWrapper::TileWrapper( const model::boards::Tile& tile ) : _tile( tile ) {
    color = GetColorForTileType( _tile.get_type() );

    if ( _tile.get_type() == model::boards::TileType::RESERVED_FOR_OCEAN )
        border_color = TILE_BORDER_COLOR_FOR_OCEAN;
    else if ( _tile.get_type() == model::boards::TileType::RESERVED_FOR_NOCTIS )
        border_color = TILE_BORDER_COLOR_FOR_NOCTIS;
    else
        border_color = TILE_BORDER_COLOR_IDLE;

    show_resources = _tile.IsEmpty();
}

TileWrapper::~TileWrapper() {}

void TileWrapper::OnTilePlaced() {
    color.UpdateAnim( GetColorForTileType( _tile.get_type() ), TILE_CHANGE_DURATION, true );

    if ( _tile.get_type() != model::boards::TileType::RESERVED_FOR_OCEAN )
        border_color = TILE_BORDER_COLOR_IDLE;

    show_resources = false;
}

void TileWrapper::Update( float delta ) {
    color.Update( delta );
}

glm::vec3 TileWrapper::GetColorForTileType( model::boards::TileType tile_type ) {
    switch ( tile_type ) {
        case model::boards::TileType::EMPTY:
        case model::boards::TileType::RESERVED_FOR_OCEAN:
        case model::boards::TileType::RESERVED_FOR_NOCTIS:
            return TILE_COLOR_EMPTY;
            break;
        case model::boards::TileType::OCEAN:
            return TILE_COLOR_OCEAN;
            break;
        case model::boards::TileType::GREENERY:
            return TILE_COLOR_GREENERY;
            break;
        case model::boards::TileType::CITY:
            return TILE_COLOR_CITY;
            break;
        default:
            throw std::logic_error( "TileWrapper::ctor: tile.get_type() was of unknown type!" );
    }
}
}
