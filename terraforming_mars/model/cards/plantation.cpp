#include "plantation.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
Plantation::Plantation( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::PLANTATION, 15, false, false ) {
    AddTag( Tag::PLANT );
}

Plantation::~Plantation() noexcept {}

bool Plantation::SatisfiesRequirements() const {
    return _owner->GetTagCount( Tag::SCIENCE ) >= 2 && _model.IsTilePlaceable();
}

void Plantation::ApplyImmediateEffects() {
    _owner->PlaceGreenery();
}
}
