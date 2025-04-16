#include "micro_mills.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
MicroMills::MicroMills( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::MICRO_MILLS, 3 ) {}

MicroMills::~MicroMills() noexcept {}

void MicroMills::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::HEAT, 1 );
}
}
