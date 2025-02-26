#include "shuttles.h"

#include "../active_card_with_effect.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
Shuttles::Shuttles( const GameModel& model ) noexcept :
    ActiveCardWithEffect( model, CardID::SHUTTLES, 10, false, true ) {
    AddTag( Tag::SPACE );
}

Shuttles::~Shuttles() noexcept {}

bool Shuttles::SatisfiesRequirements() const {
    return _model.Oxygen() >= 5 && _owner->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void Shuttles::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResourceProduction( Resource::CREDIT, 2 );
}

int Shuttles::DoCountVPs() const {
    return 1;
}

int Shuttles::DoModifyCardCost( const Card* card, int cost ) {
    if ( card->TagsOfType( Tag::SPACE ) > 0 )
        return cost - 2;
    else
        return cost;
}
}
