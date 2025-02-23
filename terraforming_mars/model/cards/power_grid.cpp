#include "power_grid.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
PowerGrid::PowerGrid() noexcept :
    AutomatedCard( CardID::POWER_GRID, 18, false, false ) {
    AddTag( Tag::POWER );
}

PowerGrid::~PowerGrid() noexcept {}

void PowerGrid::ApplyImmediateEffects( const GameModel& model ) {
    _owner->GainEnergyProduction( _owner->GetTagCount( Tag::POWER ) );
}
}
