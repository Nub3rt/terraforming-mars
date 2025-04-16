#include "carbonate_processing.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
CarbonateProcessing::CarbonateProcessing( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::CARBONATE_PROCESSING, 6 ) {
    AddTag( Tag::BUILDING );
}

CarbonateProcessing::~CarbonateProcessing() noexcept {}

bool CarbonateProcessing::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void CarbonateProcessing::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResourceProduction( Resource::HEAT, 3 );
}
}
