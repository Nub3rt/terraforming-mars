#include "farming.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
Farming::Farming( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::FARMING, 16, false, false ) {
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
