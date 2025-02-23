#include "mangrove.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
Mangrove::Mangrove() noexcept :
    AutomatedCard( CardID::MANGROVE, 12, false, false ) {
    AddTag( Tag::PLANT );
}

Mangrove::~Mangrove() noexcept {}

bool Mangrove::SatisfiesRequirements( const GameModel& _model ) const {
    return _model.Temperature() >= 4;
}

void Mangrove::ApplyImmediateEffects( const GameModel& _model ) {
    _owner->PlaceGreeneryOnOcean();
}

int Mangrove::DoCountVPs( const GameModel& _model ) const { return 1; }
}
