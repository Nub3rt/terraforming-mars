#include "advanced_ecosystems.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
AdvancedEcosystems::AdvancedEcosystems( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::ADVANCED_ECOSYSTEMS, 11, false, false ) {
    AddTag( Tag::PLANT );
    AddTag( Tag::MICROBE );
    AddTag( Tag::ANIMAL );
}

AdvancedEcosystems::~AdvancedEcosystems() noexcept {}

bool AdvancedEcosystems::SatisfiesRequirements() const {
    return _owner->GetTagCount( Tag::PLANT ) >= 1 && _owner->GetTagCount( Tag::MICROBE ) >= 1 && _owner->GetTagCount( Tag::ANIMAL ) >= 1;
}

void AdvancedEcosystems::ApplyImmediateEffects() {
}

int AdvancedEcosystems::DoCountVPs() const {
    return 3;
}
}
