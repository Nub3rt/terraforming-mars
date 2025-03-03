#include "game_model.h"

#include <stdexcept>

#include "board.h"
#include "deck.h"
#include "event.h"
#include "game_model_state.h"
#include "player.h"

namespace model
{
template<typename... Args>
using Callback = std::function<void( Args... )>;

model::GameModel::GameModel() {}

GameModel::~GameModel() {
    delete _board;
    delete _deck;
    delete _local_player;
}

void GameModel::Initialize( board::Board* board, decks::Deck* deck ) {
    if ( _initialized )
        throw std::logic_error( "GameModel::Initialize: model was already initialized!" );

    _initialized = true;

    _state = new GameModelState();

    _temperature = STARTING_TEMPTERATURE;
    _ocean_count = STARTING_OCEAN_COUNT;
    _oxygen_level = STARTING_OXYGEN_LEVEL;

    _board = board;
    _deck = deck;

    _local_player = CreateLocalPlayer();
    SubscribeCallbacksOnPlayer( _local_player );
}

void GameModel::SubscribeCallbacksOnPlayer( Player* player ) {
    player->SetOnDrawCardCallback( std::bind_front( &GameModel::OnDrawCard, this ) );

    player->SetOnRaiseTRCallback( std::bind_front( &GameModel::OnRaiseTR, this ) );

    player->SetOnRaiseTemperatureCallback( std::bind_front( &GameModel::OnRaiseTemperature, this ) );
    player->SetOnPlaceOceanCallback( std::bind_front( &GameModel::OnPlaceOcean, this ) );
    player->SetOnPlaceOceanOnNonOceanCallback( std::bind_front( &GameModel::OnPlaceOceanOnNonOcean, this ) );
    player->SetOnRaiseOxygenCallback( std::bind_front( &GameModel::OnRaiseOxygen, this ) );

    player->SetOnPlaceGreeneryCallback( std::bind_front( &GameModel::OnPlaceGreenery, this ) );
    player->SetOnPlaceGreeneryOnOceanCallback( std::bind_front( &GameModel::OnPlaceGreeneryOnOcean, this ) );
    player->SetOnPlaceCityCallback( std::bind_front( &GameModel::OnPlaceCity, this ) );
    player->SetOnPlaceNoctisCityCallback( std::bind_front( &GameModel::OnPlaceNoctisCity, this ) );
    player->SetOnPlaceLonelyCityCallback( std::bind_front( &GameModel::OnPlaceLonelyCity, this ) );
    player->SetOnPlaceUrbanizedAreaCallback( std::bind_front( &GameModel::OnPlaceUrbanizedArea, this ) );

    player->SetOnResourceAmountChangedCallback( std::bind_front( &GameModel::OnResourceAmountChanged, this ) );
    player->SetOnResourceProductionAmountChangedCallback( std::bind_front( &GameModel::OnResourceProductionAmountChanged, this ) );
    player->SetOnDestroyResourceCallback( std::bind_front( &GameModel::OnDestroyResource, this ) );
    player->SetOnDestroyResourceProductionCallback( std::bind_front( &GameModel::OnDestroyResourceProduction, this ) );
}

void GameModel::OnDrawCard( Player* player ) { _state->OnDrawCard( this, player ); }
void GameModel::OnRaiseTR( Player* player, int amount ) { _state->OnRaiseTR( this, player, amount );}
void GameModel::OnRaiseTemperature( Player* player ) { _state->OnRaiseTemperature( this, player );}
void GameModel::OnPlaceOcean( Player* player ) { _state->OnPlaceOcean( this, player );}
void GameModel::OnPlaceOceanOnNonOcean( Player* player ) { _state->OnPlaceOceanOnNonOcean( this, player );}
void GameModel::OnRaiseOxygen( Player* player ) { _state->OnRaiseOxygen( this, player );}
void GameModel::OnPlaceGreenery( Player* player ) { _state->OnPlaceGreenery( this, player );}
void GameModel::OnPlaceGreeneryOnOcean( Player* player ) { _state->OnPlaceGreeneryOnOcean( this, player );}
void GameModel::OnPlaceCity( Player* player ) { _state->OnPlaceCity( this, player );}
void GameModel::OnPlaceNoctisCity( Player* player ) { _state->OnPlaceNoctisCity( this, player );}
void GameModel::OnPlaceLonelyCity( Player* player ) { _state->OnPlaceLonelyCity( this, player );}
void GameModel::OnPlaceUrbanizedArea( Player* player ) { _state->OnPlaceUrbanizedArea( this, player );}
void GameModel::OnResourceAmountChanged( Player* player, Resource resource, int amount ) { _state->OnResourceAmountChanged( this, player, resource, amount );}
void GameModel::OnResourceProductionAmountChanged( Player* player, Resource resource, int amount ) { _state->OnResourceProductionAmountChanged( this, player, resource, amount );}
void GameModel::OnDestroyResource( Player* player, Resource resource, int amount ) { _state->OnDestroyResource( this, player, resource, amount );}
void GameModel::OnDestroyResourceProduction( Player* player, Resource resource, int amount ) { _state->OnDestroyResourceProduction( this, player, resource, amount );}
}
