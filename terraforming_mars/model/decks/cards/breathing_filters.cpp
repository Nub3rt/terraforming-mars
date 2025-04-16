#include "breathing_filters.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
BreathingFilters::BreathingFilters( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::BREATHING_FILTERS, 11 ) {
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
