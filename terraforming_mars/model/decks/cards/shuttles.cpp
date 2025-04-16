#include "shuttles.hpp"

#include "../active_card_with_effect.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Shuttles::Shuttles( const GameModel& model ) noexcept :
    ActiveCardWithEffect( model, CardID::SHUTTLES, 10 ) {
    AddTag( Tag::SPACE );
}

Shuttles::~Shuttles() noexcept {}

bool Shuttles::SatisfiesRequirements() const {
    return _model.Oxygen() >= 5 && _holder->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void Shuttles::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResourceProduction( Resource::CREDIT, 2 );
}

int Shuttles::DoCountVPs() const {
    return 1;
}

int Shuttles::DoModifyCardCost( const Card* card, int cost ) {
    if ( card->HasTag( Tag::SPACE ) )
        return cost - 2;
    else
        return cost;
}
}
