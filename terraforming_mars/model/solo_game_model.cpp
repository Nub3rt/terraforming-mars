#include "solo_game_model.h"

#include <random>
#include <utility>
#include <vector>

#include "board.h"
#include "deck.h"
#include "player.h"
#include "tile_type.h"

namespace model
{
using pii = std::pair<int, int>;

SoloGameModel::SoloGameModel( int seed ) : GameModel( seed ) {}
SoloGameModel::~SoloGameModel() {}

void SoloGameModel::Initialize( board::Board* board, decks::Deck* deck ) {
    GameModel::Initialize( board, deck );

    const std::vector<pii> empties = _board->GetTilesOfType( board::TileType::EMPTY );
    int half_point = static_cast<int>( empties.size() / 2 );
    int index_of_first_city = _random() % half_point;
    int index_of_second_city = _random() % half_point + half_point;
    auto& [q1, r1] = empties[ index_of_first_city ];
    auto& [q2, r2] = empties[ index_of_second_city ];
    _board->SetTileType( q1, r1, board::TileType::CITY );
    _board->SetTileType( q2, r2, board::TileType::CITY );

    const std::vector<pii> neighbours_of_first = _board->GetNeighbouringTilesOfType( q1, r1, board::TileType::EMPTY );
    if ( neighbours_of_first.size() > 0 ) {
        auto& [q1_n, r1_n] = neighbours_of_first[ _random() / neighbours_of_first.size() ];
        _board->SetTileType( q1_n, r1_n, board::TileType::GREENERY );
    }

    const std::vector<pii> neighbours_of_second = _board->GetNeighbouringTilesOfType( q2, r2, board::TileType::EMPTY );
    if ( neighbours_of_second.size() > 0 ) {
        auto& [q2_n, r2_n] = neighbours_of_second[ _random() / neighbours_of_second.size() ];
        _board->SetTileType( q2_n, r2_n, board::TileType::GREENERY );
    }
}

Player* SoloGameModel::CreateLocalPlayer() {
    Player* player = new Player( 14, 0 );
    SubscribeCallbacksOnPlayer( player );
    return player;
}
void SoloGameModel::Player_OnDestroyResource( Player* player, Resource resource, int amount ) {}

void SoloGameModel::Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount ) {}
}
