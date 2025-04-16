#include "fueled_generators.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
FueledGenerators::FueledGenerators( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::FUELED_GENERATORS, 1 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

FueledGenerators::~FueledGenerators() noexcept {}

bool FueledGenerators::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::CREDIT ) >= 1 - 5;
}

void FueledGenerators::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 1 );

    _owner->GainResourceProduction( Resource::ENERGY, 1 );
}
}
