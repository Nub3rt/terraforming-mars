#include "food_factory.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
FoodFactory::FoodFactory( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::FOOD_FACTORY, 12 ) {
    AddTag( Tag::BUILDING );
}

FoodFactory::~FoodFactory() noexcept {}

bool FoodFactory::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::PLANTS ) >= 1;
}

void FoodFactory::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::PLANTS, 1 );

    _owner->GainResourceProduction( Resource::CREDIT, 4 );
}

int FoodFactory::DoCountVPs() const {
    return 1;
}
}
