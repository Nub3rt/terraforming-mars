#include "power_grid.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
