#include "nitrophilic_moss.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
NitrophilicMoss::NitrophilicMoss( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::NITROPHILIC_MOSS, 8 ) {
    AddTag( Tag::PLANT );
}

NitrophilicMoss::~NitrophilicMoss() noexcept {}

bool NitrophilicMoss::SatisfiesRequirements() const {
    return _model.OceanCount() >= 3 && _holder->GetResource( Resource::PLANTS ) >= 2;
}

void NitrophilicMoss::ApplyImmediateEffects() {
    _owner->LoseResource( Resource::PLANTS, 2 );

    _owner->GainResourceProduction( Resource::PLANTS, 2 );
}
}
