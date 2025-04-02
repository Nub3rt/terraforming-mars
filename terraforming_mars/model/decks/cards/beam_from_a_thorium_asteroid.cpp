#include "beam_from_a_thorium_asteroid.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
BeamFromAThoriumAsteroid::BeamFromAThoriumAsteroid( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::BEAM_FROM_A_THORIUM_ASTEROID, 32 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::POWER );
    AddTag( Tag::JOVIAN );
}

BeamFromAThoriumAsteroid::~BeamFromAThoriumAsteroid() noexcept {}

bool BeamFromAThoriumAsteroid::SatisfiesRequirements() const {
    return _holder->GetTagCount( Tag::JOVIAN ) >= 1;
}

void BeamFromAThoriumAsteroid::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 3 );
    _owner->GainResourceProduction( Resource::HEAT, 3 );
}

int BeamFromAThoriumAsteroid::DoCountVPs() const {
    return 1;
}
}
