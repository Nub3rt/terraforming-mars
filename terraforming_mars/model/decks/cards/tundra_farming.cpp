#include "tundra_farming.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
TundraFarming::TundraFarming( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::TUNDRA_FARMING, 16 ) {
    AddTag( Tag::PLANT );
}

TundraFarming::~TundraFarming() noexcept {}

bool TundraFarming::SatisfiesRequirements() const {
    return _model.Temperature() >= -6;
}

void TundraFarming::ApplyImmediateEffects() {
    _owner->GainResource( Resource::PLANTS, 1 );
    _owner->GainResourceProduction( Resource::CREDIT, 2 );
    _owner->GainResourceProduction( Resource::PLANTS, 1 );
}

int TundraFarming::DoCountVPs() const {
    return 2;
}
}
