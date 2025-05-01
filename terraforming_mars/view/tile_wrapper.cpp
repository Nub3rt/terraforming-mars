#include "tile_wrapper.hpp"

#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtx/easing.hpp>

#include "constants.hpp"

#include "../model/boards/tile.hpp"
#include "../model/boards/tile_type.hpp"


namespace view
{
TileWrapper::TileWrapper( const model::boards::Tile& tile ) : _tile( tile ) {
    if ( _tile.get_type() == model::boards::TileType::RESERVED_FOR_OCEAN )
        border_color = TILE_BORDER_COLOR_FOR_OCEAN;
    else if ( _tile.get_type() == model::boards::TileType::RESERVED_FOR_NOCTIS )
        border_color = TILE_BORDER_COLOR_FOR_NOCTIS;
    else
        border_color = TILE_BORDER_COLOR_IDLE;

    mars_to_special.SetEase( glm::sineEaseInOut<float> );
    special_z.SetEase( glm::quarticEaseOut<float> );

    show_resources = _tile.IsEmpty();

    if ( !_tile.IsEmpty() ) {
        special = _tile.get_type();

        if ( special == Special::GREENERY )
            mars_to_special.Set( 1.0f );

        else if ( special == Special::OCEAN )
            special_z.Set( -0.005f );

        else if ( special == Special::CITY )
            special_z.Set( 0.0f );
    }
}

TileWrapper::~TileWrapper() {}

void TileWrapper::OnTilePlaced() {
    special = _tile.get_type();

    if ( special == Special::GREENERY )
        mars_to_special.SetAnim( 0.0f, 1.0f, TILE_CHANGE_DURATION );

    special_z.SetAnim( -0.1f, -0.01f, TILE_CHANGE_DURATION );

    if ( _tile.get_type() != model::boards::TileType::OCEAN )
        border_color = TILE_BORDER_COLOR_IDLE;

    show_resources = false;
}

void TileWrapper::Update( float delta ) {
    mars_to_special.Update( delta );
    special_z.Update( delta );
}
}
