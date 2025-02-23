#include "research_outpost.h"

#include "../active_card_with_effect.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
ResearchOutpost::ResearchOutpost() noexcept :
    ActiveCardWithEffect( CardID::RESEARCH_OUTPOST, 18, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::SCIENCE );
    AddTag( Tag::CITY );
}

ResearchOutpost::~ResearchOutpost() noexcept {}

bool ResearchOutpost::SatisfiesRequirements( const GameModel& model ) const {
    return model.IsAvailableLonelyTile();
}

void ResearchOutpost::ApplyImmediateEffects( const GameModel& model ) {
    _owner->PlaceLonelyCity();
}

int ResearchOutpost::DoModifyCardCost( int cost, const Card* card ) {
    return cost - 1;
}
}
