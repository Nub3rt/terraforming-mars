#include "great_dam.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
GreatDam::GreatDam( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::GREAT_DAM, 12, true, false ) {
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
