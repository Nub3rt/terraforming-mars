#include "power_grid.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
PowerGrid::PowerGrid( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::POWER_GRID, 18 ) {
    AddTag( Tag::POWER );
}

PowerGrid::~PowerGrid() noexcept {}

void PowerGrid::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, _owner->GetTagCount( Tag::POWER ) );
}
}
