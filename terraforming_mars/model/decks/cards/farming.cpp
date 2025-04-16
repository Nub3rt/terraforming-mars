#include "farming.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Farming::Farming( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::FARMING, 16 ) {
    AddTag( Tag::PLANT );
}

Farming::~Farming() noexcept {}

bool Farming::SatisfiesRequirements() const {
    return _model.Temperature() >= 4;
}

void Farming::ApplyImmediateEffects() {
    _owner->GainResource( Resource::PLANTS, 2 );
    _owner->GainResourceProduction( Resource::CREDIT, 2 );
    _owner->GainResourceProduction( Resource::PLANTS, 2 );
}

int Farming::DoCountVPs() const {
    return 2;
}
}
