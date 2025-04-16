#include "beam_from_a_thorium_asteroid.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
