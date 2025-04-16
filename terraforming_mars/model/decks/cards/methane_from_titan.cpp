#include "methane_from_titan.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
MethaneFromTitan::MethaneFromTitan( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::METHANE_FROM_TITAN, 28 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::JOVIAN );
}

MethaneFromTitan::~MethaneFromTitan() noexcept {}

bool MethaneFromTitan::SatisfiesRequirements() const {
    return _model.Oxygen() >= 2;
}

void MethaneFromTitan::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::PLANTS, 2 );
    _owner->GainResourceProduction( Resource::HEAT, 2 );
}

int MethaneFromTitan::DoCountVPs() const {
    return 2;
}
}
