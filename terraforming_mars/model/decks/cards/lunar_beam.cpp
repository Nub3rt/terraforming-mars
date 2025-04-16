#include "lunar_beam.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
LunarBeam::LunarBeam( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::LUNAR_BEAM, 13 ) {
    AddTag( Tag::POWER );
    AddTag( Tag::EARTH );
}

LunarBeam::~LunarBeam() noexcept {}

bool LunarBeam::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::CREDIT ) >= 2 - 5;
}

void LunarBeam::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 2 );

    _owner->GainResourceProduction( Resource::ENERGY, 2 );
    _owner->GainResourceProduction( Resource::HEAT, 2 );
}
}
