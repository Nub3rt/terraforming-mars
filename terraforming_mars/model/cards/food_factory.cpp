#include "food_factory.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
FoodFactory::FoodFactory( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::FOOD_FACTORY, 12 ) {
    AddTag( Tag::BUILDING );
}

FoodFactory::~FoodFactory() noexcept {}

bool FoodFactory::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::PLANTS ) >= 1;
}

void FoodFactory::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::PLANTS, 1 );

    _owner->GainResourceProduction( Resource::CREDIT, 4 );
}

int FoodFactory::DoCountVPs() const {
    return 1;
}
}
