#include "nitrophilic_moss.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
