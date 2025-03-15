#include "micro_mills.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
MicroMills::MicroMills( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::MICRO_MILLS, 3 ) {}

MicroMills::~MicroMills() noexcept {}

void MicroMills::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::HEAT, 1 );
}
}
