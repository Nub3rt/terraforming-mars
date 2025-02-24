#include "designed_microorganisms.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
DesignedMicroorganisms::DesignedMicroorganisms( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::DESIGNED_MICROORGANISMS, 16, false, false ) {
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
