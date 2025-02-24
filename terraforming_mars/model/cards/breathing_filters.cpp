#include "breathing_filters.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
BreathingFilters::BreathingFilters( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::BREATHING_FILTERS, 11, false, false ) {
    AddTag( Tag::SCIENCE );
}

BreathingFilters::~BreathingFilters() noexcept {}

bool BreathingFilters::SatisfiesRequirements() const {
    return _model.Oxygen() >= 7;
}

void BreathingFilters::ApplyImmediateEffects() {
}

int BreathingFilters::DoCountVPs() const {
    return 2;
}
}
