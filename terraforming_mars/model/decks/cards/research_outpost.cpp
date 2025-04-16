#include "research_outpost.hpp"

#include "../active_card_with_effect.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
ResearchOutpost::ResearchOutpost( const GameModel& model ) noexcept :
    ActiveCardWithEffect( model, CardID::RESEARCH_OUTPOST, 18 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::SCIENCE );
    AddTag( Tag::CITY );
}

ResearchOutpost::~ResearchOutpost() noexcept {}

bool ResearchOutpost::SatisfiesRequirements() const {
    return _model.IsAvailableLonelyTile( _holder );
}

void ResearchOutpost::ApplyImmediateEffects() {
    _owner->PlaceLonelyCity();
}

int ResearchOutpost::DoModifyCardCost( const Card* card, int cost ) {
    return cost - 1;
}
}
