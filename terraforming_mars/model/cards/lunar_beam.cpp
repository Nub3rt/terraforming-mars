#include "lunar_beam.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
LunarBeam::LunarBeam( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::LUNAR_BEAM, 13, false, false ) {
    AddTag( Tag::POWER );
    AddTag( Tag::EARTH );
}

LunarBeam::~LunarBeam() noexcept {}

bool LunarBeam::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::CREDIT ) >= 2;
}

void LunarBeam::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 2 );

    _owner->GainResourceProduction( Resource::ENERGY, 2 );
    _owner->GainResourceProduction( Resource::HEAT, 2 );
}
}
