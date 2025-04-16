#include "great_dam.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
GreatDam::GreatDam( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::GREAT_DAM, 12 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

GreatDam::~GreatDam() noexcept {}

bool GreatDam::SatisfiesRequirements() const {
    return _model.OceanCount() >= 4;
}

void GreatDam::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 2 );
}

int GreatDam::DoCountVPs() const {
    return 1;
}
}
