#include "plantation.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
Plantation::Plantation( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::PLANTATION, 15 ) {
    AddTag( Tag::PLANT );
}

Plantation::~Plantation() noexcept {}

bool Plantation::SatisfiesRequirements() const {
    return _holder->GetTagCount( Tag::SCIENCE ) >= 2 && _model.IsTilePlaceable( _holder );
}

void Plantation::ApplyImmediateEffects() {
    _owner->PlaceGreenery();
}
}
