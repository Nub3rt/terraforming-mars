#include "mangrove.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
Mangrove::Mangrove( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::MANGROVE, 12 ) {
    AddTag( Tag::PLANT );
}

Mangrove::~Mangrove() noexcept {}

bool Mangrove::SatisfiesRequirements() const {
    return _model.Temperature() >= 4;
}

void Mangrove::ApplyImmediateEffects() {
    _owner->PlaceGreeneryOnOcean();
}

int Mangrove::DoCountVPs() const { return 1; }
}
