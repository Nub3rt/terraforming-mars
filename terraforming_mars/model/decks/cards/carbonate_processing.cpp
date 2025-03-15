#include "carbonate_processing.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
CarbonateProcessing::CarbonateProcessing( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::CARBONATE_PROCESSING, 6 ) {
    AddTag( Tag::BUILDING );
}

CarbonateProcessing::~CarbonateProcessing() noexcept {}

bool CarbonateProcessing::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void CarbonateProcessing::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResourceProduction( Resource::HEAT, 3 );
}
}
