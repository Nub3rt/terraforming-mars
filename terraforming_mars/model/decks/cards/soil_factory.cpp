#include "soil_factory.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
SoilFactory::SoilFactory( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::SOIL_FACTORY, 9 ) {
    AddTag( Tag::BUILDING );
}

SoilFactory::~SoilFactory() noexcept {}

bool SoilFactory::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void SoilFactory::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResourceProduction( Resource::PLANTS, 1 );
}

int SoilFactory::DoCountVPs() const {
    return 1;
}
}
