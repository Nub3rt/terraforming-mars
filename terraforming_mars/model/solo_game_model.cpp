#include "solo_game_model.hpp"

#include <random>
#include <utility>
#include <vector>

#include "boards/board.hpp"
#include "constants.hpp"
#include "decks/deck.hpp"
#include "player.hpp"
#include "boards/tile_type.hpp"

namespace model
{
SoloGameModel::SoloGameModel( int seed ) : GameModel( seed ) {}
SoloGameModel::~SoloGameModel() {}

void SoloGameModel::Initialize( boards::Board* board, decks::Deck* deck ) {
    GameModel::Initialize( board, deck );

    const std::vector<std::pair<int, int>> empties = _board->GetTilesOfType( boards::TileType::EMPTY );
    int half_point = static_cast<int>( empties.size() / 2 );
    int index_of_first_city = _random() % half_point;
    int index_of_second_city = _random() % half_point + half_point;
    auto& [q1, r1] = empties[ index_of_first_city ];
    auto& [q2, r2] = empties[ index_of_second_city ];
    _board->SetTileType( q1, r1, boards::TileType::CITY );
    _board->SetTileType( q2, r2, boards::TileType::CITY );

    const std::vector<std::pair<int, int>> neighbours_of_first = _board->GetNeighbouringTilesOfType( q1, r1, boards::TileType::EMPTY );
    if ( neighbours_of_first.size() > 0 ) {
        auto& [q1_n, r1_n] = neighbours_of_first[ _random() % neighbours_of_first.size() ];
        _board->SetTileType( q1_n, r1_n, boards::TileType::GREENERY );
    }

    const std::vector<std::pair<int, int>> neighbours_of_second = _board->GetNeighbouringTilesOfType( q2, r2, boards::TileType::EMPTY );
    if ( neighbours_of_second.size() > 0 ) {
        auto& [q2_n, r2_n] = neighbours_of_second[ _random() % neighbours_of_second.size() ];
        _board->SetTileType( q2_n, r2_n, boards::TileType::GREENERY );
    }
}

void SoloGameModel::Start() {
    _local_player->GainResource( Resource::CREDIT, BEGINNER_CORPORATION_CREDITS );

    std::vector<const decks::Card*> cards;
    for ( int i = 0; i < STARTING_CARD_COUNT; ++i ) {
        decks::Card* card = _deck->DrawCard();
        cards.emplace_back( card );
        _local_player->GainCard( card );
    }
    _on_draw_cards.Invoke( std::move( cards ) );
}

Player* SoloGameModel::CreateLocalPlayer() {
    Player* player = new Player( SOLO_GAME_STARTING_TR, SOLO_GAME_STARTING_RESOURCE_PRODUCTION );
    SubscribeCallbacksOnPlayer( player );
    return player;
}

IdleState* SoloGameModel::CreateIdleState() {
    return new SoloIdleState( this );
}
PostLastGenerationState* SoloGameModel::CreatePostLastGenerationState() {
    return new SoloPostLastGenerationState( this );
}

void SoloGameModel::Player_OnDestroyResource( Player* player, Resource resource, int amount ) {}
void SoloGameModel::Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount ) {}


SoloIdleState::SoloIdleState( GameModel* model ) : IdleState( model ) {}
SoloIdleState::~SoloIdleState() {}

void SoloIdleState::EndTurn() {
    _model->_local_player->PerformProductionPhase();

    if ( _model->_generation < SOLO_GAME_MAX_GENERATIONS && !_model->AreGlobalParametersFulfilled() ) {
        ++_model->_generation;
        _model->RequestStateChange( _model->CreateResearchState() );
        return;
    }

    if ( CanConvertPlantsToGreenery() ) {
        _model->_in_post_last_generation = true;
        _model->RequestStateChange( _model->CreatePostLastGenerationState() );
        return;
    }

    _model->EndGame();
}

SoloPostLastGenerationState::SoloPostLastGenerationState( GameModel* model ) : PostLastGenerationState( model ) {}
SoloPostLastGenerationState::~SoloPostLastGenerationState() {
}

void SoloPostLastGenerationState::EndTurn() {
    _model->EndGame();
}
}
