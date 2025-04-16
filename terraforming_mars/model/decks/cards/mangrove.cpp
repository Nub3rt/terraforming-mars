#include "mangrove.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
