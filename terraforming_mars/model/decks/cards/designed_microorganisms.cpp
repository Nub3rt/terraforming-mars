#include "designed_microorganisms.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
DesignedMicroorganisms::DesignedMicroorganisms( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::DESIGNED_MICROORGANISMS, 16 ) {
    AddTag( Tag::SCIENCE );
    AddTag( Tag::MICROBE );
}

DesignedMicroorganisms::~DesignedMicroorganisms() noexcept {}

bool DesignedMicroorganisms::SatisfiesRequirements() const {
    return _model.Temperature() <= -14;
}

void DesignedMicroorganisms::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::PLANTS, 2 );
}
}
