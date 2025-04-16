#include "plantation.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
